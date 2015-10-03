#include <stdio.h>
#include <locale.h>
#include "../avm/utils/http-request.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    char *url = build_url("gateway.lan", "/login_sid.lua");
    printf("%s\n", url);
    free(url);

    return 0;
}
