
/*	$NetBSD: main.c,v 1.26 2006/11/09 20:50:53 christos Exp $	*/

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



/*- © 
 * Copyright (c) 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Cimarron D. Taylor of the University of California, Berkeley.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
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
#include <sys/select.h>
#ifndef lint
#if 0
static char sccsid[] = "@(#)main.c	8.4 (Berkeley) 5/4/95";
#else
__COPYRIGHT("@(#) Copyright (c) 1990, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n");
__RCSID("$NetBSD: main.c,v 1.26 2006/11/09 20:50:53 christos Exp $");
#endif
#endif /* not lint */

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include "fts.h"
#include <signal.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "find.h"

time_t now;			/* time find was run */
int dotfd;			/* starting directory */
int ftsoptions;			/* options for the ftsopen(3) call */
int isdeprecated;		/* using deprecated syntax */
int isdepth;			/* do directories on post-order visit */
int isoutput;			/* user specified output operator */
int issort;			/* sort directory entries */
int isxargs;			/* don't permit xargs delimiting chars */
char *path;

static void usage(void);

char* get_value(char* var, char** env_list);

int main(int argc, char *argv[], char** env_list)
{


//	struct sigaction sa;
	char **p, **start;
	int ch;

	(void)time(&now);	/* initialize the time-of-day */
	(void)setlocale(LC_ALL, "");

/*	memset(&sa, 0, sizeof(sa));
*	sa.sa_flags = SA_RESTART;
*	sa.sa_handler = show_path;
*	sigaction(SIGINFO, &sa, NULL);
*/

	
/* array to hold dir list.  at most (argc - 1) elements. */
	p = start = malloc(argc * sizeof (char *));
	if (p == NULL)
		printf(NULL);

	ftsoptions = FTS_NOSTAT | FTS_PHYSICAL;
	while ((ch = getopt(argc, argv, "HLPXdf:hsx")) != -1)
		switch (ch) {
		case 'H':
			ftsoptions &= ~FTS_LOGICAL;
			ftsoptions |= FTS_PHYSICAL|FTS_COMFOLLOW;
			break;
		case 'L':
			ftsoptions &= ~(FTS_COMFOLLOW|FTS_PHYSICAL);
			ftsoptions |= FTS_LOGICAL;
			break;
		case 'P':
			ftsoptions &= ~(FTS_COMFOLLOW|FTS_LOGICAL);
			ftsoptions |= FTS_PHYSICAL;
			break;
		case 'X':
			isxargs = 1;		//XARGS NA YET.
			break;
		case 'd':
			isdepth = 1;
			break;
		case 'f':
			*p++ = optarg;			
			break;
		case 'h':
			ftsoptions &= ~FTS_PHYSICAL;
			ftsoptions |= FTS_LOGICAL;
			break;
		case 's':
			issort = 1;
			break;
		case 'x':
			ftsoptions |= FTS_XDEV;
			break;
		case '?':
		default:
			break;
		}

	argc -= optind;
	argv += optind;

	/*
	 * Find first option to delimit the file list.  The first argument
	 * that starts with a -, or is a ! or a ( must be interpreted as a
	 * part of the find expression, according to POSIX .2.
	 */
	for (; *argv != NULL; *p++ = *argv++) {
		if (argv[0][0] == '-')
			break;
		if ((argv[0][0] == '!' || argv[0][0] == '(') &&
		    argv[0][1] == '\0')
			break;
	}

	if (p == start)
		usage();
	else
	#ifdef __SYMBIAN32__  //  changing directory to the working dir
		path=get_value("PWD", env_list); 
		if(path) {
			if (chdir(path) == -1) {
				printf("%s: %s\n", path, strerror(errno));
			}
		}
	#endif
		

	*p = NULL;

	#ifdef __SYMBIAN32__
	
	if ((dotfd =open (".", O_RDONLY,0)) == -1 )
		errx(1,".");
	#else

	if ((dotfd = open(".", O_RDONLY, 0)) == -1 ||
	    fcntl(dotfd, F_SETFD, FD_CLOEXEC) == -1)
		errx(1,".");
	#endif

exit(find_execute(find_formplan(argv), start));
}

static void
usage(void)
{
	printf("Wrong Usage\n");
	(void)printf("usage: find  [-d s] file [file ...] [expression]\n");
	exit(1);
}

#ifdef __SYMBIAN32__  
//get the value for the given var from the env list...
char* get_value(char* var, char** env_list)
{
	char* pwd = NULL;
	size_t idx=0;
	
	if(!var || !env_list)
		return NULL;
	
	while(env_list[idx])
		{
		char* temp=env_list[idx];
		if(strstr(temp, var))
			{
			while(*temp!= '='&& temp++) {};			
			if(temp++)
				pwd=temp;
			break;
			}
		idx++;	
		}	
	return pwd; 	
}	
#endif //__SYMBIAN32__
