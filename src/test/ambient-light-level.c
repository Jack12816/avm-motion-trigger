#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../sensors/ambient-light.h"

#define ADC_CHANNEL 0

int main(void)
{
    if (0 != amblght_init((uint8_t) ADC_CHANNEL)) {
        printf("Failed to initialize the Ambient Light sensor.\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("[%u] Current ambient light level: %d\n", (unsigned)time(NULL),
                amblght_level());
        sleep(5);
    }

    amblght_close();
    return EXIT_SUCCESS;
}
