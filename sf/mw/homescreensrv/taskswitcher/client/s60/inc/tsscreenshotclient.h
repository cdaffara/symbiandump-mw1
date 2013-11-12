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
#ifndef TSSCREENSHOTCLIENT_H
#define TSSCREENSHOTCLIENT_H

#include <e32base.h>

#include "tstaskmonitorglobals.h"

class CFbsBitmap;
class CTsScreenshotClientImpl;

NONSHARABLE_CLASS( CTsScreenshotClient ) : public CBase
{
public:
    IMPORT_C static CTsScreenshotClient* NewL();
    IMPORT_C static CTsScreenshotClient* NewLC();
    IMPORT_C ~CTsScreenshotClient();

public:
    IMPORT_C TInt RegisterScreenshot(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority = Normal);
    IMPORT_C TInt UnregisterScreenshot(TInt wgId);
    IMPORT_C TInt SetVisibility(TInt wgId, Visibility value);

private:
    CTsScreenshotClient();
    void ConstructL();

private:
    CTsScreenshotClientImpl* iImpl;
};

#endif  // TSSCREENSHOTCLIENT_H
