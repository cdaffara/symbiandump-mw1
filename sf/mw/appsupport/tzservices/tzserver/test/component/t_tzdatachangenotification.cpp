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


#include <e32base.h>
#include <e32test.h>
#include <e32property.h>
#include <tz.h>
#include <tzupdate.h>
#include "testserver.h"


_LIT(KSrcTzRulesDb, "z:\\testresourcefiles\\tzdb.dbz");
_LIT(KDestTzRulesDb, "c:\\private\\1020383e\\tzdb.dbz");

_LIT(KSrcTzUserDb, "z:\\testresourcefiles\\tz_userdata_empty.db");
_LIT(KDestTzUserDb, "c:\\private\\1020383e\\SQLite__tzuserdata.db");

_LIT(KSrcTzLocRscDir, "z:\\testresourcefiles\\");
_LIT(KDestTzLocRscDir, "c:\\resource\\timezonelocalization\\");

_LIT(KTimeStampFile, "c:\\private\\1020383e\\timestamps");

_LIT(KSrcTzLocRscFile1, "z:\\testresourcefiles\\timezonegroups.r01");
_LIT(KSrcTzLocRscFile2, "z:\\testresourcefiles\\timezonegroups.r02");
_LIT(KSrcTzLocRscFile3, "z:\\testresourcefiles\\timezonegroups.rsc");
_LIT(KSrcTzLocRscFile4, "z:\\testresourcefiles\\timezones.r01");
_LIT(KSrcTzLocRscFile5, "z:\\testresourcefiles\\timezones.r02");
_LIT(KSrcTzLocRscFile6, "z:\\testresourcefiles\\timezones.rsc");

_LIT(KDestTzLocRscFile1, "c:\\resource\\timezonelocalization\\timezonegroups.r01");
_LIT(KDestTzLocRscFile2, "c:\\resource\\timezonelocalization\\timezonegroups.r02");
_LIT(KDestTzLocRscFile3, "c:\\resource\\timezonelocalization\\timezonegroups.rsc");
_LIT(KDestTzLocRscFile4, "c:\\resource\\timezonelocalization\\timezones.r01");
_LIT(KDestTzLocRscFile5, "c:\\resource\\timezonelocalization\\timezones.r02");
_LIT(KDestTzLocRscFile6, "c:\\resource\\timezonelocalization\\timezones.rsc");


RTest test(_L("TZ Data Change Notification Test Suite"));


/**
Timer used to detect if a test case times out.
*/
class CTestTimeOut : public CTimer
	{
public:
    static CTestTimeOut* NewL();
	
	void Start(TInt aTimeOut);
	void Stop();
	TBool TimedOut();
	
private:
	CTestTimeOut();
	void ConstructL();

    // From CActive.
    void RunL();

private:
	TBool iTimedOut;
	};


