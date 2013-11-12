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
#ifndef TSSERVICE_H
#define TSSERVICE_H

#include <e32base.h>

#include "tsmodel.h"
#include "tsdataobserver.h"

class QObject;
class TsAbstractModel;
class MTsResourceManager;

class CTsService: public CBase,
                  public MTsModel,
                  public MTsDataObserver
{
public:
    static CTsService* NewLC( MTsResourceManager& aResources, QObject* model );
    ~CTsService();

public: //from MTsDataObserver
    void DataChanged();

public: //from MTsModel
    TInt Count() const;
    void SetObserver( MTsModelObserver *aObserver );
    
protected: //from MTsModel
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
    
private:
    CTsService();
    void ConstructL( MTsResourceManager& aResources, QObject* aModel );
    TInt IntValueL( TInt aOffset, const char* aKey ) const;
    TTime TimeValueL( TInt aOffset, const char* aKey ) const;
    const TDesC& StringValueL( TInt aOffset, const char* aKey ) const;
private:
    QObject *iModel;
    QObject *iServiceObserver;
    MTsModelObserver *iModelObserver;
    HBufC *iBuffer;
};
#endif //TSSERVICE_H
