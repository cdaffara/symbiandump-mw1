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

#ifndef TSVISIBILITYMSG_H
#define TSVISIBILITYMSG_H
#include <e32base.h>
#include <s32strm.h>

#include "tstaskmonitorglobals.h"

class CTsVisibilitMsg: public CBase
{
public:
    static CTsVisibilitMsg* NewLC(RReadStream&);
    static CTsVisibilitMsg* NewLC(TInt windowGroupId, Visibility visi);
    
    static TInt size();
    ~CTsVisibilitMsg();
    TInt windowGroupId() const;
    Visibility visibility() const;
    HBufC8* ExternalizeLC() const;
    void ExternalizeL(RWriteStream &stream) const;
    void InternalizeL(RReadStream &stream);

private:
    CTsVisibilitMsg();
    void ConstructL(RReadStream &stream);
    void ConstructL(TInt windowGroupId, Visibility visi);

private:
    TInt mWindowGroupId;
    Visibility mVisibility;
};

#endif
