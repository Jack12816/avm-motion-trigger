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
#include <unistd.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include "pidfile.h"
#include "../utils/logger.h"

/* Reads the specified pidfile and returns the read pid. 0 on errors. */
int pidfile_read(const char *pidfile)
{
    FILE *file;
    int pid;

    if (!(file = fopen(pidfile, "r"))) {
        return 0;
    }

    fscanf(file,"%d", &pid);
    fclose(file);

    return pid;
}

/* Check if the process already exists. If so 1 is returned, otherwise 0. */
int pidfile_check(const char *pidfile)
{
    int pid = pidfile_read(pidfile);

    if (!pid) {
        return 0;
    }

    if (kill(pid, 0) && ESRCH == errno) {
        return(0);
    }

    return pid;
}

/* Writes the pid to the specified file. 0 on errors, otherwise the pid. */
int pidfile_write(const char *pidfile)
{
    FILE *file;
    int fd;
    int pid;

    if ((-1 == (fd = open(pidfile, O_RDWR|O_CREAT, 0644))) ||
        (NULL == (file = fdopen(fd, "r+")))) {
        utlog(LOG_ERR, "Can't open or create %s.\n", pidfile);
        return 0;
    }

    if (-1 == flock(fd, LOCK_EX|LOCK_NB)) {
        fscanf(file, "%d", &pid);
        fclose(file);
        utlog(LOG_ERR, "Can't lock pid file, lock is held by pid %d.\n", pid);
        return 0;
    }

    pid = getpid();

    if (!fprintf(file, "%d\n", pid)) {
        utlog(LOG_ERR, "Can't write pid to pid file.\n");
        close(fd);
        return 0;
    }

    fflush(file);

    if (-1 == flock(fd, LOCK_UN)) {
        utlog(LOG_ERR, "Can't unlock pid file %s.\n", pidfile);
        close(fd);
        return 0;
    }

    close(fd);

    return pid;
}

/* Remove the the specified file. The result from unlink(2) is returned */
int pidfile_remove(const char *pidfile)
{
    return unlink(pidfile);
}
