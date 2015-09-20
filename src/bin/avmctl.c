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
#include <string.h>
#include <wchar.h>
#include <getopt.h>
#include "../config.h"

void print_help()
{
    printf("avmctl [OPTION] {COMMAND} ...\n");
    printf("\n");
    printf("Control AVM Smart Home devices.\n");
    printf("\n");
    printf("  -h --help         Show the available operations / arguments\n");
    printf("  -c --config       Set the path to a config file\n");
    printf("  -H --host         Set a host for the operations (overwrites config host)\n");
    printf("  -u --user         Set a username for session (overwrites config username)\n");
    printf("  -p --password     Set a password for the session (overwrites config password)\n");
    printf("\n");
    printf("Commands:\n");
    printf("  check-config      Try to start a test session and on success end it\n");
    printf("  list              List all usable actors and print some meta information about them\n");
    printf("  present [AIN]     Check if the configured actor is present/connected\n");
    printf("  state [AIN]       Retrieve the current state of the actor (EXIT 0 = on, EXIT_FAILURE 1 = off)\n");
    printf("  status [AIN]      Print information about presence and state of the actor\n");
    printf("  toggle [AIN]      Toggle the state of the actor\n");
    printf("  off [AIN]         Turn the actor off\n");
    printf("  on [AIN]          Turn the actor on\n");
    printf("\n");
    printf("Common hints:\n");
    printf("  All configuration arguments (-H, -u, -p and any [AIN]) overwrites their counter\n");
    printf("  parts from a configuration file. This is can be handy if you just want to\n");
    printf("  operate on another actor with the same credentials or on another FRITZ!Box with\n");
    printf("  the same username and password.\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    if (1 == argc) {
        print_help();
    }

    int c;
    char *config_file = "";
    char *hostname    = "";
    char *username    = "";
    char *password    = "";
    char *ain         = "";

    while (1) {

        static struct option long_options[] = {
            {"help",     no_argument,       0, 'h'},
            {"config",   required_argument, 0, 'c'},
            {"host",     required_argument, 0, 'H'},
            {"user",     required_argument, 0, 'u'},
            {"password", required_argument, 0, 'p'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "hc:H:u:p:",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'h':
                print_help();
                break;

            case 'c':
                config_file = optarg;
                break;

            case 'H':
                hostname = optarg;
                break;

            case 'u':
                username = optarg;
                break;

            case 'p':
                password = optarg;
                break;

            case '?':
                // getopt_long already printed an error message.
                printf("\n");
                print_help();
                break;

            default:
                print_help();
                break;
        }
    }

    struct config conf;

    if (0 != strlen(config_file)) {
        // Config file was set, so load it
        conf = get_config(config_file, VERBOSE_QUITE);
    }

    if (0 != strlen(hostname)) {
        conf.avm.hostname = hostname;
    }

    if (0 != strlen(username)) {
        conf.avm.username = username;
    }

    if (0 != strlen(password)) {
        conf.avm.password = (const wchar_t*) strwchar_t(password);
    }

    printf("cur hostname: %s\n", conf.avm.hostname);
    printf("cur username: %s\n", conf.avm.username);
    printf("cur password: %s\n", (char*)conf.avm.password);
    printf("cur ain: %s\n", conf.device.ain);

    return 0;
}
