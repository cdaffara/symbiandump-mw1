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
#ifndef AFAPPLICATIONSSTORAGE_H
#define AFAPPLICATIONSSTORAGE_H

#include <e32base.h>
#include "afstorage.h"
#include "afapplicationsobserver.h"
#include "afapplicationsregistry.h"


class CAfApplicationsStorage: public CBase,
                               public MAfApplicationsObserver
{
public:
    static CAfApplicationsStorage* NewL(CAfStorage& storage, const MAfApplicationsRegistry& provider, MAfApplicationsObserver &observer);
    ~CAfApplicationsStorage();
    void applicationsChanged();
    
private:
    CAfApplicationsStorage(CAfStorage& storage, const MAfApplicationsRegistry& provider, MAfApplicationsObserver &observer);
    void deleteActivityL(TUid appId);
private:
    CAfStorage& mStorage;
    const MAfApplicationsRegistry &mProvider;
    MAfApplicationsObserver &mObserver;
};

#endif // AFAPPLICATIONSSTORAGE_H
