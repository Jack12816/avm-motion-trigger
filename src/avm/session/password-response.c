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
#include "../utils/encoding.h"
#include "password-response.h"

/* Build a password response form a challenge and the cleartext password */
char* passwd_response(const char *challenge, const wchar_t *password)
{
    size_t challenge_len = strlen(challenge);
    size_t md5_src_len  = challenge_len + wcslen(password) + 2;
    size_t response_len = challenge_len + 32 + 2;

    struct strutf16le *md5_src_utf16le;
    wchar_t *md5_src = malloc(sizeof(wchar_t) * md5_src_len);
    char* response = malloc(sizeof(char) * response_len);
    char *md5;

    // Prepare md5 source (challenge-password)
    swprintf(md5_src, md5_src_len, L"%hs-%ls", challenge,
            striso8859_1(password));

    md5_src_utf16le = strutf16le(md5_src);

    // Generate the md5 hash
    md5 = strmd5((const unsigned char *) md5_src_utf16le->ptr,
            md5_src_utf16le->len);

    // Build the response
    snprintf(response, response_len, "%s-%s", challenge, md5);

    free(md5_src);

    return response;
}
