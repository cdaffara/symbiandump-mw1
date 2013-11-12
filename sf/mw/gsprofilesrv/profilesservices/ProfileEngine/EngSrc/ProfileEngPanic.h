/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Panic the current thread.
*
*/



#ifndef __PROFILEENGPANIC_H__
#define __PROFILEENGPANIC_H__

//  INCLUDES
#include <e32std.h>

// DATA TYPES
enum TProfileEngPanics
    {
    EProfileEngPanicNullPointer     = 1,    // Null pointer exception
    EProfileEngPanicBadParameter    = 2     // Incorrect parameter
    };

// CLASS DECLARATION

/**
*  This class used to panic the current thread.
*
*  @lib ?library
*  @since 2.0
*/
class ProfileEngPanic
    {
    public: // New functions

        /**
        * Panics the current thread,
        * specifying a category name and panic number.
        * @since 2.0
        * @param aReason The panic number.
        */
        static void Panic( TInt aReason );
    };

#endif      //  __PROFILEENGPANIC_H__

// End of File
