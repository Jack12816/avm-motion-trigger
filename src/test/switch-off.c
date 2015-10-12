#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../utils/config.h"
#include "../avm/session.h"
#include "../avm/switches.h"

#define ANSI_COLOR_RED    "\x1b[31;1m"
#define ANSI_COLOR_YELLOW "\x1b[33;1m"
#define ANSI_COLOR_GREY   "\x1b[1;30m"
#define ANSI_COLOR_RESET  "\x1b[0m"

#define STR(s) #s
#define COLOR(color,str) color STR(str) ANSI_COLOR_RESET

char* strbystate(char state)
{
    if (SWITCH_STATE_ON == state) {
        return COLOR(ANSI_COLOR_YELLOW, on);
    } else if (SWITCH_STATE_OFF) {
        return COLOR(ANSI_COLOR_RED, off);
    }

    return COLOR(ANSI_COLOR_GREY, unknown);
}

int main(void)
{
    setlocale(LC_CTYPE, "");

    const char *config_file = "avm-motion-trigger.conf";
    struct config c = get_config(config_file);

    char *session_id = session_start(c.avm.hostname, c.avm.username,
            c.avm.password);

    char init_state = switch_state(c.avm.hostname,
                        session_id, c.device.ain);

    printf("    * Current state of %s is %s\n", c.device.ain,
            strbystate(init_state));

    printf("        * Turn off..\n");
    char should_state = switch_off(c.avm.hostname, session_id, c.device.ain);

    printf("        * Check state..\n");
    char is_state = switch_state(c.avm.hostname,
                        session_id, c.device.ain);

    printf("    * The initial state was %s so it should be %s now, and actually it is %s",
            strbystate(init_state), strbystate(should_state), strbystate(is_state));

    session_end(c.avm.hostname, session_id);
    free_config(&c);

    return 0;
}
