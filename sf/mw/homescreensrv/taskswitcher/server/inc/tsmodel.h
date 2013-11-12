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
#ifndef TSMODEL_H
#define TSMODEL_H

#include <e32base.h>
#include "tsmodelitem.h"

class MTsModelObserver;
class MTsModel 
{
public:
    virtual TInt Count() const = 0;
    TTsModelItem ItemL( TInt aOffset ) const;
    virtual void SetObserver( MTsModelObserver *observer ) = 0;
    
protected: //data access. should be used by TModelItem
    virtual const TDesC& DisplayNameL( TInt aOffset ) const = 0;
    virtual TInt IconHandleL( TInt aOffset ) const = 0;
    virtual TTime TimestampL( TInt aOffset ) const = 0;
    virtual TTime TimestampUpdateL(TInt offset) const = 0;
    virtual TTsModelItemKey KeyL( TInt aOffset ) const = 0;
    virtual TBool IsActiveL( TInt aOffset ) const = 0;
    virtual TBool IsClosableL( TInt aOffset ) const = 0;
    virtual TBool IsMandatoryL( TInt aOffset ) const = 0;

protected: //operations execution
    virtual TBool CloseL( TTsModelItemKey aKey ) const = 0;
    virtual TBool LaunchL( TTsModelItemKey aKey ) const = 0;

    friend class TTsModelItem;
};

#endif //TSMODEL_H
