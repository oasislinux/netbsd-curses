/*	$NetBSD: type_ipv4.c,v 1.11 2016/03/09 19:47:13 christos Exp $	*/

/*-
 * Copyright (c) 1998-1999 Brett Lymn
 *                         (blymn@baea.com.au, brett_lymn@yahoo.com.au)
 * All rights reserved.
 *
 * This code has been donated to The NetBSD Foundation by the Author.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "form.h"
#include "internals.h"

/*
 * The IP v4 address type handling.
 */

/*
 * define the styles of address we can have, they are:
 *    FORMI_DOTTED_QUAD    address of form aaa.bbb.ccc.ddd
 *    FORMI_HEX            address of form 0xaabbccdd
 *    FORMI_CLASSLESS      address of form aaa.bbb.ccc.ddd/ee
 */
#define FORMI_DOTTED_QUAD  0
#define FORMI_HEX          1
#define FORMI_CLASSLESS    2

/*
 * Check the contents of the field buffer are a valid IPv4 address only.
 */
static int
ipv4_check_field(FIELD *field, char *args)
{
	char buf[19], buf1[16], *p, *slash;
	unsigned int vals[4], style, mask;
	unsigned long hex_val, working;
	int i;

	if (args == NULL)
		return FALSE;
	
#ifdef DEBUG
	_formi_dbg_printf("%s: enter with args of %s\n", __func__, args);
#endif
	p = args;
	style = FORMI_DOTTED_QUAD;
	hex_val = 0;
	mask = 0;
	
	if ((slash = strchr(p, '/')) != NULL)
		style = FORMI_CLASSLESS;
	else {
		p += _formi_skip_blanks(p, 0);
		if ((p[0] != '\0') && (p[1] != '\0') &&
		    (p[0] == '0') && ((p[1] == 'x') || (p[1] == 'X')))
			style = FORMI_HEX;
	}

	switch (style) {
	case FORMI_CLASSLESS:
		mask = atoi(slash + 1);
		if (mask > 32)
			return FALSE;
		  /* FALLTHROUGH */
		
	case FORMI_DOTTED_QUAD:
		for (i = 0; i < 4; i++) {
			if ((p == NULL) || !isdigit(*p))
				return FALSE;
			vals[i] = atoi(p);
			if (vals[i] > 255)
				return FALSE;
			if ((p = strchr(p, '.')) != NULL)
				++p;
		}
		break;

		
	case FORMI_HEX:
		errno = 0;
		hex_val = strtoul(p, NULL, 16);
		if ((hex_val == ULONG_MAX) && (errno == ERANGE))
			return FALSE;
		
		working = hex_val;
		for (i = 3; i >= 0; i--) {
			vals[i] = (unsigned int)(working & 0xffUL);
			working = working >> 8;
		}
		break;

	}
	
	switch (style) {
	case FORMI_DOTTED_QUAD:
		snprintf(buf, sizeof(buf), "%d.%d.%d.%d", vals[0], vals[1],
		         vals[2], vals[3]);
		break;

	case FORMI_CLASSLESS:
		snprintf(buf, sizeof(buf), "%d.%d.%d.%d/%d", vals[0],
		         vals[1], vals[2], vals[3], mask);
		break;

	case FORMI_HEX:
		snprintf(buf, sizeof(buf), "0x%.8lx", hex_val);
		break;
	}
	
	  /* re-set the field buffer to be the reformatted IPv4 address */
	set_field_buffer(field, 0, buf);

	  /*
	   * Set the field buffer 1 to the dotted quad format regardless
	   * of the input format, only if buffer 1 exists.
	   */
	if (field->nbuf > 1) {
		snprintf(buf1, sizeof(buf1), "%d.%d.%d.%d", vals[0], vals[1],
		         vals[2], vals[3]);
		set_field_buffer(field, 1, buf1);
	}

#ifdef DEBUG
	_formi_dbg_printf("%s: buf0 set to %s\n", __func__, buf);
	_formi_dbg_printf("%s: buf1 set to %s\n", __func__, buf1);
#endif
	
	return TRUE;
}

/*
 * Check the given character is numeric, return TRUE if it is.
 */
static int
ipv4_check_char(/* ARGSUSED1 */ int c, char *args)
{
	return (isxdigit(c) || (c == '.') || (tolower(c) == 'x') ||
		(c == '/'))? TRUE : FALSE;
}

static FIELDTYPE builtin_ipv4 = {
	_TYPE_IS_BUILTIN,                   /* flags */
	0,                                  /* refcount */
	NULL,                               /* link */
	NULL,                               /* make_args */
	NULL,                               /* copy_args */
	NULL,                               /* free_args */
	ipv4_check_field,                   /* field_check */
	ipv4_check_char,                    /* char_check */
	NULL,                               /* next_choice */
	NULL                                /* prev_choice */
};

FIELDTYPE *TYPE_IPV4 = &builtin_ipv4;


