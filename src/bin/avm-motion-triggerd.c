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
        print_help(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
