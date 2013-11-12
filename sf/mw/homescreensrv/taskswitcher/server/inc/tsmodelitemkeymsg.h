/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Task list entry
*
*/

#ifndef TSMODELITEMKEYMSG_H
#define TSMODELITEMKEYMSG_H
#include <e32base.h>
#include <s32strm.h>
#include "tsmodelitemkey.h"

class CTsModelItemKeyMsg: public CBase
{
public:
    static CTsModelItemKeyMsg* NewLC( RReadStream& aStream );
    static TInt Size();
    ~CTsModelItemKeyMsg();
    TTsModelItemKey Key() const;
    void InternalizeL( RReadStream &aStream );

private:
    CTsModelItemKeyMsg();
    void ConstructL( RReadStream &aStream );

private:
    TTsModelItemKey iKey;
};

#endif
