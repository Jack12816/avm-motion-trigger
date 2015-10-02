#include <stdio.h>
#include <locale.h>
#include "../avm/utils/http-request.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    printf("%s\n", build_url("gateway.lan", "/login_sid.lua"));

    return 0;
}
