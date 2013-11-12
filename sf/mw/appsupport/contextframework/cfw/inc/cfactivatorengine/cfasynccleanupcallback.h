/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFAsyncCleanupCallback interface.
 *
*/


#ifndef CFASYNCCLEANUPCALLBACK_H
#define CFASYNCCLEANUPCALLBACK_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFAsyncCleanup;

// DATA TYPES

// CLASS DECLARATION

/**
 * Async cleanup callback notification interface.
 *
 * @lib None.
 * @since S60 5.0
 */
class MCFAsyncCleanupCallback
    {
public:

    /**
     * Async cleanup notifies that is has completed the clean up.
     * @param aCleanup Cleanup operation which has been completed.
     * @return None.
     */
    virtual void CleanupCompleted( CCFAsyncCleanup* aCleanup ) = 0;
    };

#endif // CFASYNCCLEANUPCALLBACK_H
