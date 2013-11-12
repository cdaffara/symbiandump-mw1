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
 * Description:  Definition of ca server constants
 *
 */

#ifndef CASRVDEF_H
#define CASRVDEF_H
#include <e32std.h>
/// Major version number.
LOCAL_D const TInt KCaMajorVersion = 1;
/// Minor version number.
LOCAL_D const TInt KCaMinorVersion = 0;
/// Build number.
LOCAL_D const TInt KCaBuild = 0;
/// Menu Content Service Server name.
_LIT( KCaSrvName, "!CASRV" );
/// Menu Content Service Server executable.
_LIT( KCaSrvExe, "camenuserver" );
/// Exit delay in microseconds (5 sec).
LOCAL_D const TInt KCaSrvExitDelay = 5000000;

const TInt KInputPosition1 = 0;
const TInt KInputPosition2 = 1;
const TInt KOutputPosition = 2;

enum TCaServerRequests
    {
    EContentArsenalGetList_GetSize = 100,
    EContentArsenalGetList_GetData,
    EContentArsenalGetIds_GetSize,
    EContentArsenalGetIds_GetData,
    EContentArsenalAdd,
    EContentArsenalRemove,
    EContentArsenalOrganize,
    EContentArsenalTouch,
    EContentArsenalNotifierOpen,
    EContentArsenalNotifierClose,
    EContentArsenalNotifierNotify,
    EContentArsenalNotifierCancel,
    EContentArsenalGetChangeInfo,
    EContentArsenalCustomSort
    };

#endif // __CASRVDEF_H_
