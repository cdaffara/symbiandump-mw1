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
 * This file implements the stubs for unsupported functions. These functions
 * should be revisited to check the feasibility of supporting them.  
 */

#ifdef __SYMBIAN32__
#ifdef __WINSCW__
#pragma warn_unusedarg off
#endif//__WINSCW__
#include "dummy.h"

int dummy_sigprocmask (int how, const sigset_t *a, sigset_t *b)
	{
	return 0;
	}

int killpg(pid_t pgrp, int sig)
	{
	return 0;		
	}

int dummy_kill(pid_t pid, int sig)
	{
	return 0;
	}

int setrlimit(int resource, const struct rlimit *rlp)
	{
	return 0;
	}

int getrlimit(int resource, struct rlimit *rlp)
	{
	return 0;	
	}

int getrusage(int who, struct rusage *r_usage)
	{
	return 0;	
	}

unsigned dummy_alarm(unsigned seconds)
	{
	return 0;	
	}

int dummy_sigaddset(sigset_t *set, int signo)
	{
	return 0;
	}

int dummy_sigfillset(sigset_t *set)
	{
	return 0;	
	}
	
int dummy_sigdelset(sigset_t *set, int signo)
	{
	return 0;
	}

int dummy_sigsuspend(const sigset_t *sigmask)
	{
	return 0;
	}

int	tcgetattr(int i, struct termios * p)
	{
	return 0;
	}

int	tcsetattr(int i , int y, const struct termios *p)
	{
	return 0;
	}

int tcsetpgrp(int fildes, pid_t pgid_id)
	{
	return 0;	
	}

pid_t tcgetpgrp(int fildes)
	{
	return 0; //	shud return the foreground process grp id.
	}

int grantpt(int fildes)
	{
	return 0;	
	}
 
int unlockpt(int fildes)
	{
	return 0;
	}

char *ptsname(int fildes)
	{
	return NULL;
	}

char *mktemp(char *template)
	{
	return 0;
	}

void sync(void)
	{
	return;
	}

speed_t cfgetospeed(const struct termios *termios_p)
	{
	return (speed_t)0;
	}

int tputs (char * t, int x, int (*p)(int))
	{
	return 0;
	}

int tgetent (char *t, char *p)
	{
	return 0;
	}

char * tgetstr (char *t, char **p)
	{
	return (char*)0;
	}

int tgetflag (char *t)
	{
	return 0;
	}

int tgetnum (char *t)
	{
	return 0;
	}

char * tgoto (char *x, int y, int z)
	{
	return 0;
	}

int tigetnum (char *p)
	{
	return 0;
	}

int tigetflag (char *p)
	{
	return 0;
	}

char * tigetstr (char *p)
	{
	return 0;	
	}

int putp (const char *p)
	{
	return 0;
	}

char * tparm (char *p, ...)
	{
	return 0;
	}
	
char *getlogin(void)
	{
	return NULL;
	}

char *ttyname(int fildes)
	{
	return NULL;
	}

int dummy_sigemptyset(sigset_t *set)
	{
	return 0;
	}

void dummy_signal(int sig, __sighandler_t* ptr)
	{
	return;
	}

#endif//__SYMBIAN32__

