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
#include "password-challenge.h"

char* parse_challenge_res(struct response *res)
{
    return xml_read_chars("/SessionInfo/Challenge", res);
}

/* Retrieve a challenge */
char* passwd_challenge(const char *hostname)
{
    char *challenge;
    size_t ret_len;
    char *ret;
    struct response res;

    init_response(&res);

    if (req_get_wr(build_url(hostname, "/login_sid.lua"), &res) > 0) {
        fwprintf(stderr, L"get_challenge::perform_get_req failed\n");
        exit(EXIT_FAILURE);
    }

    challenge = parse_challenge_res(&res);

    ret_len = strlen(challenge) + 1;
    ret = (char*) malloc(sizeof(char) * ret_len);
    strncpy(ret, challenge, ret_len);

    free(res.ptr);
    free(challenge);

    return ret;
}
