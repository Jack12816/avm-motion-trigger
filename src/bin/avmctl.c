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
#include "../avm/session.h"
#include "../avm/switches.h"

char* login(struct config *c)
{
    char *session_id = (char*) malloc(sizeof(char) * 17);
    session_id = session_start(c->avm.hostname, c->avm.username,
            c->avm.password);

    if (SESSION_INVALID == session_id_chk(session_id)) {
        printf("%s\n%s\n", "Failed to login while starting a session.",
                "Wrong username or password.\n");
        exit(EXIT_FAILURE);
    }

    return session_id;
}

void replace_ain(struct config *c, char **args, size_t argc)
{
    if (1 == argc) {
        c->device.ain = args[0];
    }

    if (0 == strlen(c->device.ain)) {
        printf("%s\n", "No AIN was configured/given.\n");
        exit(EXIT_FAILURE);
    }
}

int check_config(struct config *c, char **args, size_t argc)
{
    char *session_id = login(c);
    session_end(c->avm.hostname, session_id);
    printf("Configuration is OK. (Session id was: %s)\n", session_id);
    return EXIT_SUCCESS;
}

int list(struct config *c, char **args, size_t argc)
{
    char *session_id = login(c);
    size_t max_ains = 32;
    char* ains[max_ains];

    int found = switches_list(c->avm.hostname, session_id, ains, max_ains);

    if (0 == found) {
        printf("No switches found.\n");
        return EXIT_FAILURE;
    }

    for(short i = 0; i < found; i++) {

        printf("    * Found: %s\n", ains[i]);
        printf("        * Name: %s\n", switch_name(c->avm.hostname,
                    session_id, ains[i]));

        if (SWITCH_PRESENT == switch_present(c->avm.hostname,
                    session_id, ains[i])) {
            printf("        * Present: yes (connected)\n");
        } else {
            printf("        * Present: no (not connected)\n");
        }

        if (SWITCH_STATE_ON == switch_state(c->avm.hostname,
                    session_id, ains[i])) {
            printf("        * State: on\n");
        } else {
            printf("        * State: off\n");
        }

        if (i < found-1) {
            printf("\n");
        }
    }

    session_end(c->avm.hostname, session_id);
    return EXIT_SUCCESS;
}

int present(struct config *c, char **args, size_t argc)
{
    replace_ain(c, args, argc);
    char *session_id = login(c);

    if (SWITCH_PRESENT == switch_present(c->avm.hostname,
                session_id, c->device.ain)) {
        session_end(c->avm.hostname, session_id);
        printf("%s is present\n", c->device.ain);
        return EXIT_SUCCESS;
    }

    session_end(c->avm.hostname, session_id);
    printf("%s is not present\n", c->device.ain);
    return EXIT_FAILURE;
}

int state(struct config *c, char **args, size_t argc)
{
    replace_ain(c, args, argc);
    char *session_id = login(c);

    if (SWITCH_STATE_ON == switch_state(c->avm.hostname,
                session_id, c->device.ain)) {
        session_end(c->avm.hostname, session_id);
        printf("%s is on\n", c->device.ain);
        return EXIT_SUCCESS;
    }

    session_end(c->avm.hostname, session_id);
    printf("%s is off\n", c->device.ain);
    return EXIT_FAILURE;
}

int status(struct config *c, char **args, size_t argc)
{
    replace_ain(c, args, argc);
    char *session_id = login(c);

    printf("%s\n", c->device.ain);

    if (SWITCH_PRESENT == switch_present(c->avm.hostname,
                session_id, c->device.ain)) {
        printf("  * Present: yes (connected)\n");
    } else {
        printf("  * Present: no (not connected)\n");
    }

    if (SWITCH_STATE_ON == switch_state(c->avm.hostname,
                session_id, c->device.ain)) {
        printf("  * State: on\n");
    } else {
        printf("  * State: off\n");
    }

    session_end(c->avm.hostname, session_id);
    return EXIT_SUCCESS;
}

int toggle(struct config *c, char **args, size_t argc)
{
    replace_ain(c, args, argc);
    char *session_id = login(c);

    if (SWITCH_STATE_ON == switch_toggle(c->avm.hostname,
                session_id, c->device.ain)) {
        printf("%s is now on\n", c->device.ain);
    } else {
        printf("%s is now off\n", c->device.ain);
    }

    session_end(c->avm.hostname, session_id);
    return EXIT_SUCCESS;
}

int off(struct config *c, char **args, size_t argc)
{
    replace_ain(c, args, argc);
    char *session_id = login(c);
    switch_off(c->avm.hostname, session_id, c->device.ain);
    printf("%s is now off\n", c->device.ain);
    session_end(c->avm.hostname, session_id);
    return EXIT_SUCCESS;
}

int on(struct config *c, char **args, size_t argc)
{
    replace_ain(c, args, argc);
    char *session_id = login(c);
    switch_on(c->avm.hostname, session_id, c->device.ain);
    printf("%s is now on\n", c->device.ain);
    session_end(c->avm.hostname, session_id);
    return EXIT_SUCCESS;
}

void print_help(int exit_code)
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
    printf("  present [AIN]     Check if the configured actor is present/connected (EXIT_SUCCESS = present, EXIT_FAILURE = not present)\n");
    printf("  state [AIN]       Retrieve the current state of the actor (EXIT_SUCCESS = on, EXIT_FAILURE = off)\n");
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
    exit(exit_code);
}

int main(int argc, char **argv)
{
    if (1 == argc) {
        print_help(EXIT_SUCCESS);
    }

    int c;
    char *config_file = "";
    char *hostname    = "";
    char *username    = "";
    char *password    = "";

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
                print_help(EXIT_SUCCESS);
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
                print_help(EXIT_FAILURE);
                break;

            default:
                print_help(EXIT_SUCCESS);
                break;
        }
    }

    static struct config conf;

    if (0 != strlen(config_file)) {
        // Config file was set, so load it
        conf = get_config(config_file, VERBOSE_QUITE);
    } else {
        // Init the config struct with default values
        init_config(&conf);
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

    int left = argc - optind;
    char *left_args[left-1];
    int left_i = 0, left_c = 0;

    struct {
        const char *verb;
        const int argc;
        int (* const dispatch)(struct config *c, char **args, size_t argc);
    } verbs[] = {
        { "check-config", 0, check_config },
        { "list"        , 0, list         },
        { "present"     , 1, present      },
        { "state"       , 1, state        },
        { "status"      , 1, status       },
        { "toggle"      , 1, toggle       },
        { "off"         , 1, off          },
        { "on"          , 1, on           },
        {}
    }, *verb = verbs;

    if (optind < argc) {

        // Check the not recognized args
        for (; verb->verb; verb++) {
            if (0 == strcmp(argv[optind], verb->verb)) {
                if (--left > verb->argc) {
                    fprintf(stderr, "Too many optional arguments for `%s` command\n",
                            verb->verb);
                    printf("\n");
                    print_help(EXIT_FAILURE);
                }

                // Move all left args to a new array
                optind++;
                for (left_i = 0, left_c = left; left_c > 0; left_i++, --left_c) {
                    left_args[left_i] = argv[optind+left_i];
                }

                return verb->dispatch(&conf, left_args, left);
            }
        }

        // Unknown command was specified
        fprintf(stderr, "Unknown command `%s` was specified\n", argv[optind]);
        printf("\n");
        print_help(EXIT_FAILURE);

    } else {
        // No command was specified
        fprintf(stderr, "No command was specified\n");
        printf("\n");
        print_help(EXIT_FAILURE);
    }

    return EXIT_FAILURE;
}
