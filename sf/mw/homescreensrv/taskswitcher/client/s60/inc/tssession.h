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
#ifndef TSSESSION_H
#define TSSESSION_H

#include <e32base.h>

#include "tstaskmonitorglobals.h"

class CFbsBitmap;

NONSHARABLE_CLASS( RTsSession ) : public RSessionBase
{
public:
    TInt Connect();

public: // task monitor API
    HBufC8* TasksContentLC();
    void Subscribe(TRequestStatus& aStatus);
    void CancelSubscribe();
    void OpenTask(const TDesC8 &key);
    void CloseTask(const TDesC8 &key);  

public: // screenshots API
    TInt RegisterScreenshot(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority);
    TInt UnregisterScreenshot(TInt wgId);
    TInt SetVisibility(TInt wgId, Visibility value);
    
private:
    void RegisterScreenshotL(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority);
    void UnregisterScreenshotL(TInt wgId);
    void SetVisibilityL(TInt wgId, Visibility value);
    
private:
    TInt StartServer();
};

#endif  // TSSESSION_H
