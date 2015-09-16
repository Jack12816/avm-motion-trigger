#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <curl/curl.h>
#include "../avm/challenge.h"

int main(void)
{
    /*
     * This initialize the library and check potential ABI mismatches between
     * the version it was compiled for and the actual shared library used.
     */
    LIBXML_TEST_VERSION

    /*
     * This function sets up the program environment that libcurl needs. Think
     * of it as an extension of the library loader.
     */
    curl_global_init(CURL_GLOBAL_ALL ^ CURL_GLOBAL_SSL);

    /*
     * Get a challenge token.
     */
    char *challenge;
    challenge = get_challenge();
    printf("%s\n", challenge);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();

    return 0;
}
