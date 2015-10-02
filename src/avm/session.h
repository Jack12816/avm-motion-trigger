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

#ifndef AVM_SESSION_H
#define AVM_SESSION_H

#include "utils/xml-response.h"
#include "session/password-challenge.h"
#include "session/password-response.h"

#define INVALID_SESSION_ID "0000000000000000"
#define SESSION_INVALID -1

/* Check a given session id to be valid or not */
short session_id_chk(const char*);

/* Start a new session by logging in on a given host
 * with username and password response */
char* session_start(const char*, const char*, const wchar_t*);

/* End a session on a given host with a given session id */
void session_end(const char*, const char*);

#endif
