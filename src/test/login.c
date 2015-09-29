#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../utils/config.h"
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

    char *session_id = session_start(c.avm.hostname, c.avm.username,
            c.avm.password);

    if (SESSION_INVALID == session_id_chk(session_id)) {
        printf(" * Login was not successfuly!\n");
        exit(EXIT_FAILURE);
    }

    printf("     session id: %s\n", session_id);
    printf("\n");
    printf(" --- Time to logout --- \n\n");

    session_end(c.avm.hostname, session_id);

    return 0;
}
