/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __ASSHDALARMCAL_H__
#define __ASSHDALARMCAL_H__

#include <e32base.h>

/** The Alarm publish and subscribe category
@publishedPartner
@released
*/
const TUid KAlarmServerPubSubCategory = { 0x101f5027 };

/** Used for subcribing missed alarms or time zone changes
@publishedPartner
@released
*/
const TUint KMissingAlarmPubSubKey = 100;

/** The publish and subscribe data for KMissingAlarmPubSubKey
@publishedPartner
@released
*/
struct TMissedAlarmPubSubData
	{
	/** The value indicating the changes. 
	1 - Time zone has been changes but there are no missed alarms
	2 - Some alarms have been missed after system time or time zone has changed.
	*/
	TUint8 iValue;
	
	/** The time that system time change took place, in universal (UTC) time */
	TTime iTimeOfChangeUtc;
	};

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
/** Used for subscribing to data used when searching for instances in Calendar
@publishedPartner
@released
*/
const TUint KSkippedAlarmInstancesPubSubKey = 101;

/** The publish and subscribe data for KMissingAlarmInstancesPubSubKey
@publishedPartner
@released
*/
struct TASShdAlarmedInstanceParams
	{
	/** The start of the time range in local time. */ 
	TTime iLocalStartTime;

	/** The end of the time range in local time. */
	TTime iLocalEndTime;
	
	/** The alarm time types to include. */
	TASShdAlarmTimeType iTimeType;
	};
#endif //SYMBIAN_SKIPPED_CALENDAR_ALARMS
#endif //__ASSHDALARMCAL_H__
