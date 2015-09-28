#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

// RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN RPI_GPIO_P1_11

int main(void)
{
    if (!bcm2835_init()) {
        return 1;
    }

    // Set PIN to be an input
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);

    // with a pulldown
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_DOWN);

    // And a rising edge detect enable
    bcm2835_gpio_aren(PIN);

    while (1) {

        if (bcm2835_gpio_eds(PIN)) {
            // Now clear the eds flag by setting it to 1
            bcm2835_gpio_set_eds(PIN);
            printf("[%u] Detected a motion!\n", (unsigned)time(NULL));
        }

        // wait a bit
        delay(500);
    }

    bcm2835_close();
    return 0;
}
