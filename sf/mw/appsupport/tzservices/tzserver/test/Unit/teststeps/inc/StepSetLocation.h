/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#if (!defined __SETLOCATION_H__)
#define __SETLOCATION_H__

#include <tz.h>
#include "DstStep.h"
#include "DstUnitStep.h"
#include "DstUnitServer.h"
#include "tzconfigagent.h"
#include <tzconverter.h>
class CTzUserDataDb;
//////////////////////////////////////////////////////////////////////
// String for the server create test step code
_LIT(KStepSetLocation, "SetLocation");

class CSetLocation : public CDstUnitStep
	{
public:
	CSetLocation(CDstUnitServer &aServer);
	~CSetLocation();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	TInt TestCTimeZoneIdAPIsL();
	TInt TestTimeZoneSettingL();
	TInt TestTimeZoneIdRetrievingL();

private:
	CTzConverter* iTimeConverter;
	};

class CTZUnitTestBase;

/////////////////////////////////////////////////////////////////////////
// CConvertTimeActiveWorker
//
class CTzTestActiveWorker : public CActive
	{
public:
	static CTzTestActiveWorker* NewL(const CTZUnitTestBase& aTzTestStep);
	~CTzTestActiveWorker();
	void Start();

private:
	CTzTestActiveWorker(const CTZUnitTestBase& CConvertTime);
	void CompleteSelf();

	// from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	CTZUnitTestBase& iTzTestStep;
	};


////////////////////////////////////////////////////////////////////
// 
/*
Simple Base class for Async Time Zone Unit Tests
*/
class CTZUnitTestBase : public CDstUnitStep
	{
public:
	CTZUnitTestBase(CDstUnitServer &aServer) :
					CDstUnitStep(aServer),
					iConvertUTC (EFalse),
					iSetZone(EFalse) {}

	~CTZUnitTestBase(){}

	// from CDstStep
	void PrepareTestEnvironmentL();

	// called by active worker
	virtual void TestL() = 0;

protected:
	TPtrC iStrZoneNameId;
	TPtrC iStrZoneNoId;
	TPtrC iStrUTCTime;
	TPtrC iStrLocalTime;
	TPtrC8 iZoneID8;

	TInt iNumZoneID;
	TBool iConvertUTC;
	TBool iSetZone;
	TInt iTestCount;
	CTzTestActiveWorker* iWorker;
	};


////////////////////////////////////////////////////////////////////
// String for the server create test step code
_LIT(KStepConvertTime, "ConvertTime");

/*
This tests conversion of time between utc and wall-clock time in
different situations defined by the script settings

Each test is carried out twice. The test is under the control of 
an active worker to allow the asynchronous behaviour of time 
zone change notification to be observed. 
*/
class CConvertTime : public CTZUnitTestBase
	{
public:
	CConvertTime(CDstUnitServer &aServer);
	~CConvertTime();

	// from CTZUnitTestBase
	virtual void TestL();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

private:
	CTzConverter* iTimeConverter;
	};


/////////////////////////////////////////////////////////////////////////

/** CUpdateListener Listens for UTC Offset update notification from the Publish
and Subscribe API.

@internalComponent
*/
class CUpdateListener : public CActive
    {
public: // Construction/destruction.
    static CUpdateListener* NewL();
    ~CUpdateListener();
	
public: // Methods.
    TBool SubscribeAndWait();

private: // Construction.
    CUpdateListener();

private:    // From CActive.
    void RunL();
    void DoCancel();

private:    // Attributes.
    RProperty iProperty;
	TTimeIntervalSeconds iUtcOffset;
	TBool iOffsetChanged;
	};


////////////////////////////////////////////////////////////////////
// String for the server create test step code
_LIT(KStepTZConfigAgent, "TZConfigAgentTest");

/*
This tests conversion of time between utc and wall-clock time in
different situations defined by the script settings

Each test is carried out twice. The test is under the control of 
an active worker to allow the asynchronous behaviour of time 
zone change notification to be observed. 
*/
class CTZConfigAgentTest : public CTZUnitTestBase, public MTZCfgAgentObserver
	{
public:
	CTZConfigAgentTest(CDstUnitServer &aServer);
	~CTZConfigAgentTest();

	// from CTZUnitTestBase
	virtual void TestL();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	// from MTZCfgAgentObserver
	void NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aChanger);

    // Configuration test.
	TInt TestAutoUpdateConfigurationSettingL();
    
    // Auto-update and notification test.
    TInt TestAutoUpdateAndNotificationL();

	// Auto-update and retrieval test.
	TInt TestAutoUpdateAndRetrievalL();

    // Time logging utility.
    void LogTimeLocalL(const TTime& aTime, const TDesC& aDes);
    
private:
	TInt iNotifications;
	TBool iExpectNotice;
	TInt iOriginalConfig;

	CTzConfigAgent* iConfigAgent;
	CRepository* iRepository;
	CUpdateListener* iListener;
	CTzUserDataDb* iUserDataDb;
	CTzSwiObserver* iTzSwiObserver;
	CTzLocalizationDb* iTzLocalizationDb;
	};
	
////////////////////////////////////////////////////////////////////
// String for the server create test step code
_LIT(KStepGetOffsets, "GetOffsets");

/*
This tests RTz::GetOffsetsForTimeZoneIdsL, which retrieves the UTC offsets for an array
of time zone IDs
*/
class CGetOffsetsTest : public CDstUnitStep
	{
public:
	CGetOffsetsTest(CDstUnitServer &aServer);
	~CGetOffsetsTest();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();
	
	TInt TestGetOffsetsL();
	
	void CreateUserDefinedTzDatabaseL();
	
	enum
		{
		KUserDefinedDSTOnId = 1,
		KUserDefinedDSTOffId = 2
		};
	
	};

#endif



