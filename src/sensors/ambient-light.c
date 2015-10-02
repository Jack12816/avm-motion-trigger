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

#include "ambient-light.h"

static uint8_t amblght_chnl;

/* Initialize the Ambient Light sensor on a given SPI/ADC channel, 0 on success */
int amblght_init(uint8_t chnl)
{
    // Set the given pin as our static default one
    amblght_chnl = chnl;

    return 0;
}

/* Read out the current ambient light level, returns 0-1023 */
int amblght_level()
{
    // The brighter the environment, the greater the level
    return 905;
}

/* Close Ambient Light sensor connection */
void amblght_close()
{
}
