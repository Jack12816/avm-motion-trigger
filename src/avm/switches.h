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

#ifndef AVM_SWITCHES_H
#define AVM_SWITCHES_H

#define SWITCH_PATH "/webservices/homeautoswitch.lua"
#define SWITCH_STATE_ON 1
#define SWITCH_STATE_OFF 0
#define SWITCH_PRESENT 1
#define SWITCH_NOT_PRESENT 0

#include <stdlib.h>

int switches_list(const char*, const char*, char* [], size_t);
char* switch_name(const char*, const char*, const char*);
char switch_state(const char*, const char*, const char*);
char switch_present(const char*, const char*, const char*);

#endif
