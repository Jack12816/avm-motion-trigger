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

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "ambient-light.h"

static uint8_t amblght_chnl;
static int amblght_fd = -1;

/* Initialize the Ambient Light sensor on a given SPI/Dev + SPI/ADC channel,
 * 0 on success */
int amblght_init(const char *devspi, uint8_t chnl)
{
    int ret = -1;

    amblght_fd = open(devspi, O_RDWR);

    if (amblght_fd < 0) {
        return ret;
    }

    ret = ioctl(amblght_fd, SPI_IOC_WR_MODE, SPI_MODE_0);
    if (amblght_fd < 0) {
        return ret;
    }

    ret = ioctl(amblght_fd, SPI_IOC_RD_MODE, SPI_MODE_0);
    if (amblght_fd < 0) {
        return ret;
    }

    ret = ioctl(amblght_fd, SPI_IOC_WR_BITS_PER_WORD, SPI_BITSPERWORD);
    if (amblght_fd < 0) {
        return ret;
    }

    ret = ioctl(amblght_fd, SPI_IOC_RD_BITS_PER_WORD, SPI_BITSPERWORD);
    if (amblght_fd < 0) {
        return ret;
    }

    ret = ioctl(amblght_fd, SPI_IOC_WR_MAX_SPEED_HZ, SPI_SPEED);
    if (amblght_fd < 0) {
        return ret;
    }

    ret = ioctl(amblght_fd, SPI_IOC_RD_MAX_SPEED_HZ, SPI_SPEED);
    if (amblght_fd < 0) {
        return ret;
    }

    // Set the given pin as our static default one
    amblght_chnl = chnl;

    return 0;
}

int amblght_spi_wr(unsigned char *data, int length)
{
    struct spi_ioc_transfer spi[length];

    for (int i = 0; i < length; i++) {
        spi[i].tx_buf        = (unsigned long)(data + i);
        spi[i].rx_buf        = (unsigned long)(data + i);
        spi[i].len           = sizeof(*(data + i));
        spi[i].speed_hz      = SPI_SPEED;
        spi[i].delay_usecs   = 0 ;
        spi[i].bits_per_word = SPI_BITSPERWORD;
        spi[i].cs_change     = 0;
        spi[i].tx_nbits      = 0;
        spi[i].rx_nbits      = 0;
        spi[i].pad           = 0;
    }

    return ioctl(amblght_fd, SPI_IOC_MESSAGE(length), &spi);
}

/* Read out the current ambient light level, returns 0-1023 */
int amblght_level()
{
    int lvl = 0;
    unsigned char data[3];

    // Setup the block
    data[0] = 1;
    data[1] = 0b10000000 | ((amblght_chnl & 7) << 4); // (SGL/DIF = 1, D2=D1=D0=0)
    data[2] = 0;

    // Fire the message on the SPI bus
    amblght_spi_wr(data, sizeof(data));

    // Shift a bit around to strip out the readed value
    lvl = (data[1] << 8) & 0b1100000000;
    lvl |= (data[2] & 0xff);
    lvl = 1024 - lvl;

    // The brighter the environment, the greater the level
    // 0 the darkest, 1024 the brightest
    return lvl;
}

/* Close Ambient Light sensor connection */
void amblght_close()
{
    close(amblght_fd);
}
