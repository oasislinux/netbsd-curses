/*	$NetBSD: ctrace.c,v 1.8 2000/04/16 01:16:43 thorpej Exp $	*/

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#ifndef lint
#if 0
static char sccsid[] = "@(#)ctrace.c	8.2 (Berkeley) 10/5/93";
#else
__RCSID("$NetBSD: ctrace.c,v 1.8 2000/04/16 01:16:43 thorpej Exp $");
#endif
#endif				/* not lint */

#ifdef DEBUG
#include <stdio.h>

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include "curses.h"

#ifndef TFILE
#define	TFILE	"__curses.out"
#endif

static FILE *tracefp;		/* Curses debugging file descriptor. */

void
#ifdef __STDC__
__CTRACE(const char *fmt,...)
#else
__CTRACE(fmt, va_alist)
	char   *fmt;
va_dcl
#endif
{
	va_list ap;
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	if (tracefp == NULL)
		tracefp = fopen(TFILE, "w");
	if (tracefp == NULL)
		return;
	(void) vfprintf(tracefp, fmt, ap);
	va_end(ap);
	(void) fflush(tracefp);
}
#else
/* this kills the empty translation unit message from lint... */
void __cursesi_make_lint_shut_up_if_debug_not_defined(void);
void
__cursesi_make_lint_shut_up_if_debug_not_defined(void)
{
	return;
}
#endif
