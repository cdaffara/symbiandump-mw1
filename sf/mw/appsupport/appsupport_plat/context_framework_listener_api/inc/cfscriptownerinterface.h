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


#ifndef CFSCRIPTOWNERINTERFACE_H
#define CFSCRIPTOWNERINTERFACE_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// DATA TYPES

const TInt KCFScriptOwnerInterfaceUidValue = 0x200123EA;
const TUid KCFScriptOwnerInterfaceUid = {KCFScriptOwnerInterfaceUidValue};

// CLASS DECLARATION

/**
 * Script owner interface.
 * Client implementing this interface will receive a notification
 * if scripts are removed due to a change in the operation provider.
 * The change can be initiated if the operation provider plug-in is upgraded
 * or uninstalled. Client will need to register the removed scripts again.
 *
 * @lib -
 * @since S60 5.0
 */
class MCFScriptOwnerInterface
    {
public:

    /**
     * Informs script owner that some scripts have been deregistered
     * due a change in operation provider (uninstall or upgrade).
     * @since S60 3.2
     * @param aScriptIds Script ids which have been deregistered but not
     *      automatically loaded.
     * @return None.
     */
    virtual void HandleScriptsRemovedL( const RArray<TInt>& aScriptIds ) = 0;
    };

#endif // CFSCRIPTOWNERINTERFACE_H

