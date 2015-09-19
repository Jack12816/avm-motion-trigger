#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../config.h"
#include "../avm/session.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    const char *config_file = "avm-motion-trigger.conf";
    struct config c = get_config(config_file);

    printf("   avm.hostname: %s\n", c.avm.hostname);
    printf("   avm.username: %s\n", c.avm.username);
    printf("   avm.password: %ls\n", c.avm.password);
    printf("\n");

    char *session_id = start_session(c.avm.hostname, c.avm.username,
            c.avm.password);

    printf("     session id: %s", session_id);

    return 0;
}
