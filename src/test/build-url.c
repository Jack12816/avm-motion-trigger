#include <stdio.h>
#include "../avm/http-request.h"

int main(void)
{
    printf("%s\n", build_url("gateway.lan", "/login_sid.lua"));

    return 0;
}
