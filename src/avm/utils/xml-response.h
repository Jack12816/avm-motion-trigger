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

#ifndef AVM_XML_RESPONSE_H
#define AVM_XML_RESPONSE_H

#include "http-request.h"

/* Evaluate a given XPath expression and return the resulting string */
char* xml_read_chars(const char*, struct response*);

#endif
