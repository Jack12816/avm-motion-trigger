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
#include "common/http-request.h"

/* Build switch path WithOut Ain */
char* build_sw_path_woa(const char *command, const char *session_id)
{
    size_t len = 48 + strlen(command)
        + strlen(session_id);
    char *path = (char*) malloc(sizeof(char) * len);

    snprintf(path, len, "%s?switchcmd=%s&sid=%s",
            SWITCH_PATH, command, session_id);

    return path;
}

/* Build switch path With Ain */
char* build_sw_path_wa(const char *command,
        const char *session_id, const char *ain)
{
    size_t len = 53 + strlen(command)
        + strlen(session_id) + strlen(ain);
    char *path = (char*) malloc(sizeof(char) * len);

    snprintf(path, len, "%s?switchcmd=%s&sid=%s&ain=%s",
            SWITCH_PATH, command, session_id, ain);

    return path;
}

int switches_list(const char *hostname, const char *session_id,
        char* ains[], size_t size)
{
    // Perform the request
    struct response res;

    init_response(&res);

    if (req_get_wr(build_url(hostname,
                    build_sw_path_woa("getswitchlist",
                        session_id)), &res) > 0) {
        fwprintf(stderr, L"start_session::perform_get_req failed\n");
        exit(EXIT_FAILURE);
    }

    if (strlen(res.ptr) < 1) {
        free(res.ptr);
        return 0;
    }

    short i = 0;
    short len = 0;
    char *delimiter = ",";
    char *ptr = strtok(res.ptr, delimiter);

    while (ptr != NULL && i < size) {

        len = strlen(ptr) + 1;
        ains[i] = (char*) malloc(sizeof(char) * len);
        memcpy(ains[i], ptr, len);

        ptr = strtok(NULL, delimiter);

        if (ptr != NULL) {
            i++;
        }
    }

    free(res.ptr);

    return ++i;
}
