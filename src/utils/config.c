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
struct config get_config(const char *path, char verbosity)
{
    struct config conf;
    config_t c;
    const char *avm_passwd;

    init_config(&conf);
    config_init(&c);

    if (!config_read_file(&c, path)) {
        fwprintf(stderr, L"\n%s:%d - %s", config_error_file(&c),
                config_error_line(&c), config_error_text(&c));
        config_destroy(&c);
        exit(EXIT_FAILURE);
    }

    /*
     * AVM section settings
     */
    if (!config_lookup_string(&c, "hostname", &conf.avm.hostname)) {
        if (verbosity >= VERBOSE_INFO) {
            fwprintf(stderr, L"[INFO] No hostname was configured, so we use 'fritz.box'\n");
        }
    }

    if (!config_lookup_string(&c, "username", &conf.avm.username)) {
        if (verbosity >= VERBOSE_DEBUG) {
            fwprintf(stderr, L"[DEBUG] No username was configured\n");
        }
    }

    if (!config_lookup_string(&c, "password", &avm_passwd)) {
        if (verbosity >= VERBOSE_DEBUG) {
            fwprintf(stderr, L"[DEBUG] No password was configured, use default one\n");
        }
    }
    conf.avm.password = (const wchar_t*) strwchar_t(avm_passwd);

    /*
     * Device section settings
     */
    if (!config_lookup_string(&c, "ain", &conf.device.ain)) {
        if (verbosity >= VERBOSE_DEBUG) {
            fwprintf(stderr, L"[DEBUG] No device ain was configured\n");
        }
    }

    if (!config_lookup_string(&c, "actor_command", &conf.device.actor_command)) {
        if (verbosity >= VERBOSE_DEBUG) {
            fwprintf(stderr, L"[DEBUG] No actor command was configured\n");
        }
    }

    if (!config_lookup_int(&c, "turn_device_off_after", &conf.device.turn_off_after)) {
        if (verbosity >= VERBOSE_INFO) {
            fwprintf(stderr, L"%s - %s", "[INFO] No timeout for auto device turn off was configured",
                    "it won't turn off\n");
        }
    }

    /*
     * Sensor thresholds section settings
     */
    if (!config_lookup_int(&c, "light_sensor_thold", &conf.tholds.light_sensor)) {
        if (verbosity >= VERBOSE_INFO) {
            fwprintf(stderr, L"%s - %s", "[INFO] No threshold for light sensor was configured",
                    "we won't take care of the ambient light\n");
        }
    }

    if (!config_lookup_int(&c, "motion_sensor_locktime", &conf.tholds.motion_locktime)) {
        if (verbosity >= VERBOSE_INFO) {
            fwprintf(stderr, L"%s - %s", "[INFO] No sensor lock time was configured",
                    "we won't prevent jitter\n");
        }
    }

    /*
     * Sensor ports section settings
     */
    if (!config_lookup_int(&c, "motion_sensor_gpio", &conf.sensor.motion_gpio)) {
        if (verbosity >= VERBOSE_INFO) {
            fwprintf(stderr, L"%s%s", "[ERROR] No GPIO pin for the motion sensor was configured",
                    "(this is mandatory)");
        }
    }

    if (!config_lookup_int(&c, "light_sensor_channel", &conf.sensor.light_channel)) {
        if (verbosity >= VERBOSE_INFO) {
            fwprintf(stderr, L"%s - %s", "[INFO] No channel for the light sensor on the ADC was configured",
                    "we won't take care of the ambient light\n");
        }
    }

    return conf;
}
