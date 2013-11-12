/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XIMP Framework panic defines.
 *
*/

#ifndef PRFWTESTCASEUTILSPANICS_H
#define PRFWTESTCASEUTILSPANICS_H

#include <e32std.h>

/**
 * Panic category and reason codes for XIMP Framework
 * test case utils failures.
 */
namespace NXIMPTestCaseUtilsPanic
    {
    _LIT( KCategory, "XIMPTestUtils" );

    enum TReason
        {
        EStatusEventListenerBadMaxCount,
        EStatusEventListenerAlreadyWaiting,
        EStatusEventListenerUnknownEventTemplate,
        EAddSnapshotOverflow,
        ESnapshotIndexOutOfBounds,
        EReceivingEventFromWrongContext,
        EReceivedNotAcceptedEventType,
        EUnsupportedEventTypeInVerification
        };


    inline void Panic( NXIMPTestCaseUtilsPanic::TReason aReason )
        {
        User::Panic( KCategory, aReason );

        }
    }


#endif  //PRFWTESTCASEUTILSPANICS_H





