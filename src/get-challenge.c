#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

struct response {
    char *ptr;
    size_t len;
};

void init_response(struct response *res)
{
    res->len = 0;
    res->ptr = malloc(res->len + 1);

    if (NULL == res->ptr) {
        fprintf(stderr, "init_response::malloc() failed\n");
        exit(EXIT_FAILURE);
    }

    res->ptr[0] = '\0';
}

size_t append_response_chunk(void *ptr, size_t size, size_t nmemb, struct response *res)
{
    size_t new_len = res->len + size * nmemb;
    res->ptr = realloc(res->ptr, new_len + 1);

    if (NULL == res->ptr) {
        fprintf(stderr, "append_response_chunk::realloc() failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(res->ptr + res->len, ptr, size * nmemb);
    res->ptr[new_len] = '\0';
    res->len = new_len;

    return size * nmemb;
}

int perform_get_req(char *url, struct response *res)
{
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if (NULL == curl) {
        fprintf(stderr, "perform_get_req::curl_easy_init() failed\n");
        exit(EXIT_FAILURE);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, append_response_chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);

    result = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (CURLE_OK != result) {
        fprintf(stderr, "perform_get_req::curl_easy_perform() failed (%s)\n",
                curl_easy_strerror(result));
        return 1;
    }

    return 0;
}

char* parse_challenge_res(struct response *res)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    char *challenge;
    const xmlChar* xpathExpr = "/SessionInfo/Challenge";

    /*
     * The document being in memory, it have no base per RFC 2396,
     * and the "noname.xml" argument will serve as its base.
     */
    doc = xmlReadMemory(res->ptr, res->len, "noname.xml", NULL, 0);

    if (NULL == doc) {
        fprintf(stderr, "Failed to parse document\n");
        return "";
    }

    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);

    if (NULL == xpathCtx) {
        fprintf(stderr,"parse_challenge_res::xmlXPathNewContext() failed\n");
        xmlFreeDoc(doc);
        return "";
    }

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);

    if (NULL == xpathObj) {
        fprintf(stderr,"parse_challenge_res::xmlXPathEvalExpression() failed\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return "";
    }

    if (xpathObj->nodesetval->nodeNr < 1) {
        fprintf(stderr,"parse_challenge_res() challenge node not found\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return "";
    }

    challenge = (char*) xmlNodeListGetString(
        doc, xpathObj->nodesetval->nodeTab[0]->xmlChildrenNode, 1
    );

    xmlFreeDoc(doc);

    return challenge;
}

char* get_challenge()
{
    struct response res;
    char *challenge;
    int result;

    init_response(&res);
    result = perform_get_req("http://gateway.lan/login_sid.lua", &res);

    if (result > 0) {
        fprintf(stderr, "get_challenge::perform_get_req failed\n");
        exit(EXIT_FAILURE);
    }

    challenge = parse_challenge_res(&res);

    // Release some memory
    free(res.ptr);

    return challenge;
}

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
