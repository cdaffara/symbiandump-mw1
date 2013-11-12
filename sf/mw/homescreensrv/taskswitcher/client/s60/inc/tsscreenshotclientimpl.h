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
#ifndef TSSCREENSHOTCLIENTIMPL_H
#define TSSCREENSHOTCLIENTIMPL_H

#include <e32base.h>

#include "tstaskmonitorglobals.h"
#include "tssession.h"

class CFbsBitmap;

NONSHARABLE_CLASS( CTsScreenshotClientImpl ) : public CBase
{
public:
    static CTsScreenshotClientImpl* NewL();
    static CTsScreenshotClientImpl* NewLC();
    ~CTsScreenshotClientImpl();

public:
    TInt RegisterScreenshot(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority);
    TInt UnregisterScreenshot(TInt wgId);
    TInt SetVisibility(TInt wgId, Visibility value);

private:
    CTsScreenshotClientImpl();
    void ConstructL();

    RTsSession iSession;
};

#endif  // TSSCREENSHOTCLIENTIMPL_H
