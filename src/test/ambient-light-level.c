#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../sensors/ambient-light.h"

#define ADC_CHANNEL 0

int main(void)
{
    if (0 != amblght_init((const char*) "/dev/spidev0.0", (uint8_t) ADC_CHANNEL)) {
        printf("Failed to initialize the Ambient Light sensor.\n");
        exit(EXIT_FAILURE);
    }

    printf("[%u] Current ambient light level: %d\n", (unsigned)time(NULL),
            amblght_level());

    amblght_close();
    return EXIT_SUCCESS;
}
