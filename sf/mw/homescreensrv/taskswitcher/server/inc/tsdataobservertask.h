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
#ifndef DATAOBSERVERTASK_H
#define DATAOBSERVERTASK_H
#include <e32base.h>
#include "tsdataobserver.h"

class CTsDataObserverTask: public CTsDataObserver
{
public:
    ~CTsDataObserverTask();
    static void ExecuteLD( MTsDataObserverStorage& aStorage, 
                           const RMessage2& aMsg );

private:
    CTsDataObserverTask( MTsDataObserverStorage& aStorage, 
                         const RMessage2& aMsg );
    void DataChanged();

private:
    MTsDataObserverStorage& iStorage;
    const RMessage2 iMsg;
    TBool iDataChanged;
};
#endif //DATAOBSERVERTASK_H
