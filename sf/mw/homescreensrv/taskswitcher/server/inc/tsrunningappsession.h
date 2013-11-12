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
#ifndef RUNNINGAPPSESSION_H
#define RUNNINGAPPSESSION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "tsdataobserver.h"
#include "tsdataprovider.h"
#include "tsdatastorage.h"

class CTsRunningAppSession : public CSession2,
                             public MTsDataObserver,
                             public MTsDataObserverStorage
{
public:
    ~CTsRunningAppSession();

    static CTsRunningAppSession* NewL(
                                const MTsDataProvider& aDataProvider,
                                const TArray<MTsDataStorage*> &aDataStorages);

private:
    CTsRunningAppSession( const MTsDataProvider& aDataProvider );
    void ConstructL( const TArray<MTsDataStorage*> &aDataStorages );

public://from CSession2
    void ServiceL( const RMessage2& aMessage );

public://from MTsDataObserver
    void DataChanged();

public://from MTsDataObserverStorage
    void PushL(CTsDataObserver* aObserver);
    void Pop(CTsDataObserver* aObserver);

private:
    const MTsDataProvider& iDataProvider;
    RPointerArray< MTsDataStorage > iDataStorages;
    RPointerArray<CTsDataObserver> iObservers;
    TBool iMissedDataUpdate;
};

#endif // RUNNINGAPPSESSION_H
