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
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include "../../utils/logger.h"
#include "../utils/http-request.h"
#include "operations.h"

/* Build switch path, all additional params will be in pairs of key=value */
char* build_sw_path(const char *command, const char *session_id, size_t size, ...)
{
    va_list ap, ap_len;
    va_start(ap, size);
    va_copy(ap_len, ap);

    // Compute the size of the path
    size_t size_c = size;
    short len = 48 + strlen(command) + strlen(session_id);
    char* ptr;

    for (; size_c > 0; size_c--) {
        ptr = va_arg(ap_len, char*);
        len += strlen(ptr) + 1;
    }

    // Construct the path
    char *path = (char*) malloc(sizeof(char) * len);

    snprintf(path, len, "%s?switchcmd=%s&sid=%s",
            SWITCH_PATH, command, session_id);

    for (;size > 0; size -= 2) {
        ptr = va_arg(ap, char*);
        snprintf(&(path[strlen(path)]), len, "&%s=", ptr);
        strcat(&(path[strlen(path)]), va_arg(ap, char*));
    }

    va_end(ap);
    return path;
}

char response_char(struct response res)
{
    if (0 == strcmp("1", trimcrlf(res.ptr))) {
        free(res.ptr);
        return SWITCH_UNVRSL_ON;
    }

    free(res.ptr);
    return SWITCH_UNVRSL_OFF;
}

char* response_str(struct response res)
{
    size_t len = strlen(res.ptr) + 1;
    char* str = (char*) malloc(sizeof(char) * len);
    strncpy(str, res.ptr, len);
    trimcrlf(str);

    free(res.ptr);
    return str;
}

struct response req_sw(const char *hostname, const char *command, const char *path)
{
    // Perform the request
    struct response res;
    char *url = build_url(hostname, path);

    init_response(&res);

    if (req_get_wr(url, &res) > 0) {
        utlog(LOG_ERR, "AVM: %s::req_get_wr failed\n", command);
        free(url);
        exit(EXIT_FAILURE);
    }

    free(url);

    return res;
}

/* Perform a switch command and retrieve a state (without ain) */
char req_swoa_char(const char *command, const char *hostname,
        const char *session_id)
{
    char *path = build_sw_path(command, session_id, 0);
    char res = response_char(req_sw(hostname, command, path));
    free(path);
    return res;
}

/* Perform a switch command and retrieve a state (with ain) */
char req_swa_char(const char *command, const char *hostname,
        const char *session_id, const char *ain)
{
    char *path = build_sw_path(command, session_id, 2, "ain", ain);
    char res = response_char(req_sw(hostname, command, path));
    free(path);
    return res;
}

/* Perform a switch command and retrieve a string (without ain) */
char* req_swoa_chars(const char *command, const char *hostname,
        const char *session_id)
{
    char *path = build_sw_path(command, session_id, 0);
    char *res = response_str(req_sw(hostname, command, path));
    free(path);
    return res;
}

/* Perform a switch command and retrieve a string (with ain) */
char* req_swa_chars(const char *command, const char *hostname,
        const char *session_id, const char *ain)
{
    char *path = build_sw_path(command, session_id, 2, "ain", ain);
    char *res = response_str(req_sw(hostname, command, path));
    free(path);
    return res;
}
