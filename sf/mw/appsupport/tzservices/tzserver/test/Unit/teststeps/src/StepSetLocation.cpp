// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file StepSetLocation.cpp
*/

#include "StepSetLocation.h"
#include <s32mem.h>
#include <utf.h> // CnvUtfConverter
#include "tzrules.h"
#include <centralrepository.h>  // CRepository
#include <tzupdate.h>  // ntzupdate
#include <e32test.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif
#include "timezoneserver.h"
_LIT(KZoneNameId, "ZoneNameId");
_LIT(KZoneNoId, "ZoneNoId");
_LIT(KUTCTime, "UTCTime");
_LIT(KLocalTime, "LocalTime");
_LIT(KConvert, "ConvertToUTC");
_LIT(KSetZone, "SetZone");

LOCAL_D RTest test(_L("DST"));

// Central Repository data.
const TUid KPropertyCategory = { 0x1020383E };
const TInt KConfigurationKey = 0x01;
  
CSetLocation::~CSetLocation()
	{
	delete iTimeConverter;
	iTimeZoneServer.Close();
	}

CSetLocation::CSetLocation(CDstUnitServer &aServer) : CDstUnitStep(aServer)
	{
	SetTestStepName(KStepSetLocation);
	}

TInt CSetLocation::TestCTimeZoneIdAPIsL()
	{
	// Set Time Zone by NameId
	_LIT8(KTZEuropeLondon, "Europe/London");

	//Test CTzId Serialisation API Calls
	CTzId* timeZoneId = CTzId::NewL(KTZEuropeLondon());
	CleanupStack::PushL(timeZoneId);

	CBufFlat* buffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(buffer);

	RBufWriteStream writeStream;
	writeStream.Open(*buffer);

	timeZoneId->ExternalizeL(writeStream);
	writeStream.CommitL();
	writeStream.Close();

	RBufReadStream readStream;
	readStream.Open(*buffer);
	timeZoneId->InternalizeL(readStream);
	CleanupStack::PopAndDestroy(buffer);
	CleanupStack::PopAndDestroy(timeZoneId);

	return KErrNone;
	}


TInt CSetLocation::TestTimeZoneSettingL()
	{
	// Set Time Zone by NameId
	_LIT8(KTZEuropeLondon, "Europe/London");

	//Test CTzId Serialisation API Calls
	CTzId* timeZoneId = CTzId::NewL(KTZEuropeLondon());
	CleanupStack::PushL(timeZoneId);
	iTimeZoneServer.SetTimeZoneL(*timeZoneId);
	CleanupStack::PopAndDestroy(timeZoneId);
	return KErrNone;
	}


TInt CSetLocation::TestTimeZoneIdRetrievingL()
	{

	// Get Time Zone
	CTzId* timeZoneId = iTimeZoneServer.GetTimeZoneIdL();
	delete timeZoneId;
	return KErrNone;
	}

void CSetLocation::DestroyTestUnit()
	{
	delete iTimeConverter;
	iTimeConverter = NULL;
	iTimeZoneServer.Close();
	}


TInt CSetLocation::CreateAndRunTestUnitL()
	{
	TInt results = KErrNone;
	TInt failures = 0;
	User::LeaveIfError(iTimeZoneServer.Connect());

	iTimeConverter = CTzConverter::NewL(iTimeZoneServer);

	results = TestCTimeZoneIdAPIsL();
	failures = ((results != KErrNone) ? (failures + 1) : failures);
	results = TestTimeZoneSettingL();
	failures = ((results != KErrNone) ? (failures + 1) : failures);
	results = TestTimeZoneIdRetrievingL();
	failures = ((results != KErrNone) ? (failures + 1) : failures);

	return ((failures) ? KErrUnknown : KErrNone);
	}



//
// CConvertTimeActiveWorker
// Controls testing to allow asynchronous behaviour of Time Zone Change
// Notification to be Observed
CTzTestActiveWorker::~CTzTestActiveWorker()
	{
	Deque();
	}

