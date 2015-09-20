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

#ifndef AVM_SWITCHES_OPERATIONS_H
#define AVM_SWITCHES_OPERATIONS_H

#define SWITCH_PATH "/webservices/homeautoswitch.lua"
#define SWITCH_UNVRSL_ON 1
#define SWITCH_UNVRSL_OFF 0
#define SWITCH_STATE_ON SWITCH_UNVRSL_ON
#define SWITCH_STATE_OFF SWITCH_UNVRSL_OFF
#define SWITCH_PRESENT SWITCH_UNVRSL_ON
#define SWITCH_NOT_PRESENT SWITCH_UNVRSL_OFF

#define SWITCH_CMD_LIST    "getswitchlist"
#define SWITCH_CMD_NAME    "getswitchname"
#define SWITCH_CMD_PRESENT "getswitchpresent"
#define SWITCH_CMD_STATE   "getswitchstate"
#define SWITCH_CMD_TOGGLE  "setswitchtoggle"
#define SWITCH_CMD_ON      "setswitchon"
#define SWITCH_CMD_OFF     "setswitchoff"

/* Perform a switch command and retrieve a state (without ain) */
char req_swoa_char(const char*, const char*, const char*);

/* Perform a switch command and retrieve a state (with ain) */
char req_swa_char(const char*, const char*, const char*, const char*);

/* Perform a switch command and retrieve a string (without ain) */
char* req_swoa_chars(const char*, const char*, const char*);

/* Perform a switch command and retrieve a string (with ain) */
char* req_swa_chars(const char*, const char*, const char*, const char*);

#endif