CTestTimeOut* CTestTimeOut::NewL()
    {
    CTestTimeOut* self = new(ELeave) CTestTimeOut();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CTestTimeOut::CTestTimeOut()
	:
	CTimer(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	iTimedOut = EFalse;
	}


void CTestTimeOut::ConstructL()
	{
	CTimer::ConstructL();
	}
	
	
void CTestTimeOut::Start(TInt aTimeOut)
	{
	iTimedOut = EFalse;
	Cancel();
	After(aTimeOut);
	}

void CTestTimeOut::Stop()
	{
	Cancel();
	}

TBool CTestTimeOut::TimedOut()
	{
	return iTimedOut;
	}

	
void CTestTimeOut::RunL()
	{
	iTimedOut = ETrue;
	CActiveScheduler::Stop();
	}
		

/**
Watches for a change to the given NTzUpdate::TPropertyKeys property.  The
property key should only be NTzUpdate::ETzRulesChange or NTzUpdate::ETzNames-
Change.
*/
class CTzDataChangeObserver : public CActive
    {
public:
    static CTzDataChangeObserver* NewL(NTzUpdate::TPropertyKeys
    	aChangePropertyKey);
    ~CTzDataChangeObserver();

	void Start();
	void Stop();
    TTime TimeOfChange();
    
private:
    // From CActive.
    void RunL();
    void DoCancel();
	
    CTzDataChangeObserver(NTzUpdate::TPropertyKeys aChangePropertyKey);
    void ConstructL();

    void RespondToChange();
	
private:
    NTzUpdate::TPropertyKeys iChangePropertyKey;
    RProperty iChangeProperty;
    TTime iTimeOfChange;
    };


CTzDataChangeObserver* CTzDataChangeObserver::NewL(NTzUpdate::TPropertyKeys
	aChangePropertyKey)
    {
    CTzDataChangeObserver* self =
    	new(ELeave) CTzDataChangeObserver(aChangePropertyKey);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CTzDataChangeObserver::CTzDataChangeObserver(NTzUpdate::TPropertyKeys
	aChangePropertyKey)
    :
    CActive(CActive::EPriorityStandard),
    iChangePropertyKey(aChangePropertyKey)
    {
    CActiveScheduler::Add(this);
    iTimeOfChange = Time::NullTTime();
    }


void CTzDataChangeObserver::ConstructL()
    {
    User::LeaveIfError(iChangeProperty.Attach(NTzUpdate::KPropertyCategory,
    	iChangePropertyKey));
    }


CTzDataChangeObserver::~CTzDataChangeObserver()
    {
    Cancel();    
    iChangeProperty.Close();
    }


void CTzDataChangeObserver::Start()
    {
    iChangeProperty.Subscribe(iStatus);
	SetActive();
	}


void CTzDataChangeObserver::Stop()
    {
	Cancel();
	}


void CTzDataChangeObserver::RespondToChange()
    {
    TPckgBuf<TTime> changeBuf;
    TInt err = iChangeProperty.Get(changeBuf);
    if (err == KErrNone)
        {
        iTimeOfChange = changeBuf();
        }
    }


TTime CTzDataChangeObserver::TimeOfChange()
    {
    return iTimeOfChange;
    }


void CTzDataChangeObserver::RunL()
    {	
    if (iStatus.Int() == KErrNone)
        {
		RespondToChange();
		CActiveScheduler::Stop();
        }
    }


void CTzDataChangeObserver::DoCancel()
    {
    iChangeProperty.Cancel();
    }


/**
Base class for TZ data change notification test cases.
*/
class CTzDataChangeNotificationTestCase : public CBase
	{
public:
    ~CTzDataChangeNotificationTestCase();

	void TestChangeNotificationL();
	
protected:
    CTzDataChangeNotificationTestCase();
    void ConstructL(NTzUpdate::TPropertyKeys aChangePropertyKey);

private:
    	    	
	void TearDownL();

	void PrintTime(const TDesC& aDesc, const TTime& aTime);
	virtual void SetUpL();
	virtual void TriggerChangeNotificationL() = 0;
	virtual void TidyUpAfterChangeNotificationL() = 0;

protected:
	RTz iTzServer;
	RPIMTestServer iTestServer;
	
private:
	CTestTimeOut* iTestTimeOut;
	CTzDataChangeObserver* iTzDataChangeObserver;
	};


CTzDataChangeNotificationTestCase::CTzDataChangeNotificationTestCase()
    {
	// Nothing to do.
    }


void CTzDataChangeNotificationTestCase::ConstructL(NTzUpdate::TPropertyKeys
	aChangePropertyKey)
    {
	iTestTimeOut = CTestTimeOut::NewL();
	iTzDataChangeObserver = CTzDataChangeObserver::NewL(aChangePropertyKey);
    }


CTzDataChangeNotificationTestCase::~CTzDataChangeNotificationTestCase()
    {
	delete iTestTimeOut;
	delete iTzDataChangeObserver;
    }


void CTzDataChangeNotificationTestCase::SetUpL()
	{
	User::LeaveIfError(iTzServer.Connect());
	User::LeaveIfError(iTestServer.Connect());
	}


void CTzDataChangeNotificationTestCase::TearDownL()
	{
	iTestServer.Close();
	iTzServer.Close();
	}


void CTzDataChangeNotificationTestCase::PrintTime(const TDesC& aDesc,
	const TTime& aTime)
	{
	TDateTime dt = aTime.DateTime();
	test.Printf(_L("%S: %02d:%02d:%02d:%06d\n"), &aDesc, dt.Hour(), dt.Minute(),
		dt.Second(), dt.MicroSecond());
	}
	
	
void CTzDataChangeNotificationTestCase::TestChangeNotificationL()
	{
	SetUpL();

	TTime timeBeforeChangeNotification;
	timeBeforeChangeNotification.UniversalTime();

	_LIT(KTimeBeforeChangeNotification, "Time before change notification");
	PrintTime(KTimeBeforeChangeNotification, timeBeforeChangeNotification);

	iTzDataChangeObserver->Start();

	TriggerChangeNotificationL();
	
	const TInt KTestTimeOut = 50000000 /* microseconds */;
	iTestTimeOut->Start(KTestTimeOut);

	// The thread will be blocked after this call until either the test times
	// out or the change in the observed property occurs. 
	CActiveScheduler::Start();

	iTzDataChangeObserver->Stop();
	iTestTimeOut->Stop();
	
	TidyUpAfterChangeNotificationL();

	test(iTestTimeOut->TimedOut() == EFalse);

	TTime timeOfPropertyChange = iTzDataChangeObserver->TimeOfChange();
	_LIT(KTimeOfPropertyChange, "Time of property change");
	PrintTime(KTimeOfPropertyChange, timeOfPropertyChange);

	// We have received a change to the property - sanity check that the time
	// of the property change is later than the time of the event which
	// initiated the change.
	test(timeOfPropertyChange > timeBeforeChangeNotification);

	TearDownL();
	}
	
	
/**
@SYMTestCaseID 	PIM-APPSERV-TZ-TZS-CN-0001
	
@SYMTestCaseDesc
	The purpose of this test is to verify that a change to the system TZ rules
	database causes a change in value of the TZ rules change property.

@SYMTestActions
	1. Subscribe to the TZ rules change property.
	2. Copy a system TZ rules database to the appropriate location on the C:
	   drive.

@SYMTestExpectedResults
	The TZ rules change property changes value.
	
@SYMREQ
	REQ9950, REQ9951

@SYMTestType
	CT

@SYMTestPriority
	1
*/
class CTestSysTzRulesDbChangeNotification : public CTzDataChangeNotificationTestCase
	{
public:
    static CTestSysTzRulesDbChangeNotification* NewL();
    ~CTestSysTzRulesDbChangeNotification();

protected:
	CTestSysTzRulesDbChangeNotification();
	void ConstructL();

private:
	void TriggerChangeNotificationL();
	void TidyUpAfterChangeNotificationL();
	};


CTestSysTzRulesDbChangeNotification* CTestSysTzRulesDbChangeNotification::NewL()
    {
    CTestSysTzRulesDbChangeNotification* self =
    	new(ELeave) CTestSysTzRulesDbChangeNotification();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CTestSysTzRulesDbChangeNotification::CTestSysTzRulesDbChangeNotification()
	{
	// Nothing to do.
	}


void CTestSysTzRulesDbChangeNotification::ConstructL()
    {
	CTzDataChangeNotificationTestCase::ConstructL(NTzUpdate::ETzRulesChange);
	}


CTestSysTzRulesDbChangeNotification::~CTestSysTzRulesDbChangeNotification()
	{
	// Nothing to do.
	}


void CTestSysTzRulesDbChangeNotification::TriggerChangeNotificationL()
	{
	// By copying the TZ rules database to the C: drive a change in the
	// NTzUpdate::ETzRulesChange property should occur. 
	iTestServer.CopyFileL(KSrcTzRulesDb, KDestTzRulesDb);
	iTzServer.SwiObsBeginL();
	iTzServer.SwiObsFileChangedL(RTz::EFilterTzPrivate);
	iTzServer.SwiObsEndL();
	}


void CTestSysTzRulesDbChangeNotification::TidyUpAfterChangeNotificationL()
	{
	iTestServer.DeleteFileL(KDestTzRulesDb);
	}


/**
@SYMTestCaseID 	PIM-APPSERV-TZ-TZS-CN-0002
	
@SYMTestCaseDesc
	The purpose of this test is to verify that a change to the system TZ
	localization resources causes a change in value of the TZ names change
	property.
	
@SYMTestActions
	1. Subscribe to the TZ names change property.
	2. Copy system TZ localization resources to the appropriate location on the
	   C: drive.

@SYMTestExpectedResults
	The TZ names change property changes value.
	
@SYMREQ
	REQ9950, REQ9951

@SYMTestType
	CT

@SYMTestPriority
	1
*/
class CTestSysTzLocRscChangeNotification : public CTzDataChangeNotificationTestCase
	{
public:
    static CTestSysTzLocRscChangeNotification* NewL();
    ~CTestSysTzLocRscChangeNotification();

protected:
	CTestSysTzLocRscChangeNotification();
	void ConstructL();

private:
	void TriggerChangeNotificationL();
	void TidyUpAfterChangeNotificationL();
	};


CTestSysTzLocRscChangeNotification* CTestSysTzLocRscChangeNotification::NewL()
    {
    CTestSysTzLocRscChangeNotification* self =
    	new(ELeave) CTestSysTzLocRscChangeNotification();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CTestSysTzLocRscChangeNotification::CTestSysTzLocRscChangeNotification()
	{
	// Nothing to do.
	}


void CTestSysTzLocRscChangeNotification::ConstructL()
    {
	CTzDataChangeNotificationTestCase::ConstructL(NTzUpdate::ETzNamesChange);
	}


CTestSysTzLocRscChangeNotification::~CTestSysTzLocRscChangeNotification()
	{
	// Nothing to do.
	}


void CTestSysTzLocRscChangeNotification::TriggerChangeNotificationL()
	{
	// By copying the TZ localization resources to the C: drive a change in the
	// NTzUpdate::ETzNamesChange property should occur. 
	iTestServer.CopyFileL(KSrcTzLocRscFile1, KDestTzLocRscFile1);
	iTestServer.CopyFileL(KSrcTzLocRscFile2, KDestTzLocRscFile2);
	iTestServer.CopyFileL(KSrcTzLocRscFile3, KDestTzLocRscFile3);
	iTestServer.CopyFileL(KSrcTzLocRscFile4, KDestTzLocRscFile4);
	iTestServer.CopyFileL(KSrcTzLocRscFile5, KDestTzLocRscFile5);
	iTestServer.CopyFileL(KSrcTzLocRscFile6, KDestTzLocRscFile6);
	iTzServer.SwiObsBeginL();
	iTzServer.SwiObsFileChangedL(RTz::EFilterResourceTimezonelocalization);
	iTzServer.SwiObsEndL();
	}


void CTestSysTzLocRscChangeNotification::TidyUpAfterChangeNotificationL()
	{
	//Close the server, otherwise can't delete the resource file
	iTestServer.CloseTzSession();
	iTzServer.Close();
	iTestServer.DeleteFileL(KDestTzLocRscFile1);
	iTestServer.DeleteFileL(KDestTzLocRscFile2);
	iTestServer.DeleteFileL(KDestTzLocRscFile3);
	iTestServer.DeleteFileL(KDestTzLocRscFile4);
	iTestServer.DeleteFileL(KDestTzLocRscFile5);
	iTestServer.DeleteFileL(KDestTzLocRscFile6);
	iTestServer.DeleteDirL(KDestTzLocRscDir);
	}

/* This class wraps test cases that a change to the system TZ DB, the user defined
 * DB or the syststem TZ location resource files during the TZ server is disconnected.
 * When such a change happened, the TZ server should publish NTzUpdate::ETzRulesChange or
 * NTzUpdate::ETzNamesChange or both depending on the nature of the change.
 */
class CTestTzServerStartChangeNotification : public CTzDataChangeNotificationTestCase
	{
private:
	enum TFileModification
		{
		EInstalled,
		EUninstalled
		};
public:

    static CTestTzServerStartChangeNotification* NewL(NTzUpdate::TPropertyKeys aChangePropertyKey, const TDesC& aFileCopySrc, const TDesC& aFileCopyDest);
    static CTestTzServerStartChangeNotification* NewL(NTzUpdate::TPropertyKeys aChangePropertyKey, const TDesC& aFileDelete);
     ~CTestTzServerStartChangeNotification();
     void SetUpL();
protected:
	CTestTzServerStartChangeNotification(NTzUpdate::TPropertyKeys aChangePropertyKey, TFileModification aFileModification);
	void ConstructL(const TDesC& aFileCopySrc, const TDesC& aFileCopyDest);

private:

	void TriggerChangeNotificationL();
	void TidyUpAfterChangeNotificationL();
	
	NTzUpdate::TPropertyKeys iPropertyKeys; //an expected pub&sub key to be published by the TZ server
	HBufC* iFileSource;	//a file path to copy from
	HBufC* iFileDestination; //a file path to copy to
	TFileModification iFileModification;//Flag indicates whether it tests file has been installed or uninstalled.
	};

/** This is for the test case that a file (files) is (are) installed (copied) to C: drice
@param aChangePropertyKey an expected Pub&Sub propery key which will be published when the server is connected.
@param aFileCopySrc a file path to copy from
@param aFileCopyDest a file path to copy to
*/
CTestTzServerStartChangeNotification* CTestTzServerStartChangeNotification::NewL(NTzUpdate::TPropertyKeys aChangePropertyKey, const TDesC& aFileCopySrc, const TDesC& aFileCopyDest)
    {
    CTestTzServerStartChangeNotification* self =
    	new(ELeave) CTestTzServerStartChangeNotification(aChangePropertyKey, EInstalled);
    CleanupStack::PushL(self);
    self->ConstructL(aFileCopySrc, aFileCopyDest);
    CleanupStack::Pop(self);
    return self;
    }

/** This is for the test case that a file (files) is(are) uninstalled (deleted) from C: drive
@param aChangePropertyKey an expected Pub&Sub propery key which will be published when the server is connected.
@param aFileDelete a file path to delete
*/
CTestTzServerStartChangeNotification* CTestTzServerStartChangeNotification::NewL(NTzUpdate::TPropertyKeys	aChangePropertyKey, const TDesC& aFileDelete)
    {
    CTestTzServerStartChangeNotification* self =
    	new(ELeave) CTestTzServerStartChangeNotification(aChangePropertyKey, EUninstalled);
    CleanupStack::PushL(self);
    self->ConstructL(KNullDesC, aFileDelete);
    CleanupStack::Pop(self);
    return self;
    }

CTestTzServerStartChangeNotification::CTestTzServerStartChangeNotification(NTzUpdate::TPropertyKeys aChangePropertyKey, TFileModification aFileModification)
	:iPropertyKeys(aChangePropertyKey), iFileModification(aFileModification)
	{
	}


void CTestTzServerStartChangeNotification::ConstructL(const TDesC& aFileCopySrc, const TDesC& aFileCopyDest)
    {
	CTzDataChangeNotificationTestCase::ConstructL(iPropertyKeys);
	iFileSource = aFileCopySrc.AllocL();
	iFileDestination = aFileCopyDest.AllocL();
	}

CTestTzServerStartChangeNotification::~CTestTzServerStartChangeNotification()
	{
	delete iFileSource;
	delete iFileDestination;
	}

void CTestTzServerStartChangeNotification::SetUpL()
	{
	User::LeaveIfError(iTestServer.Connect());
	}

void CTestTzServerStartChangeNotification::TriggerChangeNotificationL()
	{
	iTestServer.CloseTzSession();

	//When deleting a TZ DB or resource files to the C: drive before the TZ server is connectted, 
	//the TZ server should publish the change when it is started
	if(*iFileDestination != KDestTzLocRscDir)
		{
		iTestServer.DeleteFileL(*iFileDestination);
		}
	else
		{
		iTestServer.DeleteFileL(KDestTzLocRscFile1);
		iTestServer.DeleteFileL(KDestTzLocRscFile2);
		iTestServer.DeleteFileL(KDestTzLocRscFile3);
		iTestServer.DeleteFileL(KDestTzLocRscFile4);
		iTestServer.DeleteFileL(KDestTzLocRscFile5);
		iTestServer.DeleteFileL(KDestTzLocRscFile6);

		iTestServer.DeleteDirL(KDestTzLocRscDir);
		}
	
	//When copying a TZ DB or resource files to the C: drive before the TZ server is connectted, 
	//the TZ server should publish the change when it is started
	if(iFileModification == EInstalled)
		{
		//Delete the file which contains the time stamps to test the situation when the time stamps are not vailable first time 
		iTestServer.DeleteFileL(KTimeStampFile);
		if(*iFileDestination == KDestTzLocRscDir)
			{
			iTestServer.CopyFileL(KSrcTzLocRscFile1, KDestTzLocRscFile1);
			iTestServer.CopyFileL(KSrcTzLocRscFile2, KDestTzLocRscFile2);
			iTestServer.CopyFileL(KSrcTzLocRscFile3, KDestTzLocRscFile3);
			iTestServer.CopyFileL(KSrcTzLocRscFile4, KDestTzLocRscFile4);
			iTestServer.CopyFileL(KSrcTzLocRscFile5, KDestTzLocRscFile5);
			iTestServer.CopyFileL(KSrcTzLocRscFile6, KDestTzLocRscFile6);
			}
		else
			{
			iTestServer.CopyFileL(*iFileSource, *iFileDestination);
			}
		}
	
	// The resolution of file modified time on hardware is one second.
	// so make sure we wait at least that before connecting the the tz server again.
	User::After(2000000);
	User::LeaveIfError(iTzServer.Connect());
	}

void CTestTzServerStartChangeNotification::TidyUpAfterChangeNotificationL()
	{//Nothing to do - all data has been cleaned during uninstall testing.
	}

/**
TZ Data Change Notification Test Suite

The purpose of the following Test Cases is to verify the client notification
behaviour of the TZ Server component when changes are made to TZ data.
*/
class CTzDataChangeNotificationTestSuite : public CBase
	{
public:
	static CTzDataChangeNotificationTestSuite* NewLC();
	~CTzDataChangeNotificationTestSuite();
	
	void RunTestsL();
	
private:
	CTzDataChangeNotificationTestSuite();
	void ConstructL();

private:
	// Test cases.
	CTestSysTzRulesDbChangeNotification* sysTzRulesDbTestCase;
	CTestSysTzLocRscChangeNotification* sysTzLocRscTestCase;
	
	CTestTzServerStartChangeNotification* tzServerStartTzRuleInstall;
	CTestTzServerStartChangeNotification* tzServerStartTzRuleUninstall;
	CTestTzServerStartChangeNotification* tzServerStartUserRuleInstall;
	CTestTzServerStartChangeNotification* tzServerStartUserRuleUninstall;
	CTestTzServerStartChangeNotification* tzServerStartUseNameInstall;
	CTestTzServerStartChangeNotification* tzServerStartUseNameUninstall;
	CTestTzServerStartChangeNotification* tzServerStartLocRscInstall;
	CTestTzServerStartChangeNotification* tzServerStartLocRscUninstall;
	};
		

CTzDataChangeNotificationTestSuite* CTzDataChangeNotificationTestSuite::NewLC()
	{
	CTzDataChangeNotificationTestSuite* self =
		new(ELeave) CTzDataChangeNotificationTestSuite();
	CleanupStack::PushL(self);
	self->ConstructL();		
	return self;
	}


CTzDataChangeNotificationTestSuite::CTzDataChangeNotificationTestSuite()
	{
	}


void CTzDataChangeNotificationTestSuite::ConstructL()
	{
	sysTzRulesDbTestCase = CTestSysTzRulesDbChangeNotification::NewL();
	sysTzLocRscTestCase = CTestSysTzLocRscChangeNotification::NewL();
	
	tzServerStartTzRuleInstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzRulesChange, KSrcTzRulesDb, KDestTzRulesDb);
	tzServerStartTzRuleUninstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzRulesChange, KDestTzRulesDb);
	tzServerStartUserRuleInstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzRulesChange, KSrcTzUserDb, KDestTzUserDb);
	tzServerStartUserRuleUninstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzRulesChange, KDestTzUserDb);
	tzServerStartUseNameInstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzNamesChange, KSrcTzUserDb, KDestTzUserDb);
	tzServerStartUseNameUninstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzNamesChange, KDestTzUserDb);
	tzServerStartLocRscInstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzNamesChange, KSrcTzLocRscDir, KDestTzLocRscDir);
	tzServerStartLocRscUninstall = CTestTzServerStartChangeNotification::NewL(NTzUpdate::ETzNamesChange, KDestTzLocRscDir);
	}


