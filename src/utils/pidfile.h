/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * Project utilities
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

#ifndef UTILS_PIDFILE_H
#define UTILS_PIDFILE_H

/* Reads the specified pidfile and returns the read pid. 0 on errors. */
int pidfile_read(char*);

/* Check if the process already exists. If so 1 is returned, otherwise 0. */
int pidfile_check(char*);

/* Writes the pid to the specified file. 0 on errors, otherwise the pid. */
int pidfile_write(char*);

/* Remove the the specified file. The result from unlink(2) is returned */
int pidfile_remove(char*);

#endif
