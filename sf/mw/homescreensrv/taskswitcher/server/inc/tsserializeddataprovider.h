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
#ifndef SERIALIZEDDATAPROVIDER_H
#define SERIALIZEDDATAPROVIDER_H

#include "tsdataobserver.h"
#include "tsdataprovider.h"
#include "tsmodelobserver.h"

class RWriteStream;

class CTsSerializedDataProvider: public CBase,
                                 public MTsModelObserver,
                                 public MTsDataProvider
{
public:
    static CTsSerializedDataProvider* NewL(MTsDataObserver& aObserver);
    ~CTsSerializedDataProvider();
    const TDesC8& Data() const;
    void DataChanged(MTsModel &aModel);

private:
    CTsSerializedDataProvider(MTsDataObserver& aObserver);
    void serializeModel(const MTsModel& aSrc);
    void serializeModelL(const MTsModel& aSrc);
    void serializeModelL(RWriteStream& aDst, const MTsModel& aSrc); 

private:
    MTsDataObserver& iObserver;
    RBuf8 iData;

};

#endif //SERIALIZEDDATAPROVIDER_H