CTzTestActiveWorker::CTzTestActiveWorker(const CTZUnitTestBase& aTzTestStep) : 
	CActive(CActive::EPriorityStandard), iTzTestStep(const_cast<CTZUnitTestBase&>(aTzTestStep))
	{}


void CTzTestActiveWorker::CompleteSelf()
	{
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,KErrNone);
	}

CTzTestActiveWorker* CTzTestActiveWorker::NewL(const CTZUnitTestBase& aTzTestStep)
	{
	CTzTestActiveWorker* self = new(ELeave) CTzTestActiveWorker(aTzTestStep);
	CActiveScheduler::Add(self);
	self->CompleteSelf();
	return self;
	}

void CTzTestActiveWorker::RunL()
	{
	iTzTestStep.TestL();
	}


void CTzTestActiveWorker::Start()
	{
	CompleteSelf();
	}


void CTzTestActiveWorker::DoCancel()
	{
	// do nothing
	}

TInt CTzTestActiveWorker::RunError(TInt aError)
	{
	// 
	iTzTestStep.TestCompleted(aError);
	return KErrNone;
	}


//
//
void CTZUnitTestBase::PrepareTestEnvironmentL()
	{



	GetStringFromConfig(ConfigSection(), KZoneNameId, iStrZoneNameId);
	GetIntFromConfig(ConfigSection(), KZoneNoId, iNumZoneID);
	GetStringFromConfig(ConfigSection(), KUTCTime, iStrUTCTime);
	GetStringFromConfig(ConfigSection(), KLocalTime, iStrLocalTime);
	GetBoolFromConfig(ConfigSection(), KConvert, iConvertUTC);
	GetBoolFromConfig(ConfigSection(), KSetZone, iSetZone);

	// Convert 16bit Zone Id to 8 bit
	iZoneID8.Set(CnvUtfConverter::ConvertFromUnicodeToUtf8L(iStrZoneNameId)->Des());

	}


//
//
CConvertTime::~CConvertTime()
	{
	delete iTimeConverter;
	delete iWorker;
	iTimeZoneServer.Close();
	}

CConvertTime::CConvertTime(CDstUnitServer &aServer) :
							CTZUnitTestBase(aServer)
	{
	iTestCount = 2;
	SetTestStepName(KStepConvertTime);
	}


void CConvertTime::DestroyTestUnit()
	{
	delete iTimeConverter;
	iTimeConverter = NULL;
	delete iWorker;
	iWorker = NULL;
	iTimeZoneServer.Close();
	}


TInt CConvertTime::CreateAndRunTestUnitL()
	{
	User::LeaveIfError(iTimeZoneServer.Connect());
	iTimeConverter = CTzConverter::NewL(iTimeZoneServer);
	iWorker = CTzTestActiveWorker::NewL(*this);
	return KRequestPending;
	}

void CConvertTime::TestL()
	{
	TTime utcTime = ReadTimeParamStringL(iStrUTCTime);
	TTime localTime = ReadTimeParamStringL(iStrLocalTime);

	TTime time(0);
	TTime convertedTime(0);
	TTime compareTime(0);
	TDateTime dateTime;

	CTzId* timeZoneId = CTzId::NewL(iZoneID8);
	CleanupStack::PushL(timeZoneId);

	if (iSetZone)
		{

		// First set the time zone
		iTimeZoneServer.SetTimeZoneL(*timeZoneId);

		// now do local conversion
		if (iConvertUTC)
			{
			time = localTime;
			dateTime = time.DateTime();
			compareTime = utcTime;
			iTimeConverter->ConvertToUniversalTime(time);
			convertedTime = time;
			}
		else
			{
			time = utcTime;
			dateTime = time.DateTime();
			compareTime = localTime;
			iTimeConverter->ConvertToLocalTime(time);
			convertedTime = time;
			}
		}
	else
		{
		if (iConvertUTC)
			{
			time = localTime;
			dateTime = time.DateTime();
			compareTime = utcTime;
			iTimeConverter->ConvertToUniversalTime(time, *timeZoneId);
			convertedTime = time;
			}
		else
			{
			time = utcTime;
			dateTime = time.DateTime();
			compareTime = localTime;
			iTimeConverter->ConvertToLocalTime(time, *timeZoneId);
			convertedTime = time;
			}
		}

	// Set the time zone again through the direct connection to server and 
	// observe the notification to the converter
	iTimeZoneServer.SetTimeZoneL(*timeZoneId);

	CleanupStack::PopAndDestroy(timeZoneId);

	// for debug purposes only
#ifdef _DEBUG
	TDateTime convertedDateTime;
	TDateTime compareDateTime;
	convertedDateTime = convertedTime.DateTime();
	compareDateTime = compareTime.DateTime();
#endif

	if ((--iTestCount) <= 0)
		{
		TestCompleted((compareTime == convertedTime) ? KErrNone : KErrGeneral);
		}
	else
		{
		iWorker->Start();
		}
	}


