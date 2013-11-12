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

#ifndef TSSCREENSHOTMSG_H
#define TSSCREENSHOTMSG_H
#include <e32base.h>
#include <s32strm.h>
#include "tstaskmonitorglobals.h"
class CFbsBitmap;

class CTsScreenshotMsg: public CBase
{
public:
    static CTsScreenshotMsg* NewLC( RReadStream& aStream );
    static CTsScreenshotMsg* NewLC( TInt aWindowGroupId, 
                                    const CFbsBitmap& aBitmap, 
                                    UpdatePriority aPrior,
                                    TInt aRotation );
    
    static TInt Size();
    ~CTsScreenshotMsg();
    TInt WindowGroupId() const;
    const CFbsBitmap& Screenshot() const;
    UpdatePriority Priority() const;
    TInt Rotation()const;
    HBufC8* ExternalizeLC() const;
    void ExternalizeL(RWriteStream &stream) const;
    void InternalizeL(RReadStream &stream);

private:
    CTsScreenshotMsg();
    void ConstructL(RReadStream &stream);
    void ConstructL(TInt windowGroupId, const CFbsBitmap& bitmap, UpdatePriority prior, TInt aRotation);

private:
    TInt iWindowGroupId;
    UpdatePriority iPriority;
    CFbsBitmap* iBitmap;
    TInt iRotation;
};

#endif
