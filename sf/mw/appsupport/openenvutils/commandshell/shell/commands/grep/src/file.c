/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


/*	$NetBSD: file.c,v 1.2 2006/05/15 21:12:21 rillig Exp $	*/

/*-
 * Copyright (c) 1999 James Howard and Dag-Erling Codan Smrgrav
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/cdefs.h>
#ifndef lint
__RCSID("$NetBSD: file.c,v 1.2 2006/05/15 21:12:21 rillig Exp $");
#endif /* not lint */
#define FILE_C
#include "grep.h"


#define FILE_STDIO	0
#define FILE_MMAP	1
#define FILE_GZIP	2
#define ZEXTERN extern
struct file {
	int type;
	FILE *f;
	mmf_t *mmf;
	#ifndef __SYMBIAN32__
	gzFile *gzf;
	#endif
};
char fname[MAXPATHLEN];
 char *lnbuf;
 size_t lnbuflen;

/* Get a line from the file*/
extern char *fgetln(FILE *f, size_t *len)
{
	size_t n;
	int c;
	for (n = 0; ; ++n) {
		c = getc(f);
		if ((c == EOF))  {
			
			if (feof(f))
				break;
			err(2, "%s", fname);
			/* ERROR */
		
		}
		if (c == line_endchar)
			break;
		if (n >= lnbuflen) {
			lnbuflen *= 2;
			lnbuf = (char *)grep_realloc(lnbuf, ++lnbuflen);
		}
		if(c=='\r')
		{
			n--;	
		}
		else
		lnbuf[n] = c;	
	}
	if (feof(f) && n == 0)
		return NULL;
	*len = n;
	return lnbuf;
}

/* Get aline of data from the file*/
static char *
grepfgetln(FILE *f, size_t *len)
{
	size_t n;
	int c;
	
	for (n = 0; ; ++n) {
		c = getc(f);
		if (c == EOF) {
			if (feof(f))
				break;
			err(2, "%s", fname);
			/* ERROR */
		
		}
		if (c == line_endchar)
			break;
		if (n >= lnbuflen) {
			lnbuflen *= 2;
			lnbuf = (char *)grep_realloc(lnbuf, ++lnbuflen);
		}
		lnbuf[n] = c;
	}
	if (feof(f) && n == 0)
		return NULL;
	*len = n;
	return lnbuf;
}
#ifndef __SYMBIAN32__
/* Get a line from the gz file*/
static char *
gzfgetln(gzFile *f, size_t *len)
{
	size_t n;
	int c;

	for (n = 0; ; ++n) {
		c = gzgetc(f);
		if (c == -1) {
			const char *gzerrstr;
			int gzerr;

			if (gzeof(f))
				break;

			gzerrstr = gzerror(f, &gzerr);
			if (gzerr == Z_ERRNO)
				err(2, "%s", fname);
			else
				errx(2, "%s: %s", fname, gzerrstr);
		}
		if (c == line_endchar)
			break;
		if (n >= lnbuflen) {
			lnbuflen *= 2;
			lnbuf = (char *)grep_realloc(lnbuf, ++lnbuflen);
		}
		lnbuf[n] = c;
	}

	if (gzeof(f) && n == 0)
		return NULL;
	*len = n;
	return lnbuf;
}
#endif
/* Open the file in read mode*/
file_t *
grep_fdopen(int fd, const char *mode)
{
	file_t *f;

	if (fd == 0)
		sprintf(fname, "(standard input)");
	else
		sprintf(fname, "(fd %d)", fd);

	f = (struct file *)grep_malloc(sizeof *f);
	#ifndef __SYMBIAN32__
	if (zgrep) {
		f->type = FILE_GZIP;
		if ((f->gzf = gzdopen(fd, mode)) != NULL)
			return f;
	} else 
	#endif
	{
		f->type = FILE_STDIO;
		if ((f->f = fdopen(fd, mode)) != NULL)
			return f;
	}

	free(f);
	return NULL;
}

/* Open the file in read mode depending on the type*/
file_t *
grep_open(const char *path, const char *mode)
{
	file_t *f;

	snprintf(fname, MAXPATHLEN, "%s", path);

	f = (struct file *)grep_malloc(sizeof *f);
	#ifndef __SYMBIAN32__
	if (zgrep) {
		f->type = FILE_GZIP;
		if ((f->gzf = gzopen(fname, mode)) != NULL)
			return f;
	
	}
	else 
	#endif
	{ 
				/* try mmap first; if it fails, try stdio */
		
		if ((f->mmf = mmopen(fname, mode)) != NULL) {
			f->type = FILE_MMAP;
			return f;
	
		}
			f->type = FILE_STDIO;
			if ((f->f = fopen(path, mode)) != NULL)
				return f;	
	}

	free(f);
	return NULL;
}

/* Check if the file is binary*/
int
grep_bin_file(file_t *f)
{
	switch (f->type) {
	case FILE_STDIO:
		return bin_file(f->f);
	case FILE_MMAP:
		return mmbin_file(f->mmf); 
		#ifndef __SYMBIAN32__
	case FILE_GZIP:
		return gzbin_file(f->gzf);
	#endif
	default:
		/* can't happen */
		errx(2, "invalid file type");
	}
	return -1;
}

/*Get a line from the file */
char *
grep_fgetln(file_t *f, size_t *l)
{
	switch (f->type) {
	case FILE_STDIO:
		if (line_endchar == '\n') 
			return fgetln(f->f, l);
		else
			return grepfgetln(f->f, l);
	case FILE_MMAP:
		return mmfgetln(f->mmf, l);
		#ifndef __SYMBIAN32__
	case FILE_GZIP:
		return gzfgetln(f->gzf, l);
	#endif
	default:
		/* can't happen */
		errx(2, "invalid file type");
	}
	return NULL;
}

/* Close the file depending on the type*/
void
grep_close(file_t *f)
{
	switch (f->type) {
	case FILE_STDIO:
		fclose(f->f);
		break;
	case FILE_MMAP:
		mmclose(f->mmf);
		break;
		#ifndef __SYMBIAN32__
	case FILE_GZIP:
		gzclose(f->gzf);
		break;
		#endif
	default:
		/* can't happen */
		errx(2, "invalid file type");
	}
}
