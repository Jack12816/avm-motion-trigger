#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include "../utils/pidfile.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    char *pidfile = "/tmp/avm-motion-trigger-test.pid";

    printf(" pidfile_write (1): %d\n", pidfile_write(pidfile));
    printf("  pidfile_read (1): %d\n", pidfile_read(pidfile));
    printf(" pidfile_check (1): %d\n", pidfile_check(pidfile));
    printf("pidfile_remove (1): %d\n", pidfile_remove(pidfile));
    printf("\n");

    FILE *file;
    int fd;
    pidfile = "/tmp/avm-motion-trigger-test.1.pid";

    if ((-1 == (fd = open(pidfile, O_RDWR|O_CREAT, 0644))) ||
        (NULL == (file = fdopen(fd, "r+")))) {
        fprintf(stderr, "Can't open or create %s.\n", pidfile);
        return 0;
    }

    if (!fprintf(file, "1\n")) {
        printf("Can't write pid.\n");
        close(fd);
        return 0;
    }

    fflush(file);
    close(fd);

    printf("  pidfile_read (2): %d\n", pidfile_read(pidfile));
    printf(" pidfile_check (2): %d\n", pidfile_check(pidfile));
    printf("pidfile_remove (2): %d\n", pidfile_remove(pidfile));
    printf("pidfile_remove (2): %d\n", pidfile_remove(pidfile));

    return 0;
}
