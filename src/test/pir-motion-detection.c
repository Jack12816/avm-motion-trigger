#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../sensors/pir-motion.h"

// RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN RPI_GPIO_P1_11

int main(void)
{
    if (0 != pirmtn_init((uint8_t) PIN)) {
        printf("Failed to initialize the PIR motion detection sensor.\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    while (1) {

        if (1 == pirmtn_detected()) {
            printf("[%u] Detected a motion!\n", (unsigned)time(NULL));
        }

        i++;

        if (i > 50) {
            break;
        }
    }

    pirmtn_close();
    return EXIT_SUCCESS;
}
