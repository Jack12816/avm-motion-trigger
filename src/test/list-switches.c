#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../utils/config.h"
#include "../avm/session.h"
#include "../avm/switches.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    const char *config_file = "avm-motion-trigger.conf";
    struct config c = get_config(config_file, VERBOSE_QUITE);

    char *session_id = session_start(c.avm.hostname, c.avm.username,
            c.avm.password);

    size_t max_ains = 32;
    char* ains[max_ains];

    int found = switches_list(c.avm.hostname, session_id, ains, max_ains);

    if (0 == found) {
        printf(" * No switches found!\n");
    } else {
        for(short i = 0; i < found; i++) {
            printf("    * Found: %s\n", ains[i]);
            printf("        * Name: %s\n", switch_name(c.avm.hostname,
                        session_id, ains[i]));

            if (SWITCH_PRESENT == switch_present(c.avm.hostname,
                        session_id, ains[i])) {
                printf("        * Present: yes (connected)\n");
            } else {
                printf("        * Present: no (not connected)\n");
            }

            if (SWITCH_STATE_ON == switch_state(c.avm.hostname,
                        session_id, ains[i])) {
                printf("        * State: on\n");
            } else {
                printf("        * State: off\n");
            }

            if (i < found-1) {
                printf("\n");
            }
        }
    }

    session_end(c.avm.hostname, session_id);

    return 0;
}
