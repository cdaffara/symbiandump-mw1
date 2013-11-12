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

#ifndef __ASSHDALARM_INTERNAL_H__
#define __ASSHDALARM_INTERNAL_H__

    /*
	 * @internalAll
	 */
	enum TASShdAlarmFlags
		{

		EASShdAlarmFlagsHasAssociatedData	= 0,

		EASShdAlarmFlagsHasOwningSession	= 1,

		EASShdAlarmFlagsHasBecomeOrphaned	= 2,
		
		/*
		 * Set if alarm is disabled manually so that can not be enabled when
		 * locale changes.
		 */
		EASShdAlarmFlagsPermanentDisabled	= 4

		};
		
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	/*
	 * @internalComponent
	 */
	enum TASShdAlarmFlags2
		{
		EASShdAlarmFlag2Wakeup				= 0,
		};
#endif //SYMBIAN_SYSTEM_STATE_MANAGEMENT
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
    /*
	 * @internalComponent
	 */
	enum TASShdAlarmFlags2AlarmRepeatExtensions
		{
		EASShdAlarmFlag2AlarmDayMonday		= 1,
		EASShdAlarmFlag2AlarmDayTuesday		= 2,
		EASShdAlarmFlag2AlarmDayWednesday	= 3,
		EASShdAlarmFlag2AlarmDayThursday	= 4,
		EASShdAlarmFlag2AlarmDayFriday		= 5,
		EASShdAlarmFlag2AlarmDaySaturday	= 6,
		EASShdAlarmFlag2AlarmDaySunday		= 7,
		EASShdAlarmFlag2Continuous			= 8
		};
#endif //SYMBIAN_ALARM_REPEAT_EXTENSIONS

#endif //__ASSHDALARM_INTERNAL_H__
