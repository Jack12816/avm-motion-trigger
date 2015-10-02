#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "../avm/utils/encoding.h"
#include "../avm/session/password-response.h"

int main(void)
{
    setlocale(LC_CTYPE, "");

    const char *challenge = "1234567z";
    const wchar_t *password = L"äbc";
    wchar_t *iso_password = striso8859_1(password);

    char *response = passwd_response(challenge, password);

    wprintf(L"          password: %ls\n", password);
    wprintf(L"      new password: %ls\n", iso_password);
    wprintf(L"\n");
    wprintf(L"         challenge: %s\n", challenge);
    wprintf(L"\n");
    wprintf(L"   static response: %ls\n", L"1234567z-9e224a41eeefa284df7bb0f26c2913e2");
    wprintf(L"          response: %s\n", response);

    return 0;
}
