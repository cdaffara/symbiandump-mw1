// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __VWSPATCHDATA_H__
#define __VWSPATCHDATA_H__

#include <e32base.h>

/**
 * Rom Patchable constant.
 * If KVwsBoostAppPriorityBeforePanic is '1', then ViewSrv : Panic 11 is not issued during device boot-up. 
 * And also boosts the priority of an application to EPriorityForeground if it is less than EPriorityForeground 
 * and gives an additional timeout of 4 secs by default(this timeout value can be changed at ROM build time through patchable
 * constant KTimeoutValueForPreemptedProcess) to complete the view activation. If the application's priority is 
 * EPriorityForeground then we panic the client immediately.
 
 * The default value of KVwsBoostAppPriorityBeforePanic is 0, which implies that normal view server panic
 * behavior would be in effect. 
 * The constant can be changed at ROM build time using patchdata keyword in IBY file. 
 * To patch the value, add a line to an iby file that is included in the ROM 
 * being built using the following format:
 * "patchdata <dll> @ <symbol> <newvalue>"
 *
 * @SYMPatchable
 * @publishedPartner
 * @released
 * @see KTimeoutValueForPreemptedProcess
 *
 */

IMPORT_C extern const TInt KVwsBoostAppPriorityBeforePanic;

/**
 * Rom Patchable constant.
 * This constant states the additional timeout value for an application after boosting the priority of 
 * the application. This is 4 seconds by default.
 * The constant can be changed at ROM build time using patchdata keyword in IBY file. 
 * To patch the value, add a line to an iby file that is included in the ROM 
 * being built using the following format:
 * "patchdata <dll> @ <symbol> <newvalue>"
 * Example: patchdata viewsrv.dll @ KTimeoutValueForPreemptedProcess 1
 *
 * @SYMPatchable
 * @publishedPartner
 * @released
 *
 */
IMPORT_C extern const TInt KTimeoutValueForPreemptedProcess;

#endif