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
#include <fbs.h>

#include "tsscreenshotmsg.h"

// -----------------------------------------------------------------------------
CTsScreenshotMsg* CTsScreenshotMsg::NewLC(RReadStream& aStream)
    {
    CTsScreenshotMsg *self = new(ELeave)CTsScreenshotMsg();
    CleanupStack::PushL(self);
    self->ConstructL(aStream);
    return self;
    }

// -----------------------------------------------------------------------------
CTsScreenshotMsg* CTsScreenshotMsg::NewLC( TInt aWindowGroupId, 
                                           const CFbsBitmap& aBitmap, 
                                           UpdatePriority aPrior, 
                                           TInt aRotation )
    {
    CTsScreenshotMsg *self = new(ELeave)CTsScreenshotMsg();
    CleanupStack::PushL(self);
    self->ConstructL(aWindowGroupId, aBitmap, aPrior, aRotation);
    return self;
    }
// -----------------------------------------------------------------------------
TInt CTsScreenshotMsg::CTsScreenshotMsg::Size()
    {
    return sizeof(TInt) * 4;
    }

// -----------------------------------------------------------------------------
CTsScreenshotMsg::~CTsScreenshotMsg()
    {
    delete iBitmap;
    }

// -----------------------------------------------------------------------------
TInt CTsScreenshotMsg::WindowGroupId() const
    {
    return iWindowGroupId;
    }

// -----------------------------------------------------------------------------
const CFbsBitmap& CTsScreenshotMsg::Screenshot() const
    {
    return *iBitmap;
    }

// -----------------------------------------------------------------------------
UpdatePriority CTsScreenshotMsg::Priority() const
    {
    return iPriority;
    }

// -----------------------------------------------------------------------------
TInt CTsScreenshotMsg::Rotation() const
    {
    return iRotation;
    }

// -----------------------------------------------------------------------------
HBufC8* CTsScreenshotMsg::ExternalizeLC() const
    {
    HBufC8* retVal = HBufC8::NewLC(Size());
    TPtr8 des(retVal->Des());
    RDesWriteStream stream(des);
    CleanupClosePushL(stream);
    stream << (*this);
    CleanupStack::PopAndDestroy(&stream);
    return retVal;
    }

// -----------------------------------------------------------------------------
void CTsScreenshotMsg::ExternalizeL(RWriteStream &stream) const
    {
    stream.WriteInt32L(iWindowGroupId);
    stream.WriteInt32L(iBitmap->Handle());
    stream.WriteInt32L(iPriority);
    stream.WriteInt32L(iRotation);
    }
// -----------------------------------------------------------------------------
void CTsScreenshotMsg::InternalizeL(RReadStream &stream)
    {
    iWindowGroupId = stream.ReadInt32L();
    User::LeaveIfError(iBitmap->Duplicate(stream.ReadInt32L()));
    iPriority = static_cast<UpdatePriority>(stream.ReadInt32L());
    iRotation = stream.ReadInt32L();
    }

// -----------------------------------------------------------------------------
CTsScreenshotMsg::CTsScreenshotMsg()
    {
    }
// -----------------------------------------------------------------------------
void CTsScreenshotMsg::ConstructL(RReadStream &stream)
    {
    iBitmap = new(ELeave)CFbsBitmap();
    stream >> (*this);
    }

// -----------------------------------------------------------------------------
void CTsScreenshotMsg::ConstructL(TInt windowGroupId, const CFbsBitmap& bitmap, UpdatePriority prior, TInt aRotation)
    {
    iBitmap = new(ELeave)CFbsBitmap();
    User::LeaveIfError(iBitmap->Duplicate(bitmap.Handle()));
    iWindowGroupId = windowGroupId;
    iPriority = prior;
    iRotation = aRotation;
    }
