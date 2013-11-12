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
#ifndef AFTHUMBNAILTASK_H
#define AFTHUMBNAILTASK_H

#ifndef __E32SVR_H__
#define __E32SVR_H__
#endif

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

#include "aftask.h"
#include "aftaskstorage.h"

class CFbsBitmap;

class CAfThumbnailTask : public CAfTask
{
public:
    ~CAfThumbnailTask();

    static void ExecuteLD(MAfTaskStorage& taskStorage,
                          const RMessage2& message);

private:
    CAfThumbnailTask(MAfTaskStorage& storage, const RMessage2 msg);

    void ConstructL();

    const TDesC8& Data()const;

    void BroadcastReceivedL(const RMessage2&, TBool applicationUninstalled = EFalse);

    TBool IsSessionTask(const CSession2* session);

private:
    MAfTaskStorage& mStorage; 
    const RMessage2 mMsg;
    CFbsBitmap* mBitmap;
};

#endif // AFTHUMBNAILTASK_H
