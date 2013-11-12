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

#include "afactivitylauncher_p.h"

#include <uri16.h>
#include <delimitedquery16.h>

#include <afstorageclient.h>
#include <afentry.h>

// -----------------------------------------------------------------------------
//  Public part of construction, destructor
// -----------------------------------------------------------------------------
//
CAfActivityLauncherPrivate *CAfActivityLauncherPrivate::NewL(RApaLsSession &apaLsSession, RWsSession &wsSession)
{
    CAfActivityLauncherPrivate *self = new (ELeave)CAfActivityLauncherPrivate(apaLsSession, wsSession);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CAfActivityLauncherPrivate::~CAfActivityLauncherPrivate()
{
    delete mActivitiesClient;
}

// -----------------------------------------------------------------------------
//  Private part of construction
// -----------------------------------------------------------------------------
//
CAfActivityLauncherPrivate::CAfActivityLauncherPrivate(RApaLsSession &apaLsSession, RWsSession &wsSession) : mApplicationLauncher(apaLsSession, wsSession)
{
}

void CAfActivityLauncherPrivate::ConstructL()
{
    mActivitiesClient = CAfStorageClient::NewL(*this);
}

// -----------------------------------------------------------------------------
//  Implementation of MAfAsyncRequestObserver interface. We don't need any
//  notifications, but we have to pass an instance of this interface to
//  CAfStorageClient constructor.
// -----------------------------------------------------------------------------
//
void CAfActivityLauncherPrivate::waitActivityRequestCompleted(int /*result*/, const TDesC8 &/*data*/)
{
}

void CAfActivityLauncherPrivate::getThumbnailRequestCompleted(int /*result*/, int /*bitmapHandle*/, void */*userData*/)
{
}

void CAfActivityLauncherPrivate::dataChangeNotificationCompleted(int /*result*/)
{
} 

// -----------------------------------------------------------------------------
//  Actual activity launching
// -----------------------------------------------------------------------------
//

void CAfActivityLauncherPrivate::launchActivityL(const TDesC &uriDesc)
{
    TUriParser16 parser;
    parser.Parse(uriDesc);

    if (parser.Extract(EUriScheme).Compare(_L("appto")) == 0) {
        TLex16 uidParser(parser.Extract(EUriHost));        
        TUint uid;
        User::LeaveIfError(uidParser.Val(uid, EHex));
        
        if (mApplicationLauncher.isRunning(TUid::Uid(uid))) {        
            CAfEntry *launchRequest = CAfEntry::NewLC(0, uid, uriDesc, KNullDesC(), KNullDesC(), KNullDesC8(), KNullDesC8(), TTime());
            User::LeaveIfError(mActivitiesClient->launchActivity(*launchRequest));
            CleanupStack::PopAndDestroy(launchRequest);
        } else {
            mApplicationLauncher.startApplicationL(TUid::Uid(uid), uriDesc);
        }        
    } else {
        User::Leave(KErrArgument);
    }
}
