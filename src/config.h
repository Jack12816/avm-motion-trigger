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

#ifndef CONFIG_H
#define CONFIG_H

struct config {
    struct config_avm {
        const char *hostname;
        const char *username;
        const wchar_t *password;
    } avm;
    struct config_device {
        const char *ain;
        int turn_off_after;
    } device;
    struct config_tholds {
        int light_sensor;
    } tholds;
    struct config_sensor {
        int motion_gpio;
        int light_channel;
    } sensor;
};

struct config get_config(const char*);

#endif
