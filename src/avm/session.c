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
#include <string.h>
#include <wchar.h>
#include "../utils/logger.h"
#include "common/http-request.h"
#include "session.h"

/* Check a given session id to be valid or not */
short session_id_chk(const char *session_id)
{
    if (0 == strcmp(session_id, INVALID_SESSION_ID)) {
        return -1;
    }

    return 1;
}

char* parse_start_session_res(struct response *res)
{
    return xml_read_chars("/SessionInfo/SID", res);
}

/* Start a new session by logging in on a given host
 * with username and password response */
char* session_start(const char *hostname, const char *username,
        const wchar_t *password)
{
    char *challenge = passwd_challenge(hostname);
    char *response  = passwd_response(challenge, password);

    // Build the path for the URL
    size_t path_len = 76 + strlen(username);
    char *path = (char*) malloc(sizeof(char) * path_len);
    snprintf(path, path_len, "/login_sid.lua?username=%s&response=%s",
            username, response);

    // Perform the request
    struct response res;
    char *session_id;

    init_response(&res);

    if (req_get_wr(build_url(hostname, path), &res) > 0) {
        utlog(LOG_ERR, "AVM: start_session::perform_get_req failed\n");
        exit(EXIT_FAILURE);
    }

    session_id = parse_start_session_res(&res);

    free(res.ptr);

    return session_id;
}

/* End a session on a given host with a given session id */
void session_end(const char *hostname, const char *session_id)
{
    // Build the path for the URL
    size_t path_len = 48;
    char *path = (char*) malloc(sizeof(char) * path_len);
    snprintf(path, path_len, "/login_sid.lua?sid=%s&logout=1337",
            session_id);

    // Perform the request
    if (req_get_wor(build_url(hostname, path)) > 0) {
        utlog(LOG_ERR, "AVM: end_session::perform_get_req failed\n");
        exit(EXIT_FAILURE);
    }
}