//
//
CTZConfigAgentTest::~CTZConfigAgentTest()
	{
	delete iConfigAgent;
	delete iRepository;
	delete iWorker;
	delete iListener;
	delete iTzSwiObserver;
	iFileServ.Close();
	iTimeZoneServer.Close();
	}

CTZConfigAgentTest::CTZConfigAgentTest(CDstUnitServer &aServer) :
							CTZUnitTestBase(aServer), iExpectNotice(EFalse)
	{
	iTestCount = 2;
	SetTestStepName(KStepTZConfigAgent);
	}


void CTZConfigAgentTest::DestroyTestUnit()
	{
	delete iConfigAgent;
	iConfigAgent = NULL;
	delete iRepository;
	iRepository = NULL;
	delete iWorker;
	iWorker = NULL;
	delete iListener;
	iListener = NULL;
	delete iTzSwiObserver;
	iTzSwiObserver = NULL;
	iFileServ.Close();
	iTimeZoneServer.Close();
	}


TInt CTZConfigAgentTest::CreateAndRunTestUnitL()
	{
	User::LeaveIfError(iFileServ.Connect());
	User::LeaveIfError(iTimeZoneServer.Connect());
    
	// register file session for stressing
	SetFileSession(iFileServ);
	//iUserDataDb can't not be initialised because the test could not open the user defined database.
	iTzSwiObserver = CTzSwiObserver::NewL();
	iConfigAgent = CTzConfigAgent::NewL(*this, *iUserDataDb, *iTzSwiObserver);
	iConfigAgent->SetLocalizationDb(iTzLocalizationDb);
	iRepository = CRepository::NewL(KPropertyCategory);
	iWorker = CTzTestActiveWorker::NewL(*this);
	iListener = CUpdateListener::NewL();
    
	return KRequestPending;
	}

/**
Tests configuration of the UTC Offset auto-update functionality.

@return An error code signalling success or failure.
@internalComponent

*/
TInt CTZConfigAgentTest::TestAutoUpdateConfigurationSettingL()
    {
    // Get the current configuration from the Central Repository.
	iRepository->Get(KConfigurationKey, iOriginalConfig);

    // Toggle auto-update configuration.
    switch(iOriginalConfig)
    {
    	case RTz::ETZAutoDSTUpdateOff:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
    	 	break;
    	case RTz::ETZAutoDSTUpdateOn:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOff);
    		break;
    	default:
    		break;
    };
    //iTimeZoneServer.SetAutoUpdateBehaviorL(!iOriginalConfig);
          
    // Get the new configuration from the Central Repository.
    TBool newConfig = EFalse;
	iRepository->Get(KConfigurationKey, newConfig);
    
    // Test that the configuration has changed.
    if (newConfig == iOriginalConfig)
        {
        return KErrGeneral;
        }
        
     // Set auto-update configuration back to original setting.
    switch(iOriginalConfig)
    {
    	case RTz::ETZAutoDSTUpdateOff:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOff);
    		break;
    	case RTz::ETZAutoDSTUpdateOn:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
    		break;
    	default:
    		break;
    };
    //iTimeZoneServer.SetAutoUpdateBehaviorL(iOriginalConfig);
    
    // Get the new configuration from the Central Repository.
	iRepository->Get(KConfigurationKey, newConfig);
    
    // Test that the configuration has changed.
    if (newConfig != iOriginalConfig)
        {
        return KErrGeneral;
        }
       
    // Test that we can't set the configuration in the Central Repository directly.
    TInt err = iRepository->Set(KConfigurationKey, newConfig);
    
    if ((err != KErrPermissionDenied) && (err != KErrAccessDenied))
        {
        return err;
        }
    
    return KErrNone;
    }
