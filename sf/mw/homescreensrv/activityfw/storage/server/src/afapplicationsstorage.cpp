/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include "afapplicationsstorage.h"
//------------------------------------------------------------------------------
CAfApplicationsStorage* CAfApplicationsStorage::NewL(CAfStorage& storage, 
                                                     const MAfApplicationsRegistry& provider, 
                                                     MAfApplicationsObserver &observer)
{
    CAfApplicationsStorage *self = new (ELeave)CAfApplicationsStorage(storage, provider, observer);
    return self;
    
}

//------------------------------------------------------------------------------
CAfApplicationsStorage::CAfApplicationsStorage(CAfStorage& storage, 
                                               const MAfApplicationsRegistry& provider, 
                                               MAfApplicationsObserver &observer)
:
    mStorage(storage),
    mProvider(provider),
    mObserver(observer)
{
}

//------------------------------------------------------------------------------
CAfApplicationsStorage::~CAfApplicationsStorage()
{}

//------------------------------------------------------------------------------
void CAfApplicationsStorage::applicationsChanged()
{
    const TArray<TUid> removedApp(mProvider.removedApplications());
    for (TInt iter(0); iter < removedApp.Count(); ++iter) {
        TRAP_IGNORE(deleteActivityL(removedApp[iter]));
    }

    if ( removedApp.Count() > 0 ) {
        mObserver.applicationsChanged();
    }
}

//------------------------------------------------------------------------------
void CAfApplicationsStorage::deleteActivityL(TUid appId)
{
    CAfEntry *entry(CAfEntry::NewLC(0, 
                                    static_cast<TInt>(appId.iUid),
                                    KNullDesC,
                                    KNullDesC,
                                    KNullDesC,
                                    KNullDesC8,
                                    KNullDesC8,
                                    TTime()));
    mStorage.DeleteActivitiesL(*entry);
    CleanupStack::PopAndDestroy(entry);
}
