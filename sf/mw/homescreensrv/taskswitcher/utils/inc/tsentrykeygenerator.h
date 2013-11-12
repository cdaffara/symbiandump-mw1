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


#ifndef TSENTRYKEYGENERATOR_H
#define TSENTRYKEYGENERATOR_H

#include <e32base.h>
#include <e32cmn.h>
#include <w32std.h>

#include "tsentrykey.h"
#include "tswindowgroupsobserver.h"


class TsEntryKeyGeneraror
{
public:
    static TInt Generate( TTsEntryKey& aReturnKey, 
                          TInt aWindowGroupId, 
                          const MTsRunningApplicationStorage& aStorage );
    static TInt Generate(TTsEntryKey& returnKey, 
                TInt windowGroupId, 
                const TArray<RWsSession::TWindowGroupChainInfo>& groupChain);
};

#endif //TSENTRYKEYGENERATOR_H
