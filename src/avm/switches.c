/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * AVM - Interface the Smart Home solutions
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
#include "switches.h"

/* Retrieve a list of all available switches */
int switches_list(const char *hostname, const char *session_id,
        char* ains[], size_t size)
{
    char *res = req_swoa_chars(SWITCH_CMD_LIST, hostname, session_id);

    if (0 == strlen(res)) {
        free(res);
        return 0;
    }

    short i = 0;
    short len = 0;
    char *delimiter = ",";
    char *ptr = strtok(res, delimiter);

    while (ptr != NULL && i < size) {

        len = strlen(ptr) + 1;
        ains[i] = (char*) malloc(sizeof(char) * len);
        strncpy(ains[i], ptr, len);

        ptr = strtok(NULL, delimiter);

        if (ptr != NULL) {
            i++;
        }
    }

    free(res);
    return ++i;
}

/* Retrieve a user defined name for a switch */
char* switch_name(const char *hostname, const char *session_id, const char *ain)
{
    return req_swa_chars(SWITCH_CMD_NAME, hostname, session_id, ain);
}

/* Check if a switch is present (connected) */
char switch_present(const char *hostname, const char *session_id, const char *ain)
{
    return req_swa_char(SWITCH_CMD_PRESENT, hostname, session_id, ain);
}

/* Get the current state of switch */
char switch_state(const char *hostname, const char *session_id, const char *ain)
{
    return req_swa_char(SWITCH_CMD_STATE, hostname, session_id, ain);
}

/* Toggle the state of a switch and retrieve the new state */
char switch_toggle(const char *hostname, const char *session_id, const char *ain)
{
    return req_swa_char(SWITCH_CMD_TOGGLE, hostname, session_id, ain);
}

/* Turn a switch on and retrieve the new state */
char switch_on(const char *hostname, const char *session_id, const char *ain)
{
    return req_swa_char(SWITCH_CMD_ON, hostname, session_id, ain);
}

/* Turn a switch off and retrieve the new state */
char switch_off(const char *hostname, const char *session_id, const char *ain)
{
    return req_swa_char(SWITCH_CMD_OFF, hostname, session_id, ain);
}
