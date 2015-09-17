/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * Config processing interface
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
#include <libconfig.h>
#include "config.h"

struct config get_config(const char *path)
{
    struct config conf;
    config_t c;

    config_init(&c);

    if (!config_read_file(&c, path)) {
        fprintf(stderr, "\n%s:%d - %s", config_error_file(&c),
                config_error_line(&c), config_error_text(&c));
        config_destroy(&c);
        exit(EXIT_FAILURE);
    }

    /*
     * AVM section settings
     */
    if (!config_lookup_string(&c, "hostname", &conf.avm.hostname)) {
        conf.avm.hostname = "fritz.box";
        printf("[INFO] No hostname was configured, so we use 'fritz.box'\n");
    }

    if (!config_lookup_string(&c, "username", &conf.avm.username)) {
        conf.avm.username = "";
    }

    if (!config_lookup_string(&c, "password", &conf.avm.password)) {
        conf.avm.password = "0000";
    }

    /*
     * Device section settings
     */
    if (!config_lookup_string(&c, "ain", &conf.device.ain)) {
        fprintf(stderr, "[ERROR] No device ain was configured (this is mandatory)");
        config_destroy(&c);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&c, "turn_device_off_after", &conf.device.turn_off_after)) {
        conf.device.turn_off_after = 0;
        fprintf(stderr, "%s - %s", "[INFO] No timeout for auto device turn off was configured",
                "it won't turn off\n");
    }

    /*
     * Sensor thresholds section settings
     */
    if (!config_lookup_int(&c, "light_sensor_thold", &conf.tholds.light_sensor)) {
        conf.tholds.light_sensor = 0;
        fprintf(stderr, "%s - %s", "[INFO] No threshold for light sensor was configured",
                "we won't take care of the ambient light\n");
    }

    /*
     * Sensor ports section settings
     */
    if (!config_lookup_int(&c, "motion_sensor_gpio", &conf.sensor.motion_gpio)) {
        fprintf(stderr, "%s%s", "[ERROR] No GPIO pin for the motion sensor was configured",
                "(this is mandatory)");
        config_destroy(&c);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&c, "light_sensor_channel", &conf.sensor.light_channel)) {
        conf.sensor.light_channel = 0;
        conf.tholds.light_sensor = 0;
        fprintf(stderr, "%s - %s", "[INFO] No channel for the light sensor on the ADC was configured",
                "we won't take care of the ambient light\n");
    }

    return conf;
}
