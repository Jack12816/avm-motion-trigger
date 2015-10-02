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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define SYSLOG_NAMES
#include <sys/syslog.h>

#include "logger.h"

static int log_mode = LOG_FOREGROUND;

/* Switch the logger mode */
void utlog_mode(int mode)
{
    log_mode = mode;
}

/* Log a message with a given priority, format and a variable list of arguments */
void utlog(int pri, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    if (LOG_FOREGROUND == log_mode) {

        size_t len = strlen(fmt) + 11;
        char *nfmt = (char*) malloc(sizeof(char) * len);
        char *priname = (char*) malloc(sizeof(char) * 8);

        for(CODE* scode = prioritynames; NULL != (*scode).c_name; scode++) {
            if (pri == scode->c_val) {
                for (int i = 0; i < strlen(scode->c_name); i++) {
                    priname[i] = toupper(scode->c_name[i]);
                }
            }
        }

        snprintf(nfmt, len, "[%7s] %s", priname, fmt);
        vprintf(nfmt, ap);
    }

    if (LOG_BACKGROUND == log_mode) {
        vsyslog(pri, fmt, ap);
    }

    va_end(ap);
}
