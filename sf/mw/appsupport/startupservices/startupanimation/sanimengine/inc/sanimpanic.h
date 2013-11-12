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
* Description:  Declaration of SAnimPanic class
*
*/


#ifndef SANIMPANIC_H
#define SANIMPANIC_H

#include <e32base.h>

/**
*  Static class for issuing panics.
*
*  @lib None
*  @since S60 3.2
*/
class SAnimPanic
    {

public:

    /** Available panic codes. */
    enum TPanicCode
        {
        EInternalError = 1,
        ENotInitialized = 2,
        };

    /**
    * Panic the current thread.
    *
    * @since S60 3.2
    *
    * @param aPanicCode Panic code to identify the reason for the panic.
    * @param aFileName Identifies the file in which the panic occurred.
    * @param aLineNum Identifies the line on which the panic occurred.
    */
    IMPORT_C static void Panic(
        const TPanicCode aPanicCode,
        const TDesC8& aFileName,
        const TInt aLineNum );

    };

#endif // SANIMPANIC_H
