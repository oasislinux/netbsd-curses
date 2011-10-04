/* $NetBSD: curterm.c,v 1.7 2011/10/04 11:01:14 roy Exp $ */

/*
 * Copyright (c) 2009, 2011 The NetBSD Foundation, Inc.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Roy Marples.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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
 */

#include <sys/cdefs.h>
__RCSID("$NetBSD: curterm.c,v 1.7 2011/10/04 11:01:14 roy Exp $");

#include <assert.h>
#include <stdlib.h>
#include <term_private.h>
#include <term.h>
#include <termios.h>
#include <stdio.h>
TERMINAL *cur_term;

static const speed_t bauds[] = {
	0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 2400, 4800, 9600,
	19200, 38400, 57600, 115200, 230400, 460800, 921600
};

void
_ti_setospeed(TERMINAL *term)
{
	struct termios termios;
	speed_t os;
	size_t i;

	_DIAGASSERT(term != NULL);
	
	term->_ospeed = 0;
	if (tcgetattr(term->fildes, &termios) == 0) {
		os = cfgetospeed(&termios);
		for (i = 0; i < __arraycount(bauds); i++)
			if (bauds[i] == os) {
				term->_ospeed = i;
				break;
			}
	}
}

TERMINAL *
set_curterm(TERMINAL *nterm)
{
	TERMINAL *oterm;

	oterm = cur_term;
	cur_term = nterm;

	ospeed = 0;
	if (cur_term == NULL)
		PC = '\0';
	else {
		if (pad_char == NULL)
			PC = '\0';
		else
			PC = *pad_char;
		_ti_setospeed(nterm);
		ospeed = nterm->_ospeed;
	}

	return oterm;
}

int
del_curterm(TERMINAL *oterm)
{

	if (oterm == NULL)
		return ERR;
	free(oterm->_area);
	free(oterm->strs);
	free(oterm->nums);
	free(oterm->flags);
	free(oterm->_userdefs);
	free(oterm);
	return OK;
}

char *
termname(void)
{

        _DIAGASSERT(cur_term != NULL);
	return __UNCONST(cur_term->name);
}

static const char * nullname = '\0';

char *
longname(void)
{

	_DIAGASSERT(cur_term != NULL);
	if (cur_term->desc == NULL)
		return __UNCONST(nullname);
	return __UNCONST(cur_term->desc);
}
