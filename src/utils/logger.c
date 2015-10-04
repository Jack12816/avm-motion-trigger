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
#include <time.h>
#include <stdarg.h>

#define SYSLOG_NAMES
#include <sys/syslog.h>

#include "logger.h"

static int log_out_mode = LOG_FOREGROUND;
static int log_pri_mode = LOG_PRI_DISABLE;

/* Switch the logger output mode */
void utlog_mode(int mode)
{
    log_out_mode = mode;
}

/* Switch the logger priority mode */
void utlog_pri_mode(int mode)
{
    log_pri_mode = mode;
}

/* Log a message with a given priority, format and a variable list of arguments */
void utlog(int pri, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    if (LOG_FOREGROUND == log_out_mode) {

        FILE *outfile = stdout;

        if (pri <= LOG_WARNING) {
            outfile = stderr;
        }

        if (LOG_PRI_ENABLE == log_pri_mode) {

            char cur_timew[26];
            time_t cur_time = time(NULL);
            strftime(cur_timew, 26, "%Y-%m-%dT%T%z", localtime(&cur_time));

            size_t len = strlen(fmt) + 26 + 11;
            char *nfmt = (char*) malloc(sizeof(char) * len);
            char *priname = (char*) malloc(sizeof(char) * 8);
            memset(nfmt, 0, sizeof(char) * len);
            memset(priname, 0, sizeof(char) * 8);

            for(CODE* scode = prioritynames; NULL != (*scode).c_name; scode++) {
                if (pri == scode->c_val) {
                    for (int i = 0; i < strlen(scode->c_name); i++) {
                        priname[i] = toupper(scode->c_name[i]);
                    }
                }
            }

            snprintf(nfmt, len, "[%s][%7s] %s", cur_timew, priname, fmt);
            vfprintf(outfile, nfmt, ap);
            free(nfmt);
            free(priname);

        } else {
            vfprintf(outfile, fmt, ap);
        }

        fflush(outfile);
    }

    if (LOG_BACKGROUND == log_out_mode) {
        vsyslog(pri, fmt, ap);
    }

    va_end(ap);
}
