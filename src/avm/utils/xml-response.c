/*
 * Copyright (c) 2015 Hermann Mayer
 *
 * AVM - Interface the Smart Home solutions
 *
 * This file is part of avm-motion-trigger.
 *
 * avm-motion-trigger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * avm-motion-trigger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avm-motion-trigger.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "../../utils/logger.h"
#include "xml-response.h"

/* Evaluate a given XPath expression and return the resulting string */
char* xml_read_chars(const char *xpath, struct response *res)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    char *output;
    size_t ret_len;
    char *ret;
    const xmlChar* xpathExpr = (xmlChar*) xpath;

    /*
     * The document being in memory, it have no base per RFC 2396,
     * and the "noname.xml" argument will serve as its base.
     */
    doc = xmlReadMemory(res->ptr, res->len, "noname.xml", NULL, 0);

    if (NULL == doc) {
        utlog(LOG_ERR, "AVM: Failed to parse XML response\n");
        return "";
    }

    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);

    if (NULL == xpathCtx) {
        utlog(LOG_ERR, "AVM: xml_read_chars::xmlXPathNewContext() failed\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return "";
    }

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);

    if (NULL == xpathObj) {
        utlog(LOG_ERR, "AVM: xml_read_chars::xmlXPathEvalExpression() failed\n");
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return "";
    }

    if (xpathObj->nodesetval->nodeNr < 1) {
        utlog(LOG_ERR, "AVM: xml_read_chars() node not found\n");
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return "";
    }

    output = (char*) xmlNodeListGetString(
        doc, xpathObj->nodesetval->nodeTab[0]->xmlChildrenNode, 1
    );

    ret_len = strlen(output) + 1;
    ret = (char*) malloc(sizeof(char) * ret_len);
    strncpy(ret, output, ret_len);

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    free(output);
    xmlCleanupParser();

    return ret;
}
