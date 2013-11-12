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
 * This file contains the stubs for unsupported functions. These functions
 * should be revisited to check the feasibility of supporting them.  
 */
#ifdef __SYMBIAN32__
#ifdef __WINSCW__
#pragma warn_unusedarg off
#endif//__WINSCW__

#include "dummy.h"

int cfsetispeed(struct termios *term, speed_t speed)
{
	return 0;
}

int cfsetospeed(struct termios *term, speed_t speed)
{
	return 0;
}

void openlog(const char *path, int opt, int fac)
{
	return;
}

void closelog(void)
{
	return;
}

int	tcgetattr(int i, struct termios * p)
{
	return 0;
}

int	tcsetattr(int i , int y, const struct termios *p)
{
	return 0;
}

#endif//__SYMBIAN32__





