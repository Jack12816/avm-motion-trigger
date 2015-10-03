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
#include <wctype.h>
#include <locale.h>
#include <iconv.h>
#include <openssl/md5.h>
#include "encoding.h"

/* Convert a wide character string to a AVM compatible one */
wchar_t* striso8859_1(const wchar_t *str)
{
    size_t len = wcslen(str);
    wchar_t* compatstr = (wchar_t*) malloc(sizeof(wchar_t) * (len + 1));
    memset(compatstr, 0, sizeof(wchar_t) * (len + 1));

    for (int i = 0; i < len; i++) {
        if (str[i] > 255) {
            compatstr[i] = 0x2e;
        } else {
            compatstr[i] = str[i];
        }
    }

    return compatstr;
}

/* Convert a wide character string to UTF-16 Little Endian */
struct strutf16le* strutf16le(const wchar_t *str)
{
    size_t input_len = wcslen(str);
    size_t input_byte_len = sizeof(wchar_t) * (input_len);
    size_t output_byte_len = sizeof(wchar_t) * (input_len);

    wchar_t* input = (wchar_t*) malloc(input_byte_len + (sizeof(wchar_t) * 1));
    char* output = (char*) malloc(output_byte_len + (sizeof(wchar_t) * 1));
    struct strutf16le *result = (struct strutf16le*) malloc(sizeof(struct strutf16le));

    char* input_ptr = (char*) input;
    char* output_ptr = output;

    memcpy(input, str, input_byte_len);

    iconv_t cd = iconv_open("UTF-16LE", "WCHAR_T");

    while(input_byte_len > 0) {
        iconv(cd, &input_ptr, &input_byte_len, &output_ptr, &output_byte_len);
    }

    result->ptr = output;
    result->len = output_byte_len;

    // Terminate the output string.
    if (output_byte_len >= sizeof(wchar_t)) {
        *((wchar_t*) output_ptr) = L'\0';
    }

    iconv_close(cd);
    free(input);

    return result;
}

/* Build for a given string a MD5 hash */
char* strmd5(const unsigned char *str, int len)
{
    size_t output_len = MD5_DIGEST_LENGTH * 2;
    unsigned char output[output_len];
    char *res = (char*) malloc(sizeof(char) * (output_len + 1));

    MD5(str, len, output);

    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        snprintf(&(res[i*2]), MD5_DIGEST_LENGTH + 1,
                "%02x", (unsigned int) output[i]);
    }

    return res;
}
