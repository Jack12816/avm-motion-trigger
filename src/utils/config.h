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

#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

#include <libconfig.h>

struct config {
    struct config_avm {
        const char *hostname;
        const char *username;
        const wchar_t *password;
    } avm;
    struct config_device {
        const char *ain;
        enum {CMD_OFF, CMD_ON, CMD_TOGGLE, CMD_UNKNOWN} actor_command;
        int turn_off_after;
    } device;
    struct config_tholds {
        int light_sensor;
        int motion_locktime;
        enum {STATE_OFF, STATE_ON, STATE_UNKNOWN} desired_actor_state;
        int too_bright_timeout;
        int backup_action_timeout;
        int failed_backup_action_timeout;
        int desired_actor_state_missmatch_timeout;
    } tholds;
    struct config_sensor {
        int motion_gpio;
        const char *light_dev;
        int light_channel;
    } sensor;
    struct config_t *ptr;
};

/* Convert a char to a wide char */
wchar_t* strwchar_t(const char *);

/* Initialize a default config struct */
struct config* init_config(struct config*);

/* Read in a configuration from a file and return the config struct */
struct config get_config(const char*);

/* Validate a given config struct */
void validate_config(struct config*);

/* Free the config struct */
void free_config(struct config*);

#endif
