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

#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

#include <sys/syslog.h>

#define LOG_FOREGROUND 0
#define LOG_BACKGROUND 1
#define LOG_PRI_DISABLE 0
#define LOG_PRI_ENABLE 1

/* Switch the logger output mode */
void utlog_mode(int);

/* Switch the logger priority mode */
void utlog_pri_mode(int);

/* Log a message with a given priority, format and a variable list of arguments */
void utlog(int, const char*, ...);

#endif
