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
#include <apgupdate.h>

#include "afapplicationscollection.h"
//------------------------------------------------------------------------------
CAfApplicationsCollection* CAfApplicationsCollection::NewL(RApaLsSession& serviceProvider,
                                                           MAfApplicationsObserver* observer)
{
    CAfApplicationsCollection *self = new(ELeave) CAfApplicationsCollection(serviceProvider, observer);
    return self;
}

//------------------------------------------------------------------------------
CAfApplicationsCollection::CAfApplicationsCollection(RApaLsSession& serviceProvider, MAfApplicationsObserver* observer)
:
    mServiceProvider(serviceProvider),
    mObserver(observer)
{}

//------------------------------------------------------------------------------
CAfApplicationsCollection::~CAfApplicationsCollection()
{
    mDeinstalledApplications.Close();
}

//------------------------------------------------------------------------------
void CAfApplicationsCollection::applicationsChanged()
{
    TRAP_IGNORE(collectsApplicationsInfoL());
    if (mObserver) {
        mObserver->applicationsChanged();
    }
}

//------------------------------------------------------------------------------
TArray<TUid> CAfApplicationsCollection::removedApplications()const
{
    return mDeinstalledApplications.Array();
}

//------------------------------------------------------------------------------
void CAfApplicationsCollection::setObserver(MAfApplicationsObserver& observer)
{
    mObserver = &observer;
}

//------------------------------------------------------------------------------
void CAfApplicationsCollection::collectsApplicationsInfoL()
{
    mDeinstalledApplications.Close();
    RArray< TApaAppUpdateInfo > updatedApps;
    CleanupClosePushL(updatedApps);
    TInt errNo = mServiceProvider.UpdatedAppsInfoL(updatedApps);
    User::LeaveIfError(errNo);
    for (TInt offset(0); offset < updatedApps.Count(); ++offset) {
        const TApaAppUpdateInfo::TApaAppAction action(updatedApps[offset].iAction);
        switch (action) {
        case TApaAppUpdateInfo::EAppNotPresent:
            mDeinstalledApplications.AppendL(updatedApps[offset].iAppUid);
            break;
        //TODO add implementation for other cases when needed
        }
    }
    CleanupStack::PopAndDestroy(&updatedApps);
}