CTzDataChangeNotificationTestSuite::~CTzDataChangeNotificationTestSuite()
	{
	delete sysTzRulesDbTestCase;
	delete sysTzLocRscTestCase;
	delete tzServerStartTzRuleInstall;
	delete tzServerStartTzRuleUninstall;
	delete tzServerStartUserRuleInstall;
	delete tzServerStartUserRuleUninstall;
	delete tzServerStartUseNameInstall;
	delete tzServerStartUseNameUninstall;
	delete tzServerStartLocRscInstall;
	delete tzServerStartLocRscUninstall;
	}


void CTzDataChangeNotificationTestSuite::RunTestsL()
	{
	test.Title();

	
	test.Start(_L("@SYMTestCaseID PIM-APPSERV-TZ-TZS-CN-0001  Test notification of system TZ rules database change."));
	sysTzRulesDbTestCase->TestChangeNotificationL();

	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZ-TZS-CN-0002 Test notification of system TZ localization resources change."));
	sysTzLocRscTestCase->TestChangeNotificationL();

	
	test.Next(_L("Test notification when the time zone server connected"));
	//The following CTestTzServerStartChangeNotificationtest test cases are to verify that the TZ server will publish a NTzUpdate::ETzRulesChange or
	//NTzUpdate::ETzNamesChange if a change of the system TZ DB or the user defined DB or the syststem TZ location resource files 
	//happened when the TZ server is not connected.

	// A system TZ DB is copied to the C: when the server is not connected, NTzUpdate::ETzRulesChange should be publised
	test.Printf(_L("A system TZ DB is copied to the C:- expect for NTzUpdate::ETzRulesChange"));
	tzServerStartTzRuleInstall->TestChangeNotificationL();
	// A system TZ DB is removed from the C: when the server is not connected, NTzUpdate::ETzRulesChange should be publised
	test.Printf(_L("A system TZ DB is removed from the C:- expect for NTzUpdate::ETzRulesChange"));
	tzServerStartTzRuleUninstall->TestChangeNotificationL();
	// A user defined DB is copied to the C: when the server is not connected, NTzUpdate::ETzRulesChange should be publised
	test.Printf(_L("A user defined DB is copied to the C: - expect for NTzUpdate::ETzRulesChange"));
	tzServerStartUserRuleInstall->TestChangeNotificationL();
	// A user defined DB is removed from the C: when the server is not connected, NTzUpdate::ETzRulesChange should be publised
	test.Printf(_L("A user defined DB is removed from the C: - expect for NTzUpdate::ETzRulesChange"));
	tzServerStartUserRuleUninstall->TestChangeNotificationL();
	// A user defined DB is copied to the C: when the server is not connected, NTzUpdate::ETzNamesChange should be publised
	test.Printf(_L("A user defined DB is copied to the C:- expect for NTzUpdate::ETzNamesChange"));
	tzServerStartUseNameInstall->TestChangeNotificationL();
	// A user defined DB is remove from the C: when the server is not connected, NTzUpdate::ETzNamesChange should be publised
	test.Printf(_L("A user defined DB is removed from the C:- expect for NTzUpdate::ETzNamesChange"));
	tzServerStartUseNameUninstall->TestChangeNotificationL();
	// Some localization files are copied to the C: when the server is not connected, NTzUpdate::ETzNamesChange should be publised
	test.Printf(_L("Some localization files are copied to the C:- expect for NTzUpdate::ETzNamesChange"));
	tzServerStartLocRscInstall->TestChangeNotificationL();
	// Some localization files are removed from the C: when the server is not connected, NTzUpdate::ETzNamesChange should be publised
	test.Printf(_L("Some localization files are removed from the C:- expect for NTzUpdate::ETzNamesChange"));
	tzServerStartLocRscUninstall->TestChangeNotificationL();
	test.End();
	}