/**
Tests the UTC Offset auto-update and retrieval functionality.

@return An error code signalling success or failure.
@internalComponent

*/
TInt CTZConfigAgentTest::TestAutoUpdateAndRetrievalL()
    {
    // Get the current configuration from the Central Repository.
	iRepository->Get(KConfigurationKey, iOriginalConfig);
	TInt result;
    switch(iOriginalConfig)
    {
    	case RTz::ETZAutoDSTUpdateOff:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
    	  	result = iTimeZoneServer.AutoUpdateSettingL();
    	 	test(result == RTz::ETZAutoDSTUpdateOn);
    	 	break;
    	case RTz::ETZAutoDSTUpdateOn:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOff);
       	 	result = iTimeZoneServer.AutoUpdateSettingL();
	  	 	test(result==RTz::ETZAutoDSTUpdateOff);
  			break;
    	default:
    		break;
    };
    return KErrNone;
    }

/**
Tests the UTC Offset auto-update and notification functionality.

@return An error code signalling success or failure.
@internalComponent

*/
TInt CTZConfigAgentTest::TestAutoUpdateAndNotificationL()
    {
    // Note the current time zone.
    CTzId* originalId = iTimeZoneServer.GetTimeZoneIdL();
    CleanupStack::PushL(originalId);
    
    // Set the time zone to London for this test.
    _LIT8(KTzIdEuropeLondon, "Europe/London");

	CTzId* londonTimeZoneId = CTzId::NewL(KTzIdEuropeLondon());
	CleanupStack::PushL(londonTimeZoneId);

    iTimeZoneServer.SetTimeZoneL(*londonTimeZoneId);

     // Also ensure that iConfigAgent is set to the same timezone for the calculation below.
    iConfigAgent->SetTimeZoneL(*londonTimeZoneId, static_cast<const TAny*>(this), ETrue);
	CleanupStack::PopAndDestroy(londonTimeZoneId);
    
    // Turn auto-update on.
    iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
    
   // Calculate when the next DST change will take effect.
    TTime nextDstEventUtc; // In UTC.

    if (!iConfigAgent->CalculateNextDstChangeL(nextDstEventUtc))
        {
        return KErrNotFound;    // No DST change expected!
        }

    // Note the current UTC Offset.
    const TTimeIntervalSeconds utcOffset = User::UTCOffset();
    
    // Note the current system time.
    TTime nowUtc;   // In UTC.
    nowUtc.UniversalTime();
    
    // Log time before change.
    _LIT(KBeforeChange, "Before time change");
    LogTimeLocalL(nowUtc + utcOffset, KBeforeChange);
    
    // Set the system time to ten seconds before the next DST change event.
    const TTimeIntervalSeconds timeBeforeDstEvent(10);
    TTime newTimeLocal = nextDstEventUtc - timeBeforeDstEvent + utcOffset;   // Local time.
    TInt err = iTimeZoneServer.SetHomeTime(newTimeLocal);   // Local time.

    if (err != KErrNone)    // An error occurred in setting the time.
        {
        return err;
        }

    // Log time after change.
    _LIT(KAfterChange, "After time change (waiting for DST event...)");
    LogTimeLocalL(newTimeLocal, KAfterChange);
    
    // Start a Publish and Subscribe listener object and wait for P&S
    // notification, checking that the UTC Offset has been updated.
    // (This should wait for around timeBeforeDstEvent seconds.)
    if (!iListener->SubscribeAndWait())
        {
        return KErrGeneral;
        }
        
    // Log time after auto-update.
    TTime nowLocal;
    nowLocal.HomeTime();
    _LIT(KAfterDst, "After DST event");
    LogTimeLocalL(nowLocal, KAfterDst);

    // Get the new UTC Offset in order to calculate the "real" time.
    const TTimeIntervalSeconds newUtcOffset = User::UTCOffset();
    
    // Calculate the time difference since we set newLocalTime 
    // (note that this includes the new UTC Offset).
    TTimeIntervalMicroSeconds deltaTime = nowLocal.MicroSecondsFrom(newTimeLocal);

    // Reset the system time.
    nowLocal = nowUtc + utcOffset;  // Local time.
    nowLocal += deltaTime;          // Add in any time difference due to processing/waiting.
    nowLocal -= newUtcOffset;       // Remove the new UTC Offset implicit in deltaTime.

    err = iTimeZoneServer.SetHomeTime(nowLocal);   // Local time.

    if (err != KErrNone)    // An error occurred in setting the time.
        {
        return err;
        }
        
    // Log time after changing back.
    _LIT(KAfterReset, "After time reset");
    LogTimeLocalL(nowLocal, KAfterReset);

    // Reset the original configuration.
     switch(iOriginalConfig)
    {
    	case RTz::ETZAutoDSTUpdateOff:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOff);
    		break;
    	case RTz::ETZAutoDSTUpdateOn:
    		iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
    		break;
    	default:
    		break;
    };

    //iTimeZoneServer.SetAutoUpdateBehaviorL(iOriginalConfig);
    
    // Reset the time zone.
    iTimeZoneServer.SetTimeZoneL(*originalId);
    CleanupStack::PopAndDestroy(originalId);
    
    return KErrNone;
    }

