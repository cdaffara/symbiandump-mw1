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
#ifndef AFSTORAGESYNCTASK_H
#define AFSTORAGESYNCTASK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "aftask.h"
#include "afstorage.h"
#include "aftaskstorage.h"

class CAfEntry;
/**
 *  CActivityStorageSyncTask
 * 
 */
class AfStorageSyncTask
    {
public:
    static void ExecuteL(MAfTaskStorage& observers,
                         CAfStorage& dataStorage, 
                         const RMessage2& msg);

private:
    static void SaveActivityL(CAfStorage& dataStorage, 
                                const RMessage2& msg);
                                
    static void DeleteActivityL(CAfStorage& dataStorage, 
                                const RMessage2& msg);

    static void DeleteApplicationActivitiesL(CAfStorage& dataStorage, 
                                             const RMessage2& msg);

    static void NotifyChangeL(MAfTaskStorage& observers,
                              const RMessage2& msg);

    static void CreateThumbnailL(const TDesC &path, TInt hdl);
                                
    static void DeleteActivityScreenshotL(CAfStorage& dataStorage,
                                TInt uid,
                                const TDesC &activityName);
    };

#endif // AFSTORAGESYNCTASK_H
