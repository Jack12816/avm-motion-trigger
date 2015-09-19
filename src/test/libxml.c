#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

static const char *document = "<?xml version=\"1.0\" encoding=\"utf-8\"?><SessionInfo><SID>0000000000000000</SID><Challenge>c246e74c</Challenge><BlockTime>0</BlockTime><Rights></Rights></SessionInfo>";

/**
 * example3Func:
 * @content: the content of the document
 * @length: the length in bytes
 *
 * Parse the in memory document and free the resulting tree
 */
static void example3Func(const char *content, int length)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;

    const xmlChar* xpathExpr = (xmlChar*) "/SessionInfo/Challenge";

    /*
     * The document being in memory, it have no base per RFC 2396,
     * and the "noname.xml" argument will serve as its base.
     */
    doc = xmlReadMemory(content, length, "noname.xml", NULL, 0);

    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return;
    }

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return;
    }

    fprintf(stdout, "Challenge: %d\n", xpathObj->nodesetval->nodeNr);
    fprintf(stdout, "Challenge: %s\n",
            xmlNodeListGetString(doc, xpathObj->nodesetval->nodeTab[0]->xmlChildrenNode, 1));

    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        return;
    }
    xmlFreeDoc(doc);
}

int main(void) {
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    example3Func(document, strlen(document));

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