/**
Adds the given local time to the log file.

@param aTime The time to log (in local time).
@param aDes A user message to add to the log string.
@internalComponent

*/
void CTZConfigAgentTest::LogTimeLocalL(const TTime& aTime, const TDesC& aDes)
    {
    // Format time.
    _LIT(KTimeFormat, "%-B%:0%J%:1%T%:3%+B");
    TBuf<KMaxTimeFormatSpec> time;
    aTime.FormatL(time, KTimeFormat);
    
    // Format date.
    TBuf<KMaxShortDateFormatSpec> date;
    aTime.FormatL(date, TShortDateFormatSpec());
    
    // Log time.
    _LIT(KTime, "%S - system time is now: %S %S");
    INFO_PRINTF4(KTime(), &aDes, &time, &date);
    }

void CTZConfigAgentTest::TestL()
	{
	_LIT8(KTzIdEuropeLondon, "Europe/London");
	_LIT8(KTzIdEuropeParis, "Europe/Paris");

	CTzId* londonTimeZoneId = CTzId::NewL(KTzIdEuropeLondon());
	CleanupStack::PushL(londonTimeZoneId);

	CTzId* parisTimeZoneId = CTzId::NewL(KTzIdEuropeParis());
	CleanupStack::PushL(parisTimeZoneId);

	CTzId* timeZoneId = CTzId::NewL(iZoneID8);
	CleanupStack::PushL(timeZoneId);

	// Set condition for receiving Time Zone Change Notification
	iConfigAgent->SetTimeZoneL(*londonTimeZoneId, static_cast<const TAny*>(this), ETrue);
	iExpectNotice = ETrue;
	iConfigAgent->SetTimeZoneL(*parisTimeZoneId, static_cast<const TAny*>(this), ETrue);

	// Check Numeric/Name ID Matching
	const CTzId& newTimeZoneId = iConfigAgent->SetTimeZoneL(*timeZoneId, static_cast<const TAny*>(this), ETrue);
	TInt numZoneId = newTimeZoneId.TimeZoneNumericID();

	_LIT(KNumericZoneNoCheck, "Zone Number");
	LogCheck(KNumericZoneNoCheck(), ((iNumZoneID == numZoneId) ? KErrNone : KErrCorrupt));

	CleanupStack::PopAndDestroy(3, londonTimeZoneId);

    // Test configuration setting works.
	_LIT(KConfigurationCheck, "Configuration check");
	LogCheck(KConfigurationCheck(), TestAutoUpdateConfigurationSettingL());

	//Test auto-update and retrieval
	_LIT(KAutoUpdateAndRetrievalCheck, "Auto-update and retrieval check");
	LogCheck(KAutoUpdateAndRetrievalCheck(), TestAutoUpdateAndRetrievalL());

    // Test auto-update and notification.
	_LIT(KAutoUpdateAndNotificationCheck, "Auto-update and notification check");
	LogCheck(KAutoUpdateAndNotificationCheck(), TestAutoUpdateAndNotificationL());
    
	if ((--iTestCount) <= 0)
		{
		// Check Time Zone Change Notification
		_LIT(KNotifications, "Received Notifications");
		LogCheck(KNotifications(), (iNotifications != 0) ? KErrNone : KErrNotFound); 
		TestCompleted(KErrNone);
		}
	else
		{
		iWorker->Start();
		}

	}

