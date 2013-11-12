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
* Description:  Script owner interface.
 *
*/


#ifndef CFSCRIPTOWNER_H
#define CFSCRIPTOWNER_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Script owner interface.
 *
 * @lib -
 * @since S60 5.0
 */
class MCFScriptOwner
    {
public:

    /**
     * Informs script owner that some scripts have been deregistered
     * due a change in operation provider (uninstall or upgrade).
     * @param aScriptIds Script ids which have been deregistered but not
     *      automatically loaded.
     */
    virtual void HandleScriptsRemoved( const RArray<TInt>& aScriptIds ) = 0;
    };

#endif // CFSCRIPTOWNER_H

