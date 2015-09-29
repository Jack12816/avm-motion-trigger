/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * AVM Smart Home Control Interface
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
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <wchar.h>
#include <getopt.h>
#include <sys/stat.h>
#include "../utils/pidfile.h"
#include "../utils/config.h"
#include "../avm/session.h"
#include "../avm/switches.h"

static const char *pidfile = "/run/avm-motion-triggerd.pid";

void detect_motions(struct config *conf)
{
    printf("Started watching for motions..\n");

    while (1) {
        sleep(1);
    }
}

void handle_signal(int signo)
{
    if (SIGINT == signo || SIGTERM == signo) {
        pidfile_remove(pidfile);
        exit(EXIT_SUCCESS);
    }
}

void daemonize()
{
    pid_t pid;

    // Fork off the parent process
    pid = fork();

    // An error occurred
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // On success: The child process becomes session leader
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // Fork off for the second time
    pid = fork();

    // An error occurred
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Set new file permissions
    umask(0);

    // Change the working directory to the root directory
    chdir("/");

    // Close all open file descriptors
    for (int i = sysconf(_SC_OPEN_MAX); i > 0; i--) {
        close(i);
    }
}

void print_help(int exit_code)
{
    printf("avm-motion-triggerd [OPTION]\n");
    printf("\n");
    printf("Watch for motions and trigger actions on AVM Smart Home switches.\n");
    printf("\n");
    printf("  -h --help         Show the available arguments\n");
    printf("  -c --config       Set the path to a config file\n");
    printf("  -f --foreground   Keep the daemon running in foreground\n");
    exit(exit_code);
}

int main(int argc, char **argv)
{
    if (1 == argc) {
        fprintf(stderr, "No configuration file was specified\n");
        printf("\n");
        print_help(EXIT_FAILURE);
    }

    int c;
    int foreground = 0;
    char *config_file = "";

    while (1) {

        static struct option long_options[] = {
            {"help",       no_argument,       0, 'h'},
            {"config",     required_argument, 0, 'c'},
            {"foreground", no_argument,       0, 'f'},
            {0, 0, 0, 0}
        };

        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "hfc:",
                long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'h':
                print_help(EXIT_SUCCESS);
                break;

            case 'c':
                config_file = optarg;
                break;

            case 'f':
                foreground = 1;
                break;

            case '?':
                // getopt_long already printed an error message.
                printf("\n");
                print_help(EXIT_FAILURE);
                break;

            default:
                break;
        }
    }

    pid_t pid;

    if (0 != (pid = pidfile_check(pidfile))) {
        fprintf(stderr, "A instance of avm-motion-triggerd (%d) is already running\n",
                pid);
        exit(EXIT_FAILURE);
    }

    static struct config conf;
    conf = get_config(config_file);
    validate_config(&conf);

    // Run as daemon if we should
    if (0 == foreground) {
        daemonize();
    }

    // Write a pidfile for the current daemon process
    pidfile_write(pidfile);

    // Bind the signal handler
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Call the business logic loop
    detect_motions(&conf);

    return EXIT_SUCCESS;
}
