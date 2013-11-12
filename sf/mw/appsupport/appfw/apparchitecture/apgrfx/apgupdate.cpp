// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apgupdate.cpp
//

#include "apgupdate.h"

/**
 * Default Constructor.
 */
EXPORT_C TApaAppUpdateInfo::TApaAppUpdateInfo()
    {
    
    }

/**
 * Constructor for TApaAppUpdateInfo.
 * @param aAppUid Application UID.
 * @param aAction Action performed by installer on the application.
 */
EXPORT_C TApaAppUpdateInfo::TApaAppUpdateInfo(TUid aAppUid, TApaAppUpdateInfo::TApaAppAction aAction):
        iAppUid(aAppUid),
        iAction(aAction)
    {
    
    }

EXPORT_C void TApaAppUpdateInfo::InternalizeL(RReadStream& aReadStream) 
    {
        iAppUid.iUid=aReadStream.ReadInt32L();
        iAction=TApaAppAction(aReadStream.ReadInt32L());
    }

EXPORT_C void TApaAppUpdateInfo::ExternalizeL(RWriteStream& aWriteStream) const
        {
        aWriteStream.WriteInt32L(iAppUid.iUid);
        aWriteStream.WriteInt32L(iAction);
        }

