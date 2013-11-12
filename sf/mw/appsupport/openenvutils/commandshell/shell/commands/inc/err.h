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

/*-
 * © 
 * Copyright (c) 1993
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

#ifndef __ERR_H__
#define  __ERR_H__

#include <sys/cdefs.h>
#include <sys/_types.h>

#ifdef __SYMBIAN32__
#include <stdarg.h>
#endif //__SYMBIAN32__

void	err(int, const char *, ...) __dead2 __printf0like(2, 3);
void	verr(int, const char *, va_list) __dead2 __printf0like(2, 0);
void	errc(int, int, const char *, ...) __dead2 __printf0like(3, 4);
void	verrc(int, int, const char *, va_list) __dead2
	    __printf0like(3, 0);
void	errx(int, const char *, ...) __dead2 __printf0like(2, 3);
void	verrx(int, const char *, va_list) __dead2 __printf0like(2, 0);
void	warn(const char *, ...) __printf0like(1, 2);
void	vwarn(const char *, va_list) __printf0like(1, 0);
void	warnc(int, const char *, ...) __printf0like(2, 3);
void	vwarnc(int, const char *, va_list) __printf0like(2, 0);
void	warnx(const char *, ...) __printflike(1, 2);
void	vwarnx(const char *, va_list) __printflike(1, 0);
void	err_set_file(void *);

#endif //__ERR_H__
