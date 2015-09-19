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

#ifndef AVM_PASSWORD_RESPONSE_H
#define AVM_PASSWORD_RESPONSE_H

struct strutf16le {
    char *ptr;
    int len;
};

wchar_t* striso8859_1(const wchar_t*);
struct strutf16le* strutf16le(const wchar_t*);
char* strmd5(const unsigned char*, int);
wchar_t* passwd_response(const wchar_t*, const wchar_t*);

#endif
