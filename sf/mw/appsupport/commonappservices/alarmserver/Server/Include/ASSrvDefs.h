// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __ASSRVDEFS_H__
#define __ASSRVDEFS_H__

// System includes
#include <e32std.h>

// Type definitions
typedef TInt TASSrvSessionId;
typedef TInt TASSrvAlarmSoundCycleNumber;

// Constants
const TInt KDefaultHeapMinSize					= 0x00006000;
const TInt KDefaultHeapMaxSize					= 0x000FFFFF;
const TInt KAlarmServerStartupPriority			= 0x00002000;
const TASSrvSessionId KNullSessionId			= 0;

// Store constants
const TUid KAlarmStoreUid								= { 0x101F502C };
const TUid KAlarmStoreStreamUidQueue					= { 0x101F502D };
const TUid KAlarmStoreStreamUidData						= { 0x101F502E };
const TUid KAlarmStoreStreamUidSoundSettings			= { 0x101F502F };
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
const TUid KAlarmStoreStreamUidEnvironmentChangeData	= { 0x10286A7D };
#endif

// Literal constants
_LIT(KAlarmServerBackupFile, "AlarmServer.ini");

#endif
