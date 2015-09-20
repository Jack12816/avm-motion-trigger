#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../config.h"
#include "../avm/session.h"
#include "../avm/switches.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    const char *config_file = "avm-motion-trigger.conf";
    struct config c = get_config(config_file);

    char *session_id = session_start(c.avm.hostname, c.avm.username,
            c.avm.password);

    size_t max_ains = 32;
    char* ains[max_ains];

    printf("\n");

    int found = switches_list(c.avm.hostname, session_id, ains, max_ains);

    if (0 == found) {
        printf(" * No switches found!\n");
    } else {
        for(short i = 0; i < found; i++) {
            printf(" * Found: %s\n", ains[i]);
            printf("    * Name: %s\n", switch_name(c.avm.hostname,
                        session_id, ains[i]));
        }
    }

    session_end(c.avm.hostname, session_id);

    return 0;
}
