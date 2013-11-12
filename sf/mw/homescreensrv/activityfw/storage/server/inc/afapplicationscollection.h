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
#ifndef AFAPPLICATIONSCOLLECTION_H
#define AFAPPLICATIONSCOLLECTION_H

#include <e32base.h>
#include "afapplicationsobserver.h"
#include "afapplicationsregistry.h"


class CAfApplicationsCollection: public CBase,
                                 public MAfApplicationsObserver,
                                 public MAfApplicationsRegistry
{
public:
    static CAfApplicationsCollection* NewL(RApaLsSession& serviceProvider, MAfApplicationsObserver* observer =0);
    ~CAfApplicationsCollection();
    void applicationsChanged();
    TArray<TUid> removedApplications()const;
    void setObserver(MAfApplicationsObserver& oserver);
private:
    CAfApplicationsCollection(RApaLsSession& serviceProvider, MAfApplicationsObserver* observer);
    void collectsApplicationsInfoL();
    
private:
    RApaLsSession& mServiceProvider;
    MAfApplicationsObserver* mObserver;
    RArray<TUid> mDeinstalledApplications;
};

#endif // AFAPPLICATIONSCOLLECTION_H
