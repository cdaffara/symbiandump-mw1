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
#ifndef TSSTORAGE_H
#define TSSTORAGE_H
#include <e32base.h>

#include "tsmodel.h"
#include "tsmodelobserver.h"
#include "tsdataobserver.h"
#include "tsdatastorage.h"

class CTsStorage: public CBase,
                  public MTsModel,
                  public MTsModelObserver,
                  public MTsDataObserver,
                  public MTsDataStorage
{
public:
    static CTsStorage* NewL(const TArray<MTsModel*> &aDataProviders);
    ~CTsStorage();
    
private:
    CTsStorage();
    void ConstructL(const TArray<MTsModel*> &aDataProviders);
    TInt ResetModel();
    void ResetModelL();
    void PullDataL(const MTsModel& aSrc);
    void ReorderDataL();
    void TrimDataL();

public://from MTsModelObserver
    void DataChanged(MTsModel& aModel);

public://from MTsModel
    TInt Count() const;
    void SetObserver(MTsModelObserver* aObserver);
    
private://from MTsModel
    const TDesC& DisplayNameL( TInt aOffset ) const;
    TInt IconHandleL( TInt aOffset ) const;
    TTime TimestampL( TInt aOffset ) const;
    TTime TimestampUpdateL(TInt offset) const;
    TTsModelItemKey KeyL( TInt aOffset ) const;
    TBool IsActiveL( TInt aOffset ) const;
    TBool IsClosableL( TInt aOffset ) const;
    TBool IsMandatoryL( TInt aOffset ) const;
    TBool CloseL( TTsModelItemKey aKey ) const;
    TBool LaunchL( TTsModelItemKey aKey ) const;
    TTsModelItem FindL( TTsModelItemKey aKey ) const;

public://from MTsDataObserver
    void DataChanged();

public://from MTsDataStorage
    TBool IsSupported(TInt aFunction) const;
    void HandleDataL(TInt aFunction, RReadStream& aDataStream);

private:
    RPointerArray<MTsModel> iDataProviders;
    RArray<TTsModelItem> iData;
    MTsModelObserver* iDataObserver;

};

#endif //TSSTORAGE_H
