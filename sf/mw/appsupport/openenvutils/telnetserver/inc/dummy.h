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
// This file contains the declarations for unsupported functions. These functions
// should be revisited to check the feasibility of supporting them.  
// 
//
 
#ifdef __SYMBIAN32__
#ifndef dummy_h
#define dummy_h

#include "termios.h"
#include <unistd.h>

int cfsetispeed(struct termios *, speed_t);
int cfsetospeed(struct termios *, speed_t);
int	tcgetattr(int, struct termios *);
int	tcsetattr(int, int, const struct termios *);
void openlog(const char *path, int opt, int fac);
void closelog(void); 

#endif //dummy_h

#endif //__SYMBIAN32__