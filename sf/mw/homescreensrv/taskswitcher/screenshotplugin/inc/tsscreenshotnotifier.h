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
#ifndef TSSCREENSHOTNOTIFIER_H
#define TSSCREENSHOTNOTIFIER_H

#include "tsnotifier.h"
class CFbsBitmap;

class CTsScreenshotNotifier: public CActive
    {
public:
    static CTsScreenshotNotifier* NewL( MTsNotifier& aNotifier,
                                        TInt aId,
                                        CFbsBitmap* aBitmap,
                                        TInt aAngle );
    ~CTsScreenshotNotifier();

    TInt Handle() const;
private:
    CTsScreenshotNotifier( MTsNotifier& aNotifier, 
                           TInt aId,
                           CFbsBitmap* aBitmap,
                           TInt aAngle );

protected://from CActive
    void DoCancel();
    void RunL();
    TInt RunError(TInt aError);

private:
    MTsNotifier& iNotifier;
    const TInt iId;
    const CFbsBitmap* iBitmap;
    const TInt iAngle;

    };

#endif //SSCREENSHOTNOTIFIER_H
