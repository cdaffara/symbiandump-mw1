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
//

#include "AutoUpdateNotification.h"
#include "UtcOffsetChangeEventNotifier.h"
#include "DstStep.h"

CAutoUpdateNotification::CAutoUpdateNotification()
	{
	SetTestStepName(KAutoUpdateNotification);
	}
	
CAutoUpdateNotification::~CAutoUpdateNotification()
	{
	iTzServer.Close();
	delete iActiveScheduler;
	}

TVerdict CAutoUpdateNotification::doTestStepL()
	{
	TVerdict result1 = TestAutoUpdateNotificationL(RTz::ETZAutoDSTUpdateOn);
	TVerdict result2 = TestAutoUpdateNotificationL(RTz::ETZAutoDSTUpdateOff);
	TVerdict result3 = TestAutoUpdateNotificationL(RTz::ETZAutoDSTNotificationOnly);

	if ((result1 == EPass) && (result2 == EPass) && (result3 == EPass))
		{
		return EPass;
		}
	return EFail;
	}
   
TVerdict CAutoUpdateNotification::TestAutoUpdateNotificationL(RTz::TTzAutoDSTUpdateModes aAutoUpdateMode)
	{
	SetTestStepResult(EFail);
	TInt err(KErrNone);
	User::LeaveIfError(iTzServer.SetHomeTime(iInitialTime));
	TTimeIntervalSeconds oldOffset = User::UTCOffset();
	CUtcOffsetChangeEventNotifier* notifier = CUtcOffsetChangeEventNotifier::NewL();
	CleanupStack::PushL(notifier);
	
	// set the auto update functionality
	iTzServer.SetAutoUpdateBehaviorL(aAutoUpdateMode);

	TRAP(err,CreateUTCOffsetChangeEventL());
	if(err)
		{
		SetTestStepError(err);
		return TestStepResult();
		}
		
	switch(iTestType)
		{
	case EAutoDSTUpdate:
			{
			if (aAutoUpdateMode != RTz::ETZAutoDSTUpdateOff)
				{
				// Wait for the notification (If DST change notifier somehow does not return, test harness will time out and fail)
				
				notifier->WaitForNotification();

				// We have been notified. check the status if everything's right.
				err = notifier->iStatus.Int();
				TTimeIntervalSeconds newOffset = User::UTCOffset();

				//
				// the UTC offset should be changed ONLY when AutoUpdate is ON.
				//
				TBool isOffsetRight = (aAutoUpdateMode == RTz::ETZAutoDSTUpdateOn) ?
										newOffset!=oldOffset : newOffset==oldOffset;

				if((err == KErrNone) && isOffsetRight)
					{
					SetTestStepResult(EPass);
					}
				}
			else
				{
				//
				// Auto update is switched OFF. We should NOT receive notification
				//
				User::After(1000000);
				TInt threadsRequestSemaphoreCount = RThread().RequestCount();
			
            	if (threadsRequestSemaphoreCount == 0)
            		{
            		SetTestStepResult(EPass);
            		}
            	notifier->Cancel();	
				}
			}
			break;	
		case ESystemTimeChanged:
			{
			User::LeaveIfError(iTzServer.SetHomeTime(iChangedTime));

			// Wait for a notification - there shouldn't be one when
			// Time is changed manually or Time zone is changed    (see DEF59694)
			User::After(1000000);
			TInt threadsRequestSemaphoreCount = RThread().RequestCount();

			if (threadsRequestSemaphoreCount == 0)
				{
				SetTestStepResult(EPass);
				}
			notifier->Cancel();   
			}
			break;   
		case ETimeZoneChanged:
			{
			CTzId* tzId = CTzId::NewL(iChangedLocation);
			CleanupStack::PushL(tzId);
			iTzServer.SetTimeZoneL(*tzId);
			CleanupStack::PopAndDestroy(tzId);   

			// Wait for a notification - there shouldn't be one when
			// Time is changed manually or Time zone is changed    (see DEF59694)
			User::After(1000000);
			TInt threadsRequestSemaphoreCount2 = RThread().RequestCount();

			if (threadsRequestSemaphoreCount2 == 0)
				{
				SetTestStepResult(EPass);
				}
			notifier->Cancel();   
			}
			break;       
		default:
			{
			SetTestStepResult(EFail);
			ERR_PRINTF1(_L("Unknown test type"));
			User::Leave(KErrNotFound);
			}
		}

	CleanupStack::PopAndDestroy(notifier);

	return TestStepResult();
	}

	
