// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// dummy.c
// This file declares the stubs for unsupported functions. These functions
// should be revisited to check the feasibility of supporting them.  
// 
//
 
#ifndef dummy_h
#define dummy_h

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/signal.h>
#include "termios.h"
#include <unistd.h>
#include <setjmp.h>
#include "poll.h"
#include "times.h"

#define sigprocmask 	dummy_sigprocmask
#define kill			dummy_kill
#define sigemptyset		dummy_sigemptyset
#define sigaddset		dummy_sigaddset
#define sigfillset		dummy_sigfillset
#define sigdelset		dummy_sigdelset
#define sigsuspend		dummy_sigsuspend
#define alarm			dummy_alarm
#define signal			dummy_signal

int dummy_sigprocmask (int how, const sigset_t *a, sigset_t *b);
int setrlimit(int resource, const struct rlimit *rlp); 
extern int killpg(pid_t pgrp, int sig); 
int dummy_kill(pid_t pid, int sig); 
int getrlimit(int resource, struct rlimit *rlp);
extern int getrusage(int who, struct rusage *r_usage); 
extern unsigned dummy_alarm(unsigned seconds);
int dummy_sigemptyset(sigset_t *set);
int dummy_sigaddset(sigset_t *set, int signo);
int dummy_sigfillset(sigset_t *set);
int dummy_sigdelset(sigset_t *set, int signo);
int dummy_sigsuspend(const sigset_t *sigmask); 
int	tcgetattr(int, struct termios *);
int	tcsetattr(int, int, const struct termios *);
int tcsetpgrp(int fildes, pid_t pgid_id);
pid_t tcgetpgrp(int fildes);
int grantpt(int fildes);
int unlockpt(int fildes);
char *ptsname(int fildes);


char *mktemp(char *template);
void sync(void); 
speed_t cfgetospeed(const struct termios *termios_p);

int tputs (char *, int, int (*)(int));
int tgetent (char *, char *);
char * tgetstr (char *, char **);
int tgetflag (char *);
int tgetnum (char *);
char * tgoto (char *, int, int);
int tigetnum (char *);
int tigetflag (char *);
char * tigetstr (char *);
int putp (const char *);
char * tparm (char *, ...);
void dummy_signal(int, __sighandler_t*);


char *getlogin(void);
char *ttyname(int fildes);

#endif//dummy_h

