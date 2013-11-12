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
#ifndef TSMODELITEMKEY_H
#define TSMODELITEMKEY_H

#include <e32base.h>
#include <s32strm.h>

typedef TInt TTsKey;

class TTsModelItemKey
{
public:
    static TInt Size();
    TTsModelItemKey();
    TTsModelItemKey( TTsKey aKey, TInt aRoot );
    TTsModelItemKey( const TTsModelItemKey &aKey );
    TTsModelItemKey& operator =( const TTsModelItemKey& aKey );
    TBool operator == ( const TTsModelItemKey aKey ) const;
    TInt Key() const;
    void ExternalizeL( RWriteStream& aStream ) const;
    void InternalizeL( RReadStream& aStream );

private:
    TTsKey iKey;
    TInt iRoot;
};

#endif //TSMODELITEMKEY_H