void CAutoUpdateNotification::CreateUTCOffsetChangeEventL()
	{
	switch(iTestType)
		{
	case EAutoDSTUpdate:
		// do nothing, just wait till it turns to summer time or winter time.
		break;	
	case ESystemTimeChanged:
		break;
	case ETimeZoneChanged:
		break;
	default:
		SetTestStepResult(EFail);
		ERR_PRINTF1(_L("Unknown test type"));
		User::Leave(KErrNotFound);
		}
	}	
	
	
TVerdict CAutoUpdateNotification::doTestStepPreambleL()
	{
	iActiveScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	// connect to the time zone server
	User::LeaveIfError(iTzServer.Connect());

	// set test type - depends on the cause of UTC offset change event
	SetTestType();
	TRAPD(err,InitialiseTestStepL());
	if(err)
		{
		SetTestStepError(err);
		SetTestStepResult(EFail);
		ERR_PRINTF2(_L("Initialising test step failed err = %d"), err);
		}
		
	return TestStepResult();
	}
	
void CAutoUpdateNotification::SetTestType()
	{
	_LIT(KCauseOfEvent,		"CauseOfEvent");
	TPtrC ptr;
	GetStringFromConfig(ConfigSection(), KCauseOfEvent, ptr);
	
	_LIT(KAutoDstUpdate,	"AutoDSTUpdate");
	_LIT(KSystemTimeChange,	"SystemTimeChange");
	_LIT(KTimeZoneChange,	"TimeZoneChange");
	
	if(!ptr.Compare(KAutoDstUpdate))
		iTestType = EAutoDSTUpdate;
	else if(!ptr.Compare(KSystemTimeChange))
		iTestType = ESystemTimeChanged;
	else if(!ptr.Compare(KTimeZoneChange))
		iTestType = ETimeZoneChanged;
	}
	
void CAutoUpdateNotification::InitialiseTestStepL()
	{
	TPtrC ptr;
	_LIT(KInitialTimeTag, "InitialTime");
	_LIT(KLocationTag, "Location");
	
	GetStringFromConfig(ConfigSection(), KLocationTag, ptr);
	iLocation.Copy(ptr);
	
	// set time zone
	CTzId* tzId = CTzId::NewL(iLocation);
	CleanupStack::PushL(tzId);
	iTzServer.SetTimeZoneL(*tzId);
	CleanupStack::PopAndDestroy(tzId);
	
	// set initial system time
	GetStringFromConfig(ConfigSection(), KInitialTimeTag, ptr);
	iInitialTime = CDstStep::ReadTimeParamStringL(ptr);
	User::LeaveIfError(iTzServer.SetHomeTime(iInitialTime));
	
	switch(iTestType)
		{
	case EAutoDSTUpdate:
		break;
	case ESystemTimeChanged:
			{
			_LIT(KChangedTime,	"Changedtime");
			GetStringFromConfig(ConfigSection(), KChangedTime, ptr);
			iChangedTime = 	CDstStep::ReadTimeParamStringL(ptr);
			}
		break;
	case ETimeZoneChanged:
			{
			_LIT(KChangedLocation,	"ChangedLocation");
			GetStringFromConfig(ConfigSection(), KChangedLocation, ptr);
			iChangedLocation.Copy(ptr);
			}
		break;
	default:
			SetTestStepResult(EFail);
			ERR_PRINTF1(_L("Unknown test type"));
			User::Leave(KErrNotFound);
		}
	
	}

TVerdict CAutoUpdateNotification::doTestStepPostambleL()
	{
	iTzServer.Close();	
	return TestStepResult();
	}
