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


/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Kevin Fall.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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

#if 0
#ifndef lint
static char const copyright[] =
"@@(#) Copyright (c) 1989, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */
#endif

#ifndef lint
#if 0
static char sccsid[] = "@@(#)cat.c	8.2 (Berkeley) 4/27/95";
#endif
#endif /* not lint */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/stat.h>
#ifndef NO_UDOM_SUPPORT
#include <sys/socket.h>
#ifndef __SYMBIAN32__
#include <sys/un.h>
#endif
#include <errno.h>
#endif

#include <ctype.h>
#include <err.h>
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>


int bflag, eflag, nflag, sflag, tflag, vflag;
static int rval;
const char *filename;

static void usage(void);
static void scanfiles(char *argv[], int cooked);
static void cook_cat(FILE *);
static void raw_cat(int);

#ifndef NO_UDOM_SUPPORT
static int udom_open(const char *path, int flags);
#endif

#ifdef __SYMBIAN32__
static
void reset_globals()
{
	bflag=eflag=nflag=sflag=tflag=vflag=0;
}
#endif

int
cat_internal(int argc, char *argv[])
{
	int ch;

	setlocale(LC_CTYPE, "");
	optind=1;
#ifdef __SYMBIAN32__
	reset_globals();	
#endif			
	while ((ch = getopt(argc, argv, "benstuv")) != -1)
		switch (ch) {
		case 'b':
			bflag = nflag = 1;	/* -b implies -n */
			break;
		case 'e':
			eflag = vflag = 1;	/* -e implies -v */
			break;
		case 'n':
			nflag = 1;
			break;
		case 's':
			sflag = 1;
			break;
		case 't':
			tflag = vflag = 1;	/* -t implies -v */
			break;
		case 'u':
			setbuf(stdout, NULL);
			break;
		case 'v':
			vflag = 1;
			break;
		default:
			usage();
			return 1;
		}
	
	argv += optind;
	
	if (bflag || eflag || nflag || sflag || tflag || vflag)
		scanfiles(argv, 1);
	else
		scanfiles(argv, 0);
#ifndef __SYMBIAN32__	
	if (fclose(stdout))
		err(1, "stdout");
#endif	
	return (rval);
	/* NOTREACHED */
}

static void
usage(void)
{
	fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
	return;
	/* NOTREACHED */
}

static void
scanfiles(char *argv[], int cooked)
{
	int i = 0;
	char *path;
	FILE *fp;
	struct stat sbuf;

	while ((path = argv[i]) != NULL || i == 0) {
		int fd;

		if (path == NULL || strcmp(path, "-") == 0) {
			filename = "stdin";
			fd = STDIN_FILENO;
		} else {
			filename = path;
#ifdef __SYMBIAN32__		
		if(stat(path, &sbuf)==0 && S_ISDIR(sbuf.st_mode))
			{
			errno=EISDIR;
			printf("cat: ");
			warn("%s", path);
			break;		
			}
#endif		
			fd = open(path, O_RDONLY);
#ifndef NO_UDOM_SUPPORT
#ifndef __SYMBIAN32__
			if (fd < 0 && errno == EOPNOTSUPP)
				fd = udom_open(path, O_RDONLY);
#endif			
#endif
		}
		if (fd < 0) {
			printf("cat: ");
			warn("%s", filename);
			rval = 1;
		} else if (cooked) {
			if (fd == STDIN_FILENO)
				cook_cat(stdin);
			else {
				fp = fdopen(fd, "r");
				cook_cat(fp);
				fclose(fp);
			}
		} else {
			raw_cat(fd);
			if (fd != STDIN_FILENO)
				close(fd);
		}
		if (path == NULL)
			break;
		++i;
	}
}

static void
cook_cat(FILE *fp)
{
	int ch, gobble, line, prev;

	/* Reset EOF condition on stdin. */
	if (fp == stdin && feof(stdin))
		clearerr(stdin);

	line = gobble = 0;
	for (prev = '\n'; (ch = getc(fp)) != EOF; prev = ch) {
		if (prev == '\n') {
			if (sflag) {
				if (ch == '\n') {
					if (gobble)
						continue;
					gobble = 1;
				} else
					gobble = 0;
			}
			if (nflag && (!bflag || ch != '\n')) {
				(void)fprintf(stdout, "%6d\t", ++line);
				if (ferror(stdout))
					break;
			}
		}
		if (ch == '\n') {
			if (eflag && putchar('$') == EOF)
				break;
		} else if (ch == '\t') {
			if (tflag) {
				if (putchar('^') == EOF || putchar('I') == EOF)
					break;
				continue;
			}
		} else if (vflag) {
			if (!isascii(ch) && !isprint(ch)) {
				if (putchar('M') == EOF || putchar('-') == EOF)
					break;
				ch = toascii(ch);
			}
			if (iscntrl(ch)) {
				if (putchar('^') == EOF ||
				    putchar(ch == '\177' ? '?' :
				    ch | 0100) == EOF)
					break;
				continue;
			}
		}
		if (putchar(ch) == EOF)
			break;
	}
	if (ferror(fp)) {
		printf("cat: ");
		warn("%s", filename);
		rval = 1;
		clearerr(fp);
	}
	if (ferror(stdout))
		{
		printf("cat: ");
		err(1, "stdout");
		}
}

static void
raw_cat(int rfd)
{
	int off, wfd;
	ssize_t nr, nw;
	static size_t bsize;
	static char *buf = NULL;
	struct stat sbuf;

	wfd = fileno(stdout);
	if (buf == NULL) {
		if (fstat(wfd, &sbuf))
			{
			printf("cat: ");			
			err(1, "%s", filename);
			return;
			}
		bsize = MAX(sbuf.st_blksize, 1024);
		if ((buf = malloc(bsize)) == NULL)
			{
			printf("cat: ");
			err(1, "buffer");
			return;
			}
	}
	while ((nr = read(rfd, buf, bsize)) > 0)
		for (off = 0; nr; nr -= nw, off += nw)
			if ((nw = write(wfd, buf + off, (size_t)nr)) < 0)
				{
				printf("cat: ");
				err(1, "stdout");
				return;					
				}
#ifdef __SYMBIAN32__
	write(wfd, "\n",1);
#endif
			
	if (nr < 0) {
		printf("cat: ");
		warn("%s", filename);
		rval = 1;
	}
}

#ifndef NO_UDOM_SUPPORT
#ifndef __SYMBIAN32__
static int
udom_open(const char *path, int flags)
{
	struct sockaddr_un sou;
	int fd;
	unsigned int len;

	bzero(&sou, sizeof(sou));

	/*
	 * Construct the unix domain socket address and attempt to connect
	 */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd >= 0) {
		sou.sun_family = AF_UNIX;
		if ((len = strlcpy(sou.sun_path, path,
		    sizeof(sou.sun_path))) >= sizeof(sou.sun_path)) {
			errno = ENAMETOOLONG;
			return (-1);
		}
		len = offsetof(struct sockaddr_un, sun_path[len+1]);

		if (connect(fd, (void *)&sou, len) < 0) {
			close(fd);
			fd = -1;
		}
	}

	/*
	 * handle the open flags by shutting down appropriate directions
	 */
	if (fd >= 0) {
		switch(flags & O_ACCMODE) {
		case O_RDONLY:
			if (shutdown(fd, SHUT_WR) == -1)
				warn(NULL);
			break;
		case O_WRONLY:
			if (shutdown(fd, SHUT_RD) == -1)
				warn(NULL);
			break;
		default:
			break;
		}
	}
	return(fd);
}

#endif
#endif //__SYMBIAN32__

