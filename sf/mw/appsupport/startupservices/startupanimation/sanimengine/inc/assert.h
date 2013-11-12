/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Assert macros
*
*/


#ifndef ASSERT_H
#define ASSERT_H

#include "sanimpanic.h"

#define ASSERT_ALWAYS_TRACE(_p) SAnimPanic::Panic(_p,_L8(__FILE__),__LINE__);
#define ASSERT_TRACE(_s,_p) if(!(_s)) { ASSERT_ALWAYS_TRACE(_p); }

#endif // ASSERT_H
