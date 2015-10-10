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
#include "../utils/logger.h"
#include "../utils/pidfile.h"
#include "../utils/config.h"
#include "../avm/session.h"
#include "../avm/switches.h"
#include "../sensors/pir-motion.h"
#include "../sensors/ambient-light.h"

static const char *pidfile = "/run/avm-motion-triggerd.pid";
static struct config conf;

int switch_action(struct config *c)
{
    char *session_id;
    char *statew;
    char state;

    session_id = session_start(c->avm.hostname, c->avm.username,
            c->avm.password);

    if (SESSION_INVALID == session_id_chk(session_id)) {
        utlog(LOG_ERR, "%s\n%s\n", "Failed to login while starting a session.",
                "Maybe the username/password is wrong or could not contact the FRITZ!Box.\n");
        free(session_id);
        return 1;
    }

    switch (c->device.actor_command) {
        case ON:
            state = switch_on(c->avm.hostname, session_id, c->device.ain);
            break;
        case OFF:
            state = switch_off(c->avm.hostname, session_id, c->device.ain);
            break;
        case TOGGLE:
            state = switch_toggle(c->avm.hostname, session_id, c->device.ain);
            break;
        default:
            break;
    }

    if (SWITCH_STATE_ON == state) {
        statew = "on";
    } else {
        statew = "off";
    }

    utlog(LOG_NOTICE, "  %s was turned %s\n", c->device.ain, statew);
    session_end(c->avm.hostname, session_id);
    return 0;
}

int switch_action_off(struct config *c)
{
    int cur_actn = c->device.actor_command;
    c->device.actor_command = OFF;
    int ret = switch_action(c);
    c->device.actor_command = cur_actn;
    return ret;
}

void detect_motions(struct config *conf)
{
    int allvl = 0;
    utlog(LOG_INFO, "Started watching for motions..\n");

    while (1) {

        // When a motion is detected check the light sensor for the current
        // ambient light level and if the threshold is passed, turn the
        // configured actor (AVM lib) on. Wait for the configured timeout, if
        // set, turn out the actor. While this time range, don't detect new
        // motions. If no timeout was configured, wait for a configured bounce
        // lock time to prevent jitter.

        if (1 == pirmtn_detected()) {

            utlog(LOG_NOTICE, "A motion was detected\n");

            // Check if the light_sensor is zero,
            // then we skip the light sensor check
            if (0 != conf->tholds.light_sensor) {

                allvl = amblght_level();

                if (allvl >= conf->tholds.light_sensor) {
                    // It is too bright in here, so its unlikely to change in 30 secs
                    utlog(LOG_INFO, "  %s (%d) %s (%d), %s\n", "The ambient light level",
                            allvl, "passed the threshold", conf->tholds.light_sensor,
                            "it's too bright in here");
                    sleep(30);
                    continue;
                }

                utlog(LOG_INFO, "  %s (%d) %s (%d), %s\n", "The ambient light level",
                        allvl, "did not passed the threshold", conf->tholds.light_sensor,
                        "let's trigger the actor");
            }

            if (0 < switch_action(conf)) {
                // Something went wrong, so wait a little while and try again
                sleep(1);
                if (0 < switch_action(conf)) {
                    // The second try wasn't successful either, so skip this cicle
                    sleep(30);
                    continue;
                }
            }

            if (conf->device.turn_off_after > 0) {
                utlog(LOG_INFO, "  Wait %d secs then turn the actor off\n",
                        conf->device.turn_off_after);
                sleep(conf->device.turn_off_after);
                switch_action_off(conf);
                pirmtn_reset();
                continue;
            }

            if (conf->tholds.motion_locktime > 0) {
                utlog(LOG_INFO, "  Wait %d secs to prevent jitter\n",
                        conf->tholds.motion_locktime);
                sleep(conf->tholds.motion_locktime);
                pirmtn_reset();
                continue;
            }
        }
    }
}

void init_sensors(struct config *conf)
{
    if (0 != pirmtn_init((uint8_t) conf->sensor.motion_gpio)) {
        utlog(LOG_ERR, "Failed to initialize the PIR motion detection sensor.\n");
        exit(EXIT_FAILURE);
    }

    if (0 != conf->tholds.light_sensor) {
        if (0 != amblght_init(conf->sensor.light_dev,
                    (uint8_t) conf->sensor.light_channel)) {
            utlog(LOG_ERR, "Failed to initialize the Ambient Light sensor.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        utlog(LOG_NOTICE, "Disabled the ambient light sensor..");
    }
}

void handle_signal(int signo)
{
    if (SIGINT == signo || SIGTERM == signo) {
        utlog(LOG_NOTICE, "Received SIGINT/TERM signal, shuting down..");
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
    utlog(LOG_INFO, "avm-motion-triggerd [OPTION]\n");
    utlog(LOG_INFO, "\n");
    utlog(LOG_INFO, "Watch for motions and trigger actions on AVM Smart Home switches.\n");
    utlog(LOG_INFO, "\n");
    utlog(LOG_INFO, "  -h --help         Show the available arguments\n");
    utlog(LOG_INFO, "  -c --config       Set the path to a config file\n");
    utlog(LOG_INFO, "  -f --foreground   Keep the daemon running in foreground\n");
    exit(exit_code);
}

int main(int argc, char **argv)
{
    if (1 == argc) {
        utlog(LOG_ERR, "No configuration file was specified\n\n");
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
                utlog(LOG_INFO, "\n");
                print_help(EXIT_FAILURE);
                break;

            default:
                break;
        }
    }

    pid_t pid;

    if (0 != (pid = pidfile_check(pidfile))) {
        utlog(LOG_ERR, "A instance of avm-motion-triggerd (%d) is already running\n",
                pid);
        exit(EXIT_FAILURE);
    }

    conf = get_config(config_file);
    validate_config(&conf);

    // Run as daemon if we should
    if (0 == foreground) {
        daemonize();
        utlog_mode(LOG_BACKGROUND);
        utlog_pri_mode(LOG_PRI_DISABLE);
    } else {
        utlog_mode(LOG_FOREGROUND);
        utlog_pri_mode(LOG_PRI_ENABLE);
    }

    // Write a pidfile for the current daemon process
    if (0 == pidfile_write(pidfile)) {
        exit(EXIT_FAILURE);
    }

    // Bind the signal handler
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Initialize the sensors
    init_sensors(&conf);

    // Call the business logic loop
    detect_motions(&conf);

    return EXIT_SUCCESS;
}
