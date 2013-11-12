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
#ifndef TSMODELITEM_H
#define TSMODELITEM_H

#include <e32base.h>
#include "tsmodelitemkey.h"

class MTsModel;

class TTsModelItem {
public:
    TTsModelItem( const MTsModel& aModel, TInt aOffset );
    TTsModelItem( const TTsModelItem& aItem );
    const TDesC& DisplayNameL() const;
    TInt IconHandleL() const;
    TTime TimestampL() const;
    TTime TimestampUpdateL() const; 
    TTsModelItemKey KeyL() const;
    TBool IsActiveL() const;
    TBool IsClosableL() const;
    TBool CloseL() const;
    TBool LaunchL() const;
    TBool IsMandatoryL() const;
    
public:
    void ExternalizeL( RWriteStream& aStream ) const;
    
private:
    void ValidateL() const;

private:
    const MTsModel& iModel;
    const TInt iIndex;
};

#endif //TSMODELITEM_H
