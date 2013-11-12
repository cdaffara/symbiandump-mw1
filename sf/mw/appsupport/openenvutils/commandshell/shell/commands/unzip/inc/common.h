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
* Author	 : Sandeep G Nair

* Name		 : Common.h

*/







#ifndef _COMMON_H_
#define _COMMON_H_

typedef unsigned long uLong;
typedef unsigned int  uInt;


#define unix

#undef WIN32

#define NOUNCRYPT
#define NOCRYPT

#define ZEXPORT
//#define _TESTING_NO_ZSH_  // testing without ZSH.


#ifndef ALLOC
# define ALLOC(size) (malloc(size))
#endif
#ifndef TRYFREE
# define TRYFREE(p) {if (p) free(p);}
#endif

#endif //_COMMON_H_