void CTZConfigAgentTest::NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aChanger)
	{
	_LIT(KTzNotification, "Tz Change Notified");

	if (iExpectNotice)
		{
		// notice is sometimes sent during the contruction of the TZ Config Agent. 
		// we are not interested in that one
		iExpectNotice = EFalse;
		++iNotifications;
		LogCheck(KTzNotification(), 
				(aChanger == static_cast<const CTZConfigAgentTest*>(this)) ?
				KErrNone : KErrBadHandle);

		LogCheck(KTzNotification(), ((aChange == RTz::ETZSystemTimeZoneChanged) ?
				KErrNone : KErrArgument));
		}
	}

//
// CUpdateListener
// Listens for UTC Offset update notification from the Publish
// and Subscribe API.


/**
Destructor.

@internalComponent

*/
CUpdateListener::~CUpdateListener()
    {
    Cancel();
    iProperty.Close();  // Not actually needed, but call it anyway in case this changes in the future.
    }

/**
Constructor.

@internalComponent

*/
CUpdateListener::CUpdateListener()
 :  CActive(CActive::EPriorityStandard)
    {
    }

/**
Factory constructor.

@return The newly constructed class.
@leave The error code from RProperty::Attach().
@internalComponent

*/
CUpdateListener* CUpdateListener::NewL()
    {
    CUpdateListener* self = new (ELeave) CUpdateListener;
    CleanupStack::PushL(self);
    
    CActiveScheduler::Add(self);

    TInt err = self->iProperty.Attach(
        NTzUpdate::KPropertyCategory,
        NTzUpdate::EUtcOffsetChangeNotification,
        EOwnerThread);

    User::LeaveIfError(err);

    CleanupStack::Pop(self);
    return self;
    }

/**
Overridden RunL() method. Tests the new UTC Offset on notification from Publish
and Subscribe and sets the "offset changed?" flag accordingly. The re-entrant
Active Scheduler loop is stopped and the flow of control returns to the
CActiveScheduler::Start() call in SubscribeAndWait().

@internalComponent

*/
void CUpdateListener::RunL()
    {
    // Test if UTC Offset has changed.
    TTimeIntervalSeconds utcOffset = User::UTCOffset();

    // Set the "offset changed?" flag.    
    iOffsetChanged = (utcOffset != iUtcOffset);
    
    // Stop the re-entrant loop.
    CActiveScheduler::Stop();
    }

/**
Method to start the Active Object. The current UTC Offset is stored for later
comparison and a request made to Publish and Subscribe for UTC Offset updates.
The Active Scheduler is re-entered so the function will wait until the
corresponding CActiveScheduler::Stop() is called.

@return Whether the UTC Offset has changed after notification.
@internalComponent

*/
TBool CUpdateListener::SubscribeAndWait()
    {
    // Reset the "offset changed?" flag.
    iOffsetChanged = EFalse;
    
    // Get current UTC Offset.
    iUtcOffset = User::UTCOffset();
    
    // Subscribe to update notifications.
    iProperty.Subscribe(iStatus);
    SetActive();
    
    // Re-enter the Active Scheduler, waiting until notification occurs.
    CActiveScheduler::Start();
    
    // After the re-entrant loop has stopped return the saved flag value.
    return iOffsetChanged;
    }

