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
 * Copyright (c) 1995 - 2001 Kungliga Tekniska Hgskolan
 * (Royal Institute of Technology, Stockholm, Sweden).
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
__RCSID("$Heimdal: mini_inetd.c,v 1.30 2002/02/18 19:08:55 joda Exp $"
        "$NetBSD: mini_inetd.c,v 1.2 2002/09/20 21:48:58 mycroft Exp $");
#endif

#include <err.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/param.h>

#ifdef __SYMBIAN32__
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

#ifndef __SYMBIAN32__
#include "roken.h"
#endif

/*
 * accept a connection on `s' and pretend it's served by inetd.
 */

static void
accept_it (int s)
{
    int s2;
	socklen_t length;
	struct sockaddr sAddr;  	
    s2 = accept(s, &sAddr, &length);    
    
    close(s);
#ifdef __SYMBIAN32__    
   	if( s2 > 0 )    
		{  
#endif//__SYMBIAN32__
	    dup2(s2, STDIN_FILENO);
	    dup2(s2, STDOUT_FILENO);
	    dup2(s2, STDERR_FILENO); 
		
	    close(s2);
#ifdef __SYMBIAN32__    	    
		}
#endif//__SYMBIAN32__		
}

/*
 * Listen on a specified port, emulating inetd.
 */

void
mini_inetd_addrinfo (struct addrinfo *ai)
{
    struct addrinfo *a;
    int n, nalloc, i, ret;
    int *fds;
    int on=1;
#ifdef HAVE_POLL
    struct pollfd *set;
#else
    fd_set orig_read_set, read_set;
    int max_fd = -1;
#endif
  	
    for (nalloc = 0, a = ai; a != NULL; a = a->ai_next)
	++nalloc;
	
    fds = (int*)malloc (nalloc * sizeof(*fds));
  
#ifdef HAVE_POLL
    set = malloc (nalloc * sizeof(*set));
#else
    FD_ZERO(&orig_read_set);
#endif
	for (i = 0, a = ai; a != NULL; a = a->ai_next) {
	fds[i] = socket (a->ai_family, a->ai_socktype, a->ai_protocol);
	if (fds[i] < 0) {
		continue;
	}

	(void) setsockopt(fds[i], SOL_SOCKET, SO_REUSEADDR,
				(char *)&on, sizeof(on));

	if (bind (fds[i], a->ai_addr, a->ai_addrlen) < 0) {
		close(fds[i]);
	    continue;
	}
	if (listen (fds[i], SOMAXCONN) < 0) {
		close(fds[i]);
	    continue;
	}
#ifdef HAVE_POLL
	set[i].fd = fds[i];
	set[i].events = POLLIN;
#else
	FD_SET(fds[i], &orig_read_set);
	max_fd = MAX(max_fd, fds[i]);
#endif
	++i;
    }
    n = i;

    do {   
#ifdef HAVE_POLL
	ret = poll (set, nalloc, INFTIM);
#else
	read_set = orig_read_set;

	ret = select (max_fd + 1, &read_set, NULL, NULL, NULL);
#endif
    } while (ret <= 0);

    for (i = 0; i < n; ++i)
#ifdef HAVE_POLL
	if (set[i].revents & POLLIN)
#else
	if (FD_ISSET (fds[i], &read_set))
#endif
	{	
	    accept_it (fds[0]);
	    free (fds);
#ifdef HAVE_POLL
	    free (set);
#endif
	    return;
	}
    abort ();
}

void
mini_inetd (int port)
{
	int err;
    struct addrinfo *ai, hints;
    char portstr[NI_MAXSERV];

    memset (&hints, 0, sizeof(hints));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family   = PF_INET;

    snprintf (portstr, sizeof(portstr), "%d", ntohs(port));
    err=getaddrinfo (NULL, portstr, &hints, &ai);
    
    mini_inetd_addrinfo(ai);
    
    freeaddrinfo(ai);	
}
