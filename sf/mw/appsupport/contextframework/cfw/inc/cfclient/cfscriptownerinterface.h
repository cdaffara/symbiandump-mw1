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
* Description:  Script owner interface declaration.
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
 * Clients implementing this interface will get a notification if the scripts
 * registered by this client are removed by an update to the provider plug-in-
 * E.g. the operation plug-in which has dependency to the script is updated
 * or removed.
 *
 * @lib None.
 * @since S60 5.0
 */
class MCFScriptOwnerInterface
    {
public:

    /**
     * Notified when the scripts have been removed. CFServer will try to
     * re-activate the scripts but if the script cannot be found from the
     * file system, the client will get a notification.
     * @param aScriptIds Script ids which have been deregistered.
     */
    virtual void HandleScriptsRemovedL( const RArray<TInt>& aScriptIds ) = 0;
    };

#endif // CFSCRIPTOWNERINTERFACE_H