/**
Overridden DoCancel() method. Cancels the Publish and Subscribe request and
stops the re-entrant Active Scheduler loop under cancel conditions - the flow
of control returns to the CActiveScheduler::Start() call in SubscribeAndWait().

@internalComponent

*/
void CUpdateListener::DoCancel()
    {
    iProperty.Cancel();

    // Stop the re-entrant loop.
    CActiveScheduler::Stop();
    }
 
//
//
// CGetOffsetsTest: gets the offsets for an array of numeric TZ IDs, some of which 
// do not exist in the TZ databasee, and tests that this situation is handled
// gracefully.
//
_LIT(KId1,"ID1");
_LIT(KId2,"ID2");
_LIT(KId3,"ID3");
_LIT(KId4,"ID4");

_LIT(KOffset1,"Offset1");
_LIT(KOffset2,"Offset2");
_LIT(KOffset3,"Offset3");
_LIT(KOffset4,"Offset4");

_LIT(KUserDefinedId, "Userdefined");

CGetOffsetsTest::~CGetOffsetsTest()
	{
	iTimeZoneServer.Close();
	}

CGetOffsetsTest::CGetOffsetsTest(CDstUnitServer &aServer) : CDstUnitStep(aServer)
	{
	SetTestStepName(KStepGetOffsets);
	}

void CGetOffsetsTest::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}

TInt CGetOffsetsTest::CreateAndRunTestUnitL()
	{
	TInt result = KErrNone;
	User::LeaveIfError(iTimeZoneServer.Connect());
	
	result = TestGetOffsetsL();

 	return result;
	}

TInt CGetOffsetsTest::TestGetOffsetsL()
	{
	//Read userdefined field from ini file
	TInt userDefined;
	GetIntFromConfig(ConfigSection(),KUserDefinedId, userDefined);
	if(userDefined)
		{
		//Run only the test that we want to, according to whether or not DST is on or not
		CTzId* currentTimeZone = iTimeZoneServer.GetTimeZoneIdL();
		CleanupStack::PushL(currentTimeZone);
		TBool DSTon = iTimeZoneServer.IsDaylightSavingOnL(*currentTimeZone);
		CleanupStack::PopAndDestroy(currentTimeZone);
		if(DSTon)
			{
			if(userDefined == KUserDefinedDSTOffId)
				{
				return KErrNone;
				}
			}
		else
			{
			if(userDefined == KUserDefinedDSTOnId)
				{
				return KErrNone;
				}
			}
			
		CreateUserDefinedTzDatabaseL();
		}
		
	//
	// Read numeric IDs from ini file and add them to array
	//
	RArray<TInt> tzIds;
	TInt numericId;
	GetIntFromConfig(ConfigSection(),KId1,numericId);
	tzIds.Append(numericId);
	GetIntFromConfig(ConfigSection(),KId2,numericId);
	tzIds.Append(numericId);
	GetIntFromConfig(ConfigSection(),KId3,numericId);
	tzIds.Append(numericId);
	GetIntFromConfig(ConfigSection(),KId4,numericId);
	tzIds.Append(numericId);
	
	//
	// Read expected offsets from ini file and add them to array
	//
	RArray<TInt> expOffsets;
	TInt expOffset;
	GetIntFromConfig(ConfigSection(),KOffset1,expOffset);
	expOffsets.Append(expOffset);
	GetIntFromConfig(ConfigSection(),KOffset2,expOffset);
	expOffsets.Append(expOffset);
	GetIntFromConfig(ConfigSection(),KOffset3,expOffset);
	expOffsets.Append(expOffset);
	GetIntFromConfig(ConfigSection(),KOffset4,expOffset);
	expOffsets.Append(expOffset);
	
	
	//
	// Retrieve offsets from TZ server
	//
	RArray<TInt> offsets;
	
	TRAPD(err,iTimeZoneServer.GetOffsetsForTimeZoneIdsL(tzIds,offsets));
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("FAILED: RTz::GetOffsetsForTimeZoneIdsL left with error code %D"),&err);
		return err;
		}
	
	//
	// Validate retrieved offsets against expected ones
	//
	
	TInt expCount = expOffsets.Count();
	TInt gotCount = offsets.Count();
	
	if (expCount != gotCount)
		{
		ERR_PRINTF3(_L("FAILED: Expected %D offsets, Got %D offsets"), &expCount, &gotCount);
		return KErrGeneral;
		}
		
	for (TInt i = 0; i < expCount; ++i)
		{
		if (offsets[i] != expOffsets[i])
			{
			ERR_PRINTF5(_L("FAILED: Numeric ID = %D, Expected offset = %D, Retrieved offset = %D, Internal Step = %D"), tzIds[i], expOffsets[i], offsets[i], i);
			return KErrGeneral;
			}
		}
	
	return KErrNone;
	}
	
