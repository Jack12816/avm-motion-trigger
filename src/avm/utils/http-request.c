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
#include <curl/curl.h>
#include "../../utils/logger.h"
#include "http-request.h"

/* Build a URL from a given hostname and a given path */
char* build_url(const char *host, const char *path)
{
    size_t len = sizeof(char) * (7 + strlen(host) + strlen(path) + 1);

    char *scheme = "http://";
    char *url = (char*) malloc(len);

    snprintf(url, len, "%s%s%s", scheme, host, path);

    return url;
}

/* Initalize a empty response struct, >0 on errors. */
int init_response(struct response *res)
{
    res->len = 0;
    res->ptr = malloc(res->len + 1);

    if (NULL == res->ptr) {
        utlog(LOG_ERR, "AVM: init_response::malloc() failed\n");
        res->len = -1;
        return 1;
    }

    res->ptr[0] = '\0';
    return 0;
}

size_t append_response_chunk(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t new_len = size * nmemb;
    struct response *res = (struct response*) userp;

    res->ptr = realloc(res->ptr, res->len + new_len + 1);

    if(res->ptr == NULL) {
        utlog(LOG_ERR, "AVM: append_response_chunk::realloc() failed\n");
        return 0;
    }

    memcpy(&(res->ptr[res->len]), contents, new_len);
    res->len += new_len;
    res->ptr[res->len] = 0;

    return new_len;
}

/* Trim carriage return's and line feed's from a given string */
char* trimcrlf(char* str)
{
    if (strchr(str, 0xa)) {
        *strchr(str, 0xa) = 0x0;
    }

    if (strchr(str, 0xd)) {
        *strchr(str, 0xd) = 0x0;
    }

    return str;
}

/* GET request WithOut Response, >0 on errors. */
int req_get_wor(const char *url)
{
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if (NULL == curl) {
        utlog(LOG_ERR, "AVM: req_get_wor::curl_easy_init() failed\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

    result = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (CURLE_OK != result) {
        utlog(LOG_ERR, "AVM: req_get_wor::curl_easy_perform() failed (%s)\n",
                curl_easy_strerror(result));
        return 1;
    }

    return 0;
}

/* GET request With Response, >0 on errors. */
int req_get_wr(const char *url, struct response *res)
{
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if (NULL == curl) {
        utlog(LOG_ERR, "AVM: req_get_wr::curl_easy_init() failed\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, append_response_chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)res);

    result = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (CURLE_OK != result) {
        utlog(LOG_ERR, "AVM: req_get_wr::curl_easy_perform() failed (%s)\n",
                curl_easy_strerror(result));
        return 1;
    }

    return 0;
}
