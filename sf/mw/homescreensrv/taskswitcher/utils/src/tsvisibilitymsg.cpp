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
#include <s32mem.h>
#include "tsvisibilitymsg.h"
// -----------------------------------------------------------------------------
CTsVisibilitMsg* CTsVisibilitMsg::NewLC(RReadStream& stream)
{
    CTsVisibilitMsg *self = new (ELeave)CTsVisibilitMsg();
    CleanupStack::PushL(self);
    self->ConstructL(stream);
    return self;
}

// -----------------------------------------------------------------------------
CTsVisibilitMsg* CTsVisibilitMsg::NewLC(TInt windowGroupId, Visibility visi)
{
    CTsVisibilitMsg *self = new (ELeave)CTsVisibilitMsg();
    CleanupStack::PushL(self);
    self->ConstructL(windowGroupId, visi);
    return self;
}
// -----------------------------------------------------------------------------
TInt CTsVisibilitMsg::size()
{
    return sizeof(TInt) * 2;
}

// -----------------------------------------------------------------------------
CTsVisibilitMsg::~CTsVisibilitMsg()
{
    //No implementation required
}

// -----------------------------------------------------------------------------
TInt CTsVisibilitMsg::windowGroupId() const
{
    return mWindowGroupId;
}

// -----------------------------------------------------------------------------
Visibility CTsVisibilitMsg::visibility() const
{
    return mVisibility;
}

// -----------------------------------------------------------------------------
HBufC8* CTsVisibilitMsg::ExternalizeLC() const
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
void CTsVisibilitMsg::ExternalizeL(RWriteStream &stream) const
{
    stream.WriteInt32L(mWindowGroupId);
    stream.WriteInt32L(mVisibility);
    
}

// -----------------------------------------------------------------------------
void CTsVisibilitMsg::InternalizeL(RReadStream &stream)
{
    mWindowGroupId = stream.ReadInt32L();
    mVisibility = static_cast<Visibility>(stream.ReadInt32L());
}

// -----------------------------------------------------------------------------
CTsVisibilitMsg::CTsVisibilitMsg()
{
    //No implementaton required
}

// -----------------------------------------------------------------------------
void CTsVisibilitMsg::ConstructL(RReadStream &stream)
{
    stream >> (*this);
}

// -----------------------------------------------------------------------------
void CTsVisibilitMsg::ConstructL(TInt windowGroupId, Visibility visi)
{
    mWindowGroupId = windowGroupId;
    mVisibility = visi;
}

