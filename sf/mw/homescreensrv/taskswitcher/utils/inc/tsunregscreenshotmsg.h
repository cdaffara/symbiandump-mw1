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

#ifndef TSUNREGISTERSCREENSHOTMSG_H
#define TSUNREGISTERSCREENSHOTMSG_H
#include <e32base.h>
#include <s32strm.h>

class CTsUnregisterScreenshotMsg: public CBase
{
public:
    static CTsUnregisterScreenshotMsg* NewLC(RReadStream&);
    static CTsUnregisterScreenshotMsg* NewLC(TInt windowGroupId);
    
    static TInt size();
    ~CTsUnregisterScreenshotMsg();
    TInt windowGroupId() const;
    HBufC8* ExternalizeLC() const;
    void ExternalizeL(RWriteStream &stream) const;
    void InternalizeL(RReadStream &stream);

private:
    CTsUnregisterScreenshotMsg();
    void ConstructL(RReadStream &stream);
    void ConstructL(TInt windowGroupId);

private:
    TInt mWindowGroupId;
};

#endif
