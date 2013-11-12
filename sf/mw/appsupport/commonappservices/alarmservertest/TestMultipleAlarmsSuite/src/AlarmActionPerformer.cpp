// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains implementation of CAlarmActionPerformer class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "AlarmActionPerformer.h"

// System Include
#include <uikon/eikalsup.h>

/**
Constructor. 
Adds itself to the active scheduler
@internalTechnology
@test
*/
CAlarmActionPerformer::CAlarmActionPerformer() : CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

/**
From CActive. Performs the action and completes the test step thread
@internalTechnology
@test
*/
void CAlarmActionPerformer::RunL()	
	{
	TInt error = KErrNone;
	switch(iAlarmAction)
		{
		case TestMultipleAlarms::ESnooze:
			{
			TRAP(error, iAlarmControl->Supervisor()->CmdTaskAwayFromAlarmL(iMinsToSnooze));
			break;
			}
		case TestMultipleAlarms::EClear:
			{
			iAlarmControl->Supervisor()->CmdAcknowledgeAlarm();
			break;
			}
		case TestMultipleAlarms::EStopActiveScheduler:
			{
			CActiveScheduler::Stop();
			break;
			}
		default:
			error = KErrNotSupported;
			break;
		}
	iTestStepThreadDuplicateHandle.RequestComplete(iExternalRequest, error);
	iTestStepThreadDuplicateHandle.Close();
	}

/**
From CActive. Completes the test step thread with KErrCancel
@internalTechnology
@test
*/
void CAlarmActionPerformer::DoCancel()
	{
	if(iTestStepThreadDuplicateHandle.Handle() != KNullHandle)
		{
		iTestStepThreadDuplicateHandle.RequestComplete(iExternalRequest, KErrCancel);
		iTestStepThreadDuplicateHandle.Close();
		}
	}	
	
/**
Sets up the Alarm Action Performer to perform an action
@param aAlarmControl The alarm control object which we have to snooze/clear
@param aStatus Pointer to the request status which we will complete once we 
are done
@param aTestStepThreadDuplicateHandle A duplicate handle to the test step thread
@param aAlarmAction The action to be performed, i.e, snooze/clear
@internalTechnology
@test
*/
void CAlarmActionPerformer::SetCommonParams(CDummyAlarmControl* aAlarmControl, TRequestStatus* aStatus, RThread& aTestStepThreadDuplicateHandle, const TestMultipleAlarms::TAlarmAction& aAlarmAction)
	{
	iStatus = KRequestPending;
	SetActive(); 
	iExternalRequest = aStatus;
	iAlarmControl = aAlarmControl;
	iTestStepThreadDuplicateHandle = aTestStepThreadDuplicateHandle;
	iAlarmAction = aAlarmAction;
	}
	
/**
Sets the number of minutes to snooze the alarm
@param aMinsToSnooze The number of minutes to snooze the alarm
@internalTechnology
@test
*/
void CAlarmActionPerformer::SetMinsToSnooze(const TInt& aMinsToSnooze)
	{
	iMinsToSnooze = aMinsToSnooze;
	}
	
/**
Returns a pointer to the request status object of this AO
@return A pointer to the request status object of this AO
@internalTechnology
@test
*/
TRequestStatus* CAlarmActionPerformer::Status()	
	{
	return &iStatus;
	}

