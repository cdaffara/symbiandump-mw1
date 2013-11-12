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
* Description:  Implementation Panic function for all Startup Animation
*                components to use.
*
*/


#include <e32debug.h>

#include "sanimpanic.h"

_LIT( KSAnimPanicCategory, "STARTUPANIMATION" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// SAnimPanic::Panic
//
// ---------------------------------------------------------------------------
//
EXPORT_C void SAnimPanic::Panic(
    const TPanicCode aPanicCode,
    const TDesC8& aFileName,
    const TInt aLineNum )
    {
    TPath name;
    name.Copy( aFileName );
    RDebug::Print( _L( "Assertion failed in file=%S, line=%d" ), &name, aLineNum );

    User::Panic( KSAnimPanicCategory, aPanicCode );
    }
