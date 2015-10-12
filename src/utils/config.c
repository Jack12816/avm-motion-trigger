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
#include "config.h"
#include "../utils/logger.h"

/* Convert a char to a wide char */
wchar_t* strwchar_t(const char *str)
{
    size_t slen = strlen(str);
    size_t outlen = sizeof(wchar_t) * (slen + 1);
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
    conf->device.actor_command = ON;
    conf->device.turn_off_after = 0;

    conf->tholds.light_sensor = 0;
    conf->tholds.motion_locktime = 30;
    conf->tholds.too_bright_timeout= 30;
    conf->tholds.backup_action_timeout = 1;
    conf->tholds.failed_backup_action_timeout = 30;

    conf->sensor.motion_gpio = 0;
    conf->sensor.light_dev = "/dev/spidev0.0";
    conf->sensor.light_channel = 0;

    return conf;
}

/* Read in a configuration from a file and return the config struct */
struct config get_config(const char *path)
{
    struct config conf;
    struct config_t *c = (struct config_t*) malloc(sizeof(config_t));
    const char *avm_passwd;
    const char *actor_command = (char*) malloc(sizeof(char) * 7);
    memset((char*)actor_command, 0, sizeof(char) * 7);

    init_config(&conf);
    config_init(c);

    if (!config_read_file(c, path)) {
        utlog(LOG_ERR, "\n%s:%d - %s", config_error_file(c),
                config_error_line(c), config_error_text(c));
        config_destroy(c);
        exit(EXIT_FAILURE);
    }

    /*
     * AVM section settings
     */
    config_lookup_string(c, "hostname", &conf.avm.hostname);
    config_lookup_string(c, "username", &conf.avm.username);
    config_lookup_string(c, "password", &avm_passwd);
    free((char*) conf.avm.password);
    conf.avm.password = (const wchar_t*) strwchar_t(avm_passwd);

    /*
     * Device section settings
     */
    config_lookup_string(c, "ain", &conf.device.ain);
    config_lookup_string(c, "actor_command", &actor_command);
    config_lookup_int(c, "turn_device_off_after", &conf.device.turn_off_after);

    if (0 == strcmp("on", actor_command)) {
        conf.device.actor_command = ON;
    } else if (0 == strcmp("off", actor_command)) {
        conf.device.actor_command = OFF;
    } else if (0 == strcmp("toggle", actor_command)) {
        conf.device.actor_command = TOGGLE;
    } else {
        conf.device.actor_command = UNKNOWN;
    }

    /*
     * Sensor thresholds section settings
     */
    config_lookup_int(c, "light_sensor_thold", &conf.tholds.light_sensor);
    config_lookup_int(c, "motion_sensor_locktime", &conf.tholds.motion_locktime);
    config_lookup_int(c, "too_bright_timeout", &conf.tholds.too_bright_timeout);
    config_lookup_int(c, "backup_action_timeout", &conf.tholds.backup_action_timeout);
    config_lookup_int(c, "failed_backup_action_timeout",
            &conf.tholds.failed_backup_action_timeout);

    /*
     * Sensor ports section settings
     */
    config_lookup_int(c, "motion_sensor_gpio", &conf.sensor.motion_gpio);
    config_lookup_string(c, "light_sensor_device", &conf.sensor.light_dev);
    config_lookup_int(c, "light_sensor_channel", &conf.sensor.light_channel);

    free((char*) actor_command);
    conf.ptr = c;

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

    if (UNKNOWN == conf->device.actor_command) {
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

/* Free the config struct */
void free_config(struct config *conf)
{
    free((char*) conf->avm.password);

    if (NULL != conf->ptr) {
        config_destroy(conf->ptr);
        free(conf->ptr);
    }
}
