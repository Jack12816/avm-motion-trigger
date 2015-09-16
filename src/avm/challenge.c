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
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "http-request.h"
#include "challenge.h"

char* parse_challenge_res(struct response *res)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    char *challenge;
    const xmlChar* xpathExpr = (xmlChar*) "/SessionInfo/Challenge";

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
