/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Task list entry
 *
*/
#include "tsentrykeygenerator.h"
#include "tsrunningapp.h"
#include "tsrunningappstorage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt TsEntryKeyGeneraror::Generate( TTsEntryKey& aReturnKey, 
                                    TInt aWindowGroupId, 
                                    const MTsRunningApplicationStorage& aStorage)
{
    for (TInt iter(0); iter < aStorage.Count(); ++iter) {
        if (aStorage[iter].WindowGroupId() == aWindowGroupId) {
            aReturnKey = TTsEntryKey( aWindowGroupId );
            if (aStorage[iter].WindowGroupId() == aStorage[iter].ParentWindowGroupId()) 
                {
                return KErrBadHandle;
                }
            else if ( aStorage[iter].IsEmbeded() ) 
                {
                return TsEntryKeyGeneraror::Generate(aReturnKey, aStorage[iter].ParentWindowGroupId(), aStorage);
                
                }
            else 
                {
                return KErrNone;
                }
        }
    }
    return KErrNotFound;
}

// -----------------------------------------------------------------------------
TInt TsEntryKeyGeneraror::Generate(TTsEntryKey& returnKey, TInt windowGroupId, 
    const TArray<RWsSession::TWindowGroupChainInfo>& groupChain)
{
    for (TInt iter(0); iter < groupChain.Count(); ++iter) {
        if (groupChain[iter].iId == windowGroupId) {
            returnKey = TTsEntryKey(windowGroupId);
            if (groupChain[iter].iId == groupChain[iter].iParentId) {
                return KErrBadHandle;
            }
            else if (0 >= groupChain[iter].iParentId) {
                return KErrNone;
            }
            else {
                return TsEntryKeyGeneraror::Generate(returnKey, groupChain[iter].iParentId, groupChain);
            }
        }
    }
    return KErrNotFound;
}
