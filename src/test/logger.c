#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../utils/logger.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    // Test printf variant
    utlog_mode(LOG_FOREGROUND);
    utlog(LOG_ERR, "A error %s with a number %d\n", "message", 7);
    utlog(LOG_WARNING, "A warn %s\n", "message");
    utlog(LOG_NOTICE, "A notice %s\n", "message");
    utlog(LOG_INFO, "A info %s\n", "message");
    utlog(LOG_DEBUG, "A debug %s\n", "message");

    // Test syslog variant
    openlog("avm-motion-trigger-logger-test", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);
    utlog_mode(LOG_BACKGROUND);
    utlog(LOG_ERR, "A error %s with a number %d\n", "message", 7);
    utlog(LOG_WARNING, "A warn %s\n", "message");
    utlog(LOG_NOTICE, "A notice %s\n", "message");
    utlog(LOG_INFO, "A info %s\n", "message");
    utlog(LOG_DEBUG, "A debug %s\n", "message");
    closelog();

    return 0;
}

