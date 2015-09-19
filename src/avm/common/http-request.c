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
#include <curl/curl.h>
#include "http-request.h"

char* build_url(const char *host, const char *path)
{
    size_t len = sizeof(char) * (7 + strlen(host) + strlen(path) + 1);

    char *scheme = "http://";
    char *url = (char*) malloc(len);

    snprintf(url, len, "%s%s%s", scheme, host, path);

    return url;
}

void init_response(struct response *res)
{
    res->len = 0;
    res->ptr = malloc(res->len + 1);

    if (NULL == res->ptr) {
        fprintf(stderr, "init_response::malloc() failed\n");
        exit(EXIT_FAILURE);
    }

    res->ptr[0] = '\0';
}

size_t append_response_chunk(void *ptr, size_t size, size_t nmemb, struct response *res)
{
    size_t new_len = res->len + size * nmemb;
    res->ptr = realloc(res->ptr, new_len + 1);

    if (NULL == res->ptr) {
        fprintf(stderr, "append_response_chunk::realloc() failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(res->ptr + res->len, ptr, size * nmemb);
    res->ptr[new_len] = '\0';
    res->len = new_len;

    return size * nmemb;
}

int perform_get_req(char *url, struct response *res)
{
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if (NULL == curl) {
        fprintf(stderr, "perform_get_req::curl_easy_init() failed\n");
        exit(EXIT_FAILURE);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, append_response_chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);

    result = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (CURLE_OK != result) {
        fprintf(stderr, "perform_get_req::curl_easy_perform() failed (%s)\n",
                curl_easy_strerror(result));
        return 1;
    }

    return 0;
}
