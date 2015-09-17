#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.h"

int main(void)
{
    const char *config_file = "avm-motion-trigger.conf";
    struct config c;

    c = get_config(config_file);

    printf("\n");
    printf("\n");
    printf("         avm.hostname: %s\n", c.avm.hostname);
    printf("         avm.username: %s\n", c.avm.username);
    printf("         avm.password: %s\n", c.avm.password);
    printf("\n");
    printf("           device.ain: %s\n", c.device.ain);
    printf("device.turn_off_after: %d\n", c.device.turn_off_after);
    printf("\n");
    printf("  tholds.light_sensor: %d\n", c.tholds.light_sensor);
    printf("\n");
    printf("   sensor.motion_gpio: %d\n", c.sensor.motion_gpio);
    printf(" sensor.light_channel: %d\n", c.sensor.light_channel);

    return 0;
}
