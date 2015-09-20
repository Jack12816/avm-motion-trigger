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

#include <stdlib.h>
#include "switches/operations.h"

/* Retrieve a list of all available switches */
int switches_list(const char*, const char*, char* [], size_t);

/* Retrieve a user defined name for a switch */
char* switch_name(const char*, const char*, const char*);

/* Check if a switch is present (connected) */
char switch_present(const char*, const char*, const char*);

/* Get the current state of switch */
char switch_state(const char*, const char*, const char*);

/* Toggle the state of a switch and retrieve the new state */
char switch_toggle(const char*, const char*, const char*);

/* Turn a switch on and retrieve the new state */
char switch_on(const char*, const char*, const char*);

/* Turn a switch off and retrieve the new state */
char switch_off(const char*, const char*, const char*);

#endif
