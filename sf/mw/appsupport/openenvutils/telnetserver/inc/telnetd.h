//telnetd.h
//
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
 *
 *	@(#)telnetd.h	8.1 (Berkeley) 6/4/93
 */

#ifndef __SYMBIAN32__
#include <config.h> 
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <fcntl.h>
//#include <sys/file.h>
#include <sys/stat.h>


/* including both <sys/ioctl.h> and <termios.h> in SunOS 4 generates a
   lot of warnings */

#if defined(HAVE_SYS_IOCTL_H) && SunOS != 40
#include <sys/ioctl.h>
#endif

#include <sys/filio.h>
#include <stdapis/netinet/in.h>
#include <stdapis/arpa/inet.h>
#include <signal.h>
#include <errno.h>

#include <netdb.h>
#include "syslog.h"
#include <ctype.h>
#include <unistd.h>
#include "termios.h" 


//#include <pty.h>
#include "defs.h"

#ifndef _POSIX_VDISABLE
# ifdef VDISABLE
#  define _POSIX_VDISABLE VDISABLE
# else
#  define _POSIX_VDISABLE ((unsigned char)'\377')
# endif
#endif



//#include <sys/pty.h>
#include <sys/select.h>
//#include <sys/ptyio.h>
#include <sys/utsname.h>
#include <paths.h>
#include "telnet.h"
#include "ext.h"

#ifdef SOCKS
#include <socks.h>
/* This doesn't belong here. */
struct tm *localtime(const time_t *);
struct hostent  *gethostbyname(const char *);
#endif

#ifdef KRB4
#include <krb.h>
#endif

#ifdef AUTHENTICATION
#include <libtelnet/auth.h>
#include <libtelnet/misc.h>
#ifdef ENCRYPTION
#include <libtelnet/encrypt.h>
#endif
#endif

#ifdef HAVE_LIBUTIL_H
#include <libutil.h>
#endif

#ifndef __SYMBIAN32__
#include <roken.h>
#endif

/* Don't use the system login, use our version instead */

/* BINDIR should be defined somewhere else... */

#ifndef BINDIR
#define BINDIR "/usr/athena/bin"
#endif

#undef _PATH_LOGIN
#define _PATH_LOGIN	BINDIR "/login"

/* fallbacks */

#ifndef _PATH_DEV
#define _PATH_DEV "/dev/"
#endif

#ifndef _PATH_TTY
#define _PATH_TTY "/dev/tty"
#endif /* _PATH_TTY */

#ifdef	DIAGNOSTICS
#define	DIAG(a,b)	if (diagnostic & (a)) b
#else
#define	DIAG(a,b)
#endif

/* other external variables */
#ifndef __SYMBIAN32__
extern	char **environ;
#endif

/* prototypes */

/* appends data to nfrontp and advances */
int output_data (const char *format, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;
