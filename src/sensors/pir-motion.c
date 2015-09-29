/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * AVM - Interface the Smart Home solutions
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

#include "pir-motion.h"

static uint8_t pirmtn_pin;

/* Initialize the PIR motion detection sensor on a given GPIO pin, 0 on success */
int pirmtn_init(uint8_t pin)
{
    if (!bcm2835_init()) {
        return 1;
    }

    // Set PIN to be an input
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);

    // with a pulldown
    bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_DOWN);

    // And a rising edge detect enable
    bcm2835_gpio_aren(pin);

    // Wait little while for the init jitter on the GPIO
    delay(500);

    // Now clear the eds flag by setting it to 1
    bcm2835_gpio_set_eds(pin);

    // Set the given pin as our static default one
    pirmtn_pin = pin;

    return 0;
}

/* Check for a detection, 0 if nothing was found, 1 motion detected */
int pirmtn_detected()
{
    // Wait a little while, we don't wanna be to fast
    delay(500);

    if (bcm2835_gpio_eds(pirmtn_pin)) {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(pirmtn_pin);
        return 1;
    }

    return 0;
}

/* Close PIR motion detection sensor connection */
void pirmtn_close()
{
    bcm2835_close();
}
