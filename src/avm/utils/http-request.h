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

#ifndef AVM_HTTP_REQUEST_H
#define AVM_HTTP_REQUEST_H

#include <stdlib.h>

struct response {
    char *ptr;
    size_t len;
};

/* Initalize a empty response struct, >0 on errors. */
int init_response(struct response*);

/* Build a URL from a given hostname and a given path */
char* build_url(const char*, const char*);

/* Trim carriage return's and line feed's from a given string */
char* trimcrlf(char*);

/* GET request WithOut Response, >0 on errors. */
int req_get_wor(const char*);

/* GET request With Response, >0 on errors. */
int req_get_wr(const char*, struct response*);

#endif
