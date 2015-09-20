#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include "../config.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    const char *config_file = "avm-motion-trigger.conf";
    struct config c;

    c = get_config(config_file, VERBOSE_QUITE);

    wprintf(L"         avm.hostname: %s\n", c.avm.hostname);
    wprintf(L"         avm.username: %s\n", c.avm.username);
    wprintf(L"         avm.password: %ls\n", c.avm.password);
    wprintf(L"\n");
    wprintf(L"           device.ain: %s\n", c.device.ain);
    wprintf(L"device.turn_off_after: %d\n", c.device.turn_off_after);
    wprintf(L"\n");
    wprintf(L"  tholds.light_sensor: %d\n", c.tholds.light_sensor);
    wprintf(L"\n");
    wprintf(L"   sensor.motion_gpio: %d\n", c.sensor.motion_gpio);
    wprintf(L" sensor.light_channel: %d\n", c.sensor.light_channel);

    return 0;
}
