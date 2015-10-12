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
#include "utils/http-request.h"
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

char* session_invalid_start()
{
    char *session = INVALID_SESSION_ID;
    char *ret = (char*) malloc(sizeof(char) * 17);
    strncpy(ret, session, 17);
    return ret;
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
    size_t ret_len;
    char *ret;
    char *url = build_url(hostname, path);

    if (init_response(&res) > 0) {
        free(challenge);
        free(url);
        free(path);
        free(response);
        return session_invalid_start();
    }

    if (req_get_wr(url, &res) > 0) {
        utlog(LOG_ERR, "AVM: start_session::perform_get_req failed\n");
        if (NULL != res.ptr) {
            free(res.ptr);
        }
        free(challenge);
        free(url);
        free(path);
        free(response);
        return session_invalid_start();
    }

    session_id = parse_start_session_res(&res);

    ret_len = strlen(session_id) + 1;
    ret = (char*) malloc(sizeof(char) * ret_len);
    strncpy(ret, session_id, ret_len);

    free(res.ptr);
    free(session_id);
    free(url);
    free(path);
    free(challenge);
    free(response);

    return ret;
}

/* End a session on a given host with a given session id */
void session_end(const char *hostname, const char *session_id)
{
    // Build the path for the URL
    size_t path_len = 48;
    char *path = (char*) malloc(sizeof(char) * path_len);
    snprintf(path, path_len, "/login_sid.lua?sid=%s&logout=1337",
            session_id);

    char *url = build_url(hostname, path);

    // Perform the request
    if (req_get_wor(url) > 0) {
        utlog(LOG_ERR, "AVM: end_session::perform_get_req failed\n");
    }

    free(url);
    free(path);
    free((void*) session_id);
}