static void DoTestsL()
	{
	CTzDataChangeNotificationTestSuite* testSuite =
		CTzDataChangeNotificationTestSuite::NewLC();
	
	//Have to kill agenda server to make sure no session is connected to the TZ server
	RPIMTestServer serv;
	User::LeaveIfError(serv.Connect());
	CleanupClosePushL(serv);
	serv.CloseTzSession();
	_LIT(KAgendaServer,"agsvexe*");
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	_LIT(KTzServer,"Tzserver*");
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	CleanupStack::PopAndDestroy(&serv);

	testSuite->RunTestsL();
	
	CleanupStack::PopAndDestroy(testSuite);
	}


TInt E32Main()
    {
	__UHEAP_MARK;

	CTrapCleanup* trapCleanup = CTrapCleanup::New();
	if(!trapCleanup)
		{
		return KErrNoMemory;
		}

	CActiveScheduler* scheduler = new CActiveScheduler;
	if(!scheduler)
		{
		return KErrNoMemory;
		}
	CActiveScheduler::Install(scheduler);	

	TRAPD(ret, DoTestsL());
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);
	test(ret == KErrNone);
	
	test.Close();
	
	delete scheduler;
	delete trapCleanup;	

	__UHEAP_MARKEND;

	return (KErrNone);
    }
