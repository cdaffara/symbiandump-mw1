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
* Description: 
*
*/
#include <fbs.h>
#include <s32mem.h>

#include "tsscreenshotnotifier.h"
#include "tsscreenshotmsg.h"

//------------------------------------------------------------------------------
CTsScreenshotNotifier* CTsScreenshotNotifier::NewL( MTsNotifier& aNotifier, 
                                                    TInt aId,
                                                    CFbsBitmap* aBitmap,
                                                    TInt aAngle )
    {
    User::LeaveIfNull(aBitmap);
    CTsScreenshotNotifier* self = 
        new(ELeave)CTsScreenshotNotifier( aNotifier, aId, aBitmap, aAngle );
    return self;
    }

//------------------------------------------------------------------------------
CTsScreenshotNotifier::CTsScreenshotNotifier( MTsNotifier& aNotifier, 
                                              TInt aId,
                                              CFbsBitmap* aBitmap,
                                              TInt aAngle )
:
    CActive( EPriorityIdle ),
    iNotifier( aNotifier ),
    iId( aId ),
    iBitmap( aBitmap ),
    iAngle( aAngle )
    {
    CActiveScheduler::Add(this);
    SetActive();
    TRequestStatus *status( &iStatus );
    User::RequestComplete( status, KErrNone );
    }

//------------------------------------------------------------------------------
CTsScreenshotNotifier::~CTsScreenshotNotifier()
    {
    Cancel();
    delete iBitmap;
    }

//------------------------------------------------------------------------------
TInt CTsScreenshotNotifier::Handle() const
    {
    return iBitmap->Handle();
    }

//------------------------------------------------------------------------------
void CTsScreenshotNotifier::DoCancel()
    {
    //No implementation required
    }

//------------------------------------------------------------------------------
void CTsScreenshotNotifier::RunL()
    {
    RBuf8 message;
    CleanupClosePushL(message);
    message.CreateL(CTsScreenshotMsg::Size() + sizeof(TInt));
    RDesWriteStream stream(message);
    CleanupClosePushL(stream);
    stream.WriteInt32L(RegisterScreenshotMessage);
    CTsScreenshotMsg * screenshotMsg = 
        CTsScreenshotMsg::NewLC(iId, 
                                *iBitmap, 
                                Low, 
                                iAngle);
    stream << (*screenshotMsg);
    CleanupStack::PopAndDestroy(screenshotMsg);
    CleanupStack::PopAndDestroy(&stream);
    iNotifier.SendMessageL(message);
    CleanupStack::PopAndDestroy(&message);
    }

//------------------------------------------------------------------------------
TInt CTsScreenshotNotifier::RunError(TInt /*aError*/)
    {
    delete iBitmap;
    iBitmap = 0;
    return KErrNone;
    }
