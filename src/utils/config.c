/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * Project utilities
 *
 * This file is part of avm-motion-trigger.
 *
 * avm-motion-trigger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * avm-motion-trigger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avm-motion-trigger.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <libconfig.h>
#include "config.h"
#include "../utils/logger.h"

/* Convert a char to a wide char */
wchar_t* strwchar_t(const char *str)
{
    size_t slen = strlen(str);
    size_t outlen = sizeof(wchar_t) * slen;
    wchar_t* out = (wchar_t*) malloc(outlen);

    swprintf(out, outlen, L"%hs", str);

    return out;
}

/* Initialize a default config struct */
struct config* init_config(struct config *conf)
{
    conf->avm.hostname = "fritz.box";
    conf->avm.username = "";
    conf->avm.password = (const wchar_t*) strwchar_t("0000");

    conf->device.ain = "";
    conf->device.actor_command = "on";
    conf->device.turn_off_after = 0;

    conf->tholds.light_sensor = 0;
    conf->tholds.motion_locktime = 30;

    conf->sensor.motion_gpio = 0;
    conf->sensor.light_channel = 0;

    return conf;
}

/* Read in a configuration from a file and return the config struct */
struct config get_config(const char *path)
{
    struct config conf;
    config_t c;
    const char *avm_passwd;

    init_config(&conf);
    config_init(&c);

    if (!config_read_file(&c, path)) {
        utlog(LOG_ERR, "\n%s:%d - %s", config_error_file(&c),
                config_error_line(&c), config_error_text(&c));
        config_destroy(&c);
        exit(EXIT_FAILURE);
    }

    /*
     * AVM section settings
     */
    config_lookup_string(&c, "hostname", &conf.avm.hostname);
    config_lookup_string(&c, "username", &conf.avm.username);
    config_lookup_string(&c, "password", &avm_passwd);
    conf.avm.password = (const wchar_t*) strwchar_t(avm_passwd);

    /*
     * Device section settings
     */
    config_lookup_string(&c, "ain", &conf.device.ain);
    config_lookup_string(&c, "actor_command", &conf.device.actor_command);
    config_lookup_int(&c, "turn_device_off_after", &conf.device.turn_off_after);

    /*
     * Sensor thresholds section settings
     */
    config_lookup_int(&c, "light_sensor_thold", &conf.tholds.light_sensor);
    config_lookup_int(&c, "motion_sensor_locktime", &conf.tholds.motion_locktime);

    /*
     * Sensor ports section settings
     */
    config_lookup_int(&c, "motion_sensor_gpio", &conf.sensor.motion_gpio);
    config_lookup_int(&c, "light_sensor_channel", &conf.sensor.light_channel);

    return conf;
}

/* Validate a given config struct */
void validate_config(struct config *conf)
{
    int err_cnt = 0;

    if (0 == strcmp("", conf->device.ain)) {
        utlog(LOG_ERR, "No actor identification number (ain) was configured.\n");
        err_cnt++;
    }

    if (0 != strcmp("on", conf->device.actor_command) &&
        0 != strcmp("off", conf->device.actor_command) &&
        0 != strcmp("toggle", conf->device.actor_command)) {
        utlog(LOG_ERR, "%s [%s].\n", "Actor command (actor_command) is none of",
                "on, off, toggle");
        err_cnt++;
    }

    if (conf->device.turn_off_after < 0) {
        utlog(LOG_ERR, "%s %s.\n", "Device turn off timeout (turn_device_off_after)",
                "needs to be greater or equal zero");
        err_cnt++;
    }

    if (conf->tholds.light_sensor < 0) {
        utlog(LOG_ERR, "%s %s.\n", "Light sensor threshold (light_sensor_thold)",
                "needs to be greater or equal zero");
        err_cnt++;
    }

    if (conf->tholds.motion_locktime < 0) {
        utlog(LOG_ERR, "%s %s.\n", "Motion sensor lock time (motion_sensor_locktime)",
                "needs to be greater or equal zero");
        err_cnt++;
    }

    if (conf->sensor.motion_gpio <= 0) {
        utlog(LOG_ERR, "%s %s.\n", "Motion sensor GPIO port (motion_sensor_gpio)",
                "needs to be greater than zero");
        err_cnt++;
    }

    if (conf->sensor.light_channel < 0) {
        utlog(LOG_ERR, "%s %s.\n", "Light sensor SPI channel (light_sensor_channel)",
                "needs to be greater or equal zero");
        err_cnt++;
    }

    if (err_cnt > 0) {
        if (err_cnt > 1) {
            utlog(LOG_ERR, "Found %d configuration errors.\n", err_cnt);
        } else {
            utlog(LOG_ERR, "Found one configuration error.\n");
        }
        exit(EXIT_FAILURE);
    }
}