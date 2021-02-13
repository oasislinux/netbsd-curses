/*	$NetBSD: slave.c,v 1.13 2021/02/13 08:17:15 rillig Exp $	*/

/*-
 * Copyright 2009 Brett Lymn <blymn@NetBSD.org>
 * Copyright 2021 Roland Illig <rillig@NetBSD.org>
 *
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
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "returns.h"
#include "slave.h"

int from_director;
int to_director;
int initdone = 0;

#if 0
static const char *returns_enum_names[] = {
	"unused", "numeric", "string", "byte", "ERR", "OK", "NULL", "not NULL",
	"variable"
};
#endif

/*
 * Read the command pipe for the function to execute, gather the args
 * and then process the command.
 */
static void
process_commands(void)
{
	int len, maxlen, argslen, i, ret, type;
	char *cmdbuf, *tmpbuf, **args, **tmpargs;

	len = maxlen = 30;
	if ((cmdbuf = malloc(maxlen)) == NULL)
		err(1, "slave cmdbuf malloc failed");

	for (;;) {
		if (read(from_director, &type, sizeof(int)) < 0)
			err(1, "slave command type read failed");

		if (type != data_string)
			errx(1, "Unexpected type for command, got %d", type);

		if (read(from_director, &len, sizeof(int)) < 0)
			err(1, "slave command len read failed");

		if ((len + 1) > maxlen) {
			maxlen = len + 1;
			if ((tmpbuf = realloc(cmdbuf, maxlen)) == NULL)
				err(1, "slave cmdbuf realloc to %d "
				    "bytes failed", maxlen);
			cmdbuf = tmpbuf;
		}

		if (read(from_director, cmdbuf, len) < 0)
			err(1, "slave command read failed");
		cmdbuf[len] = '\0';
		argslen = 0;
		args = NULL;

		do {
			if (read(from_director, &type, sizeof(int)) < 0)
				err(1, "slave arg type read failed");

			if (read(from_director, &len, sizeof(int)) < 0)
				err(1, "slave arg len read failed");

			if (len >= 0) {
				tmpargs = realloc(args,
				    (argslen + 1) * sizeof(char *));
				if (tmpargs == NULL)
					err(1, "slave realloc of args array "
					    "failed");

				args = tmpargs;
				if (type != data_null) {
					args[argslen] = malloc(len + 1);

					if (args[argslen] == NULL)
						err(1, "slave alloc of %d bytes"
						    " for args failed", len);
				}

				if (len == 0) {
					if (type == data_null)
						args[argslen] = NULL;
					else
						args[argslen][0] = '\0';
				} else {
					read(from_director, args[argslen],
					     len);
					if (type != data_byte)
						args[argslen][len] = '\0';

					if (len == 6) {
						if (strcmp(args[argslen],
							   "STDSCR") == 0) {
							ret = asprintf(&tmpbuf,
								 "%p",
								 stdscr);
							if (ret < 0)
								err(2,
								    "asprintf of stdscr failed");
							free(args[argslen]);
							args[argslen] = tmpbuf;
						}
					}
				}

				argslen++;
			}
		} while (len >= 0);

		command_execute(cmdbuf, argslen, args);

		if (args != NULL) {
			for (i = 0; i < argslen; i++)
				free(args[i]);

			free(args);
		}
	}
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage: %s <in_fd> <out_fd>\n", getprogname());
		return 0;
	}
	sscanf(argv[1], "%d", &from_director);
	sscanf(argv[2], "%d", &to_director);

	process_commands();

	return 0;
}
