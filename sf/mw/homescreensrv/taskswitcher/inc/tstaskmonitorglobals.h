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
#ifndef RUNNINGAPPGLOBALS_H
#define RUNNINGAPPGLOBALS_H

#include <e32base.h>

_LIT(KRunningAppServerName, "hsrunningappmonitor");
_LIT(KTsServerReadySemaphore, "TaskSwitcherReadySemaphore");
const TUid KRunningAppServerUid = {0x200267B0};
const TInt KRequestDataSizeOffset(0); 
const TInt KRequestDataBufferOffset(0);

enum RunningAppCmd {
    GetRunningAppInfo =0,
    SubscribeRunningAppChanges,
    UpdateScreenshot,
    FlushData,
    CancelSubscribeRunningAppChanges,
    RegisterScreenshotMessage,
    UnregisterScreenshotMessage,
    VisibilityChange,
    OpenTaskMessage,
    CloseTaskMessage,
    WindowGroupToBackgroundMessage,
    AllowedWindowGroups
};

enum UpdatePriority {
    Low = 0,
    Normal,
    High
};

enum Visibility {
    Invisible =0,
    Visible
};

enum ScreenshotMessageOffsets {
    ScreenshotHandle = 0,
    WindowsGroup,
    AdditionalParameters,
    Priority
};

#endif  //RUNNINGAPPGLOBALS_H

