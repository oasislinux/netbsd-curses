/*	$NetBSD: pause.c,v 1.11 2021/09/06 07:03:50 rin Exp $	*/

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Julian Coleman.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#include "curses.h"
#include "curses_private.h"

/*
 * napms --
 *	Sleep for ms milliseconds.
 */
int
napms(int ms)
{
	struct timespec	ts;

	__CTRACE(__CTRACE_MISC, "napms: %d\n", ms);
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	(void)nanosleep(&ts, NULL);
	return OK;
}

/*
 * delay_output --
 *	Pause output using terminal pad character.
 */
int
delay_output(int ms)
{
	char delstr[(sizeof(int) * CHAR_BIT + 2) / 3 + 1];
	int ret;

	__CTRACE(__CTRACE_MISC, "delay_output: %d\n", ms);
	if (!_cursesi_screen->padchar)
		return napms(ms);

	ret = snprintf(delstr, sizeof(delstr), "%d", ms);
	if (ret < 0 || ret >= sizeof(delstr))
		return ERR;
	tputs(delstr, 0, __cputchar);
	return OK;
}
