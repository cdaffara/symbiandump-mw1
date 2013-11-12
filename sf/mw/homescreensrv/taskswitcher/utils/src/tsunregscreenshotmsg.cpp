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
#include "tsunregscreenshotmsg.h"
#include <s32mem.h>
// -----------------------------------------------------------------------------
CTsUnregisterScreenshotMsg* CTsUnregisterScreenshotMsg::NewLC(RReadStream& stream)
{
    CTsUnregisterScreenshotMsg* self = new(ELeave)CTsUnregisterScreenshotMsg();
    CleanupStack::PushL(self);
    self->ConstructL(stream);
    return self;
}

// -----------------------------------------------------------------------------
CTsUnregisterScreenshotMsg* CTsUnregisterScreenshotMsg::NewLC(TInt windowGroupId)
{
    CTsUnregisterScreenshotMsg* self = new(ELeave)CTsUnregisterScreenshotMsg();
    CleanupStack::PushL(self);
    self->ConstructL(windowGroupId);
    return self;
}

// -----------------------------------------------------------------------------
CTsUnregisterScreenshotMsg::CTsUnregisterScreenshotMsg()
{
    //No implementation required
}
// -----------------------------------------------------------------------------
void CTsUnregisterScreenshotMsg::ConstructL(RReadStream &stream)
{
    stream >> (*this);
}

// -----------------------------------------------------------------------------
void CTsUnregisterScreenshotMsg::ConstructL(TInt windowGroupId)
{
    mWindowGroupId = windowGroupId;
}

// -----------------------------------------------------------------------------
TInt CTsUnregisterScreenshotMsg::size()
{
    return sizeof(TInt);
}

// -----------------------------------------------------------------------------
CTsUnregisterScreenshotMsg::~CTsUnregisterScreenshotMsg()
{
    //No implementation required
}

// -----------------------------------------------------------------------------
TInt CTsUnregisterScreenshotMsg::windowGroupId() const
{
    return mWindowGroupId;
}

// -----------------------------------------------------------------------------
HBufC8* CTsUnregisterScreenshotMsg::ExternalizeLC() const
{
    HBufC8* retVal = HBufC8::NewLC(size());
    TPtr8 des(retVal->Des());
    RDesWriteStream stream(des);
    CleanupClosePushL(stream);
    stream << (*this);
    CleanupStack::PopAndDestroy(&stream);
    return retVal;
}

// -----------------------------------------------------------------------------
void CTsUnregisterScreenshotMsg::ExternalizeL(RWriteStream &stream) const
{
    stream.WriteInt32L(mWindowGroupId);
}

// -----------------------------------------------------------------------------
void CTsUnregisterScreenshotMsg::InternalizeL(RReadStream &stream)
{
    mWindowGroupId = stream.ReadInt32L();
}
