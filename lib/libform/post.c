/*	$NetBSD: post.c,v 1.1 2000/12/17 12:04:30 blymn Exp $	*/

/*-
 * Copyright (c) 1998-2000 Brett Lymn
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
 *    derived from this software withough specific prior written permission
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

#include "form.h"
#include "internals.h"

/*
 * Post the form to the screen.
 */
int
post_form(FORM *form)
{
	int rows, cols, status;
	
	if ((form == NULL) || (form->win == NULL))
		return E_BAD_ARGUMENT;

	if (form->posted == 1)
		return E_POSTED;

	if ((form->fields == NULL) || (form->fields[0] == NULL))
		return E_NOT_CONNECTED;

	if (form->in_init == 1)
		return E_BAD_STATE;

	if (scale_form(form, &rows, &cols) != E_OK)
		return E_SYSTEM_ERROR;
	
	if ((form->subwin != NULL) && ((rows > getmaxy(form->subwin))
				       || (cols > getmaxx(form->subwin)))) {
		return E_NO_ROOM;
	}

	if (form->subwin == NULL) {
		form->subwin_created = 1;
		form->subwin = derwin(form->win, rows, cols, 0, 0);
		if (form->subwin == NULL)
			return E_SYSTEM_ERROR;

	}
	
	form->in_init = 1;
	if (form->form_init != NULL)
		form->form_init(form);

	if (form->field_init != NULL)
		form->field_init(form);
	form->in_init = 0;

	if ((status = _formi_draw_page(form)) != E_OK)
		return status;

	form->posted = 1;
	return E_OK;
}

/*
 * Unpost the form from the screen
 */
int
unpost_form(FORM *form)
{

	if (form == NULL)
		return E_BAD_ARGUMENT;

	if (form->posted != 1)
		return E_NOT_POSTED;

	if (form->in_init == 1)
		return E_BAD_STATE;

	form->in_init = 1;
	if (form->field_term != NULL)
		form->field_term(form);

	if (form->form_term != NULL)
		form->form_term(form);
	form->in_init = 0;

	wclear(form->subwin);
	if (form->subwin_created == 1) {
		delwin(form->subwin);
		form->subwin_created = 0;
	}

	form->posted = 0;

	return E_OK;
}

