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

void init_response(struct response*);
char* build_url(const char*, const char*);
size_t append_response_chunk(void*, size_t, size_t, struct response*);
void trimcrlf(char*);

/*
 * GET request WithOut Response
 */
int req_get_wor(const char*);

/*
 * GET request With Response
 */
int req_get_wr(const char*, struct response*);

#endif
