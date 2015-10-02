/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * Sensors - a sensor abstraction interface
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

#ifndef SENSORS_PIR_MOTION_H
#define SENSORS_PIR_MOTION_H

#include <stdint.h>
#include <bcm2835.h>

/* Initialize the PIR motion detection sensor on a given GPIO pin, 0 on success */
int pirmtn_init(uint8_t);

/* Reset the PIR motion detection sensor */
void pirmtn_reset();

/* Check for a detection, 0 if nothing was found, 1 motion detected */
int pirmtn_detected();

/* Close PIR motion detection sensor connection */
void pirmtn_close();

#endif