void CGetOffsetsTest::CreateUserDefinedTzDatabaseL()	
	{
	CTzUserData* userData = CTzUserData::NewL(iTimeZoneServer);
	CleanupStack::PushL(userData);
	
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);

	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);
	
	TBuf<20> stdName;
	stdName.Format(_L("StdName1"));	
	TBuf<20> shortStdName;
	shortStdName.Format(_L("ShortStd1"));	
	TBuf<20> dstName;
	dstName.Format(_L("DstName1"));	
	TBuf<20> shortDstName;
	shortDstName.Format(_L("ShortDst1"));	
	TBuf<20> cityName;
	cityName.Format(_L("CityName1"));	
	TBuf<20> regionName;
	regionName.Format(_L("RegionName1"));
	
	CTzUserNames* names = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);
	
	CTzId* id = NULL;
	id = userData->CreateL(*rules, *names);

	CleanupStack::PopAndDestroy(2, rules);
	
	TTzRule rule3(1981,1996,0,60,EMarch,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	TTzRule rule4(1981,1996,60,0,EOctober,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	
	rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule3);
	rules->AddRuleL(rule4);

	stdName.Format(_L("StdName2"));	
	shortStdName.Format(_L("ShortStd2"));	
	dstName.Format(_L("DstName2"));	
	shortDstName.Format(_L("ShortDst2"));	
	cityName.Format(_L("CityName2"));	
	regionName.Format(_L("RegionName2"));
	
	names = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);

	id  = userData->CreateL(*rules, *names);
	
	CleanupStack::PopAndDestroy(2, rules);
		
	TTzRule rule5(2001,9999,120,60,EMarch,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	TTzRule rule6(1981,1996,60,120,EOctober,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	
	rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule5);
	rules->AddRuleL(rule6);

	stdName.Format(_L("StdName3"));	
	shortStdName.Format(_L("ShortStd3"));	
	dstName.Format(_L("DstName3"));	
	shortDstName.Format(_L("ShortDst3"));	
	cityName.Format(_L("CityName3"));	
	regionName.Format(_L("RegionName3"));
	
	names = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);

	id  = userData->CreateL(*rules, *names);

	CleanupStack::PopAndDestroy(2, rules);
			
	TTzRule rule7(1981,2010,0,60,EMarch,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	TTzRule rule8(1981,2010,60,0,EOctober,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	
	rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule7);
	rules->AddRuleL(rule8);

	stdName.Format(_L("StdName4"));	
	shortStdName.Format(_L("ShortStd4"));	
	dstName.Format(_L("DstName4"));	
	shortDstName.Format(_L("ShortDst4"));	
	cityName.Format(_L("CityName4"));	
	regionName.Format(_L("RegionName4"));
	
	names = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);

	id  = userData->CreateL(*rules, *names);
	
	delete id;
	CleanupStack::PopAndDestroy(2, rules);
	CleanupStack::PopAndDestroy(userData);
	}

//
