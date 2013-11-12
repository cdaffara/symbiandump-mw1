// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestTZServer.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <tz.h>
#include <tzconverter.h>
#include "ConvertTime.h"

// Class CTestTZServer - This class executes tests on the TZ component
CTestTZServer::CTestTZServer()
	{
	SetTestStepName(KTestTZServer);
	}

CTestTZServer::~CTestTZServer()
	{
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@test
*/
TVerdict CTestTZServer::doTestStepL()
	{
	TESTL(TestStepResult() == EPass);
	INFO_PRINTF1(_L("CTestTZServer::doTestStepL() start"));

	iField = KTZTest;
	TESTL(GetIntFromConfig(ConfigSection(),iField,iIntTestNumber));// gets the test number

	switch (iIntTestNumber)
		{
		case(ETZTest1):
			OOMGetOffsetTZIdsL();
			break;
			
		case(ETZTest2):
			OOMTimeConversionsL();
			break;
			
		case(ETZTest3):
			OOMGetCurrentTimeZoneL();
			break;
			
		case(ETZTest4):
			OOMSetCurrentTimeZoneL();
			break;
			
		case(ETZTest5):
			TimeZoneNumericIdL();
			break;
			
		case(ETZTest6):
			OOMDaylightSavingL();
			break;
			
		case(ETZTest7):
			TZConverterL();
			break;
			
		case(ETZTest8):
			MulitpleTZConverterL();
			break;

		case(ETZTest9):
			MulitpleTZServerL();
			break;
			
		default:
			User::Leave(KErrNotFound);
		}

	INFO_PRINTF1(_L("CTestTZServer::doTestStepL() finish"));
	return TestStepResult();
	}

/** Retrives offsets for various numeric ids in OOM conditions
*/
void CTestTZServer::OOMGetOffsetTZIdsL()
	{
	RArray<TPtrC>	numericIdsList;
	CleanupClosePushL(numericIdsList);
	
	TPtrC numericids;
	GetStringFromConfig(ConfigSection(), KNumericIds, numericids);
	TokenizeStringL(numericids, numericIdsList); //retrieve numeric ids to obtain their offsets
	
	RArray<TInt> tzIds;
	CleanupClosePushL(tzIds);
	
	for (TInt i = 0; i < numericIdsList.Count(); i++)
		{
		TLex lex(numericIdsList[i]);
		TInt numericId;
		User::LeaveIfError(lex.Val(numericId));
		tzIds.Append(numericId);
		}
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());

	tz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);

	RArray<TInt> offsets;
	CleanupClosePushL(offsets);
		
	iFailAt = 1;
	
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		offsets.Reset();
		TRAPD(err,tz.GetOffsetsForTimeZoneIdsL(tzIds,offsets));
		 
		if ( err == KErrNone )
			{
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of GetOffsetsForTimeZoneIdsL Api is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}		
	
	RArray<TPtrC>	expectedOffsetList;
	CleanupClosePushL(expectedOffsetList);
	
	TPtrC expectedOffsets;
	GetStringFromConfig(ConfigSection(), KOffset, expectedOffsets);
	TokenizeStringL(expectedOffsets, expectedOffsetList); //gets the expected offsets for the specified numeric ids
	
	RArray<TInt> expectedOffsetsList;
	CleanupClosePushL(expectedOffsetsList);
	
	for (TInt i = 0; i < expectedOffsetList.Count(); i++)
		{
		TLex lex(expectedOffsetList[i]);
		TInt offset;
		User::LeaveIfError(lex.Val(offset));
		expectedOffsetsList.Append(offset);
		}
	
	//compares the expected offsets for the numeric ids with the actual retrieved offsets
	TESTL(expectedOffsetsList.Count() == offsets.Count());
	for (TInt x = 0; x < expectedOffsetsList.Count(); x++)
		{
		INFO_PRINTF2(_L("tzid : %d"),tzIds[x]);
		}
		
	for (TInt x = 0; x < expectedOffsetsList.Count(); x++)
		{
		INFO_PRINTF2(_L("actual : %d"),offsets[x]);
		}
	
	for (TInt x = 0; x < expectedOffsetsList.Count(); x++)
		{
		TESTL(expectedOffsetsList[x] == offsets[x]);
		}
	
	CleanupStack::PopAndDestroy(&expectedOffsetsList);
	CleanupStack::PopAndDestroy(&expectedOffsetList);
	CleanupStack::PopAndDestroy(&offsets);
	CleanupStack::PopAndDestroy(&tz);
	CleanupStack::PopAndDestroy(&tzIds);
	CleanupStack::PopAndDestroy(&numericIdsList);
	}

/** Performs time conversions (in OOM conditions) to local/universal times either using system or specified time zone
*/
void CTestTZServer::OOMTimeConversionsL()
	{
	CActiveScheduler* scheduler;
	scheduler = new CActiveScheduler();
	CleanupStack::PushL(scheduler);  
	CActiveScheduler::Install(scheduler);
		
	_LIT8(KEuropeLondon, "Europe/London");
	CTzId* zoneId = CTzId::NewL(KEuropeLondon);
	
	CleanupStack::PushL(zoneId);

	RTz tz;
	CTzConverter* timeConverter = NULL;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	
	TRAPD(err, timeConverter = CTzConverter::NewL(tz));
	TESTL( err == KErrNone );
	
	CleanupStack::PushL(timeConverter);	
	
	TPtrC	ptrSystemTime;
	TESTL(GetStringFromConfig(ConfigSection(), KTime, ptrSystemTime));
	TTime	time(ptrSystemTime);
	
	iFailAt = 1;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TInt err = timeConverter->ConvertToLocalTime(time);
		 
		if ( err == KErrNone )
			{
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of ConvertToLocalTime Api (using current time zone) is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}
		
	iFailAt = 1;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TInt err = timeConverter->ConvertToLocalTime(time, *zoneId);
		 
		if ( err == KErrNone )
			{
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of ConvertToLocalTime Api (specifiying time zone) is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}	
		
	iFailAt = 1;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TInt err = timeConverter->ConvertToUniversalTime(time);
		 
		if ( err == KErrNone )
			{
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of ConvertToUniversalTime Api (using current time zone) is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}	
		
	iFailAt = 1;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TInt err = timeConverter->ConvertToUniversalTime(time, *zoneId);
		 
		if ( err == KErrNone )
			{
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of ConvertToUniversalTime (specifiying time zone) Api is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}	
		
	CleanupStack::PopAndDestroy(timeConverter);
	CleanupStack::PopAndDestroy(&tz);
	CleanupStack::PopAndDestroy(zoneId);
	CleanupStack::PopAndDestroy(scheduler);
	
	CActiveScheduler::Install(NULL);
	}

/** Gets the current system time zone in OOM conditions
*/
void CTestTZServer::OOMGetCurrentTimeZoneL()
	{
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	
	CTzId* tzId = NULL;

	iFailAt = 1;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TRAPD(err,tzId = tz.GetTimeZoneIdL());
		 
		if ( err == KErrNone )
			{
			TESTL(tzId != NULL);
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of GetTimeZoneIdL()) Api is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}	
	
	CleanupStack::PopAndDestroy(&tz);
	}

/** Soak/OOM tests for setting and getting time zones and testing of TimeZoneNameID api
*/
void CTestTZServer::OOMSetCurrentTimeZoneL()
	{
	_LIT8(KEuropeLondon, "Europe/London");
	CTzId* zoneId = CTzId::NewL(KEuropeLondon);
	
	CleanupStack::PushL(zoneId);
	
	_LIT8(KPacificPortMoresby, "Pacific/Port_Moresby");
	CTzId* zoneId1 = CTzId::NewL(KPacificPortMoresby);
	
	CleanupStack::PushL(zoneId1);
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());

	TInt i = 0;

	for (i = 0; i < KRepeatApiNumber; i++)
		{
		TRAPD(err,tz.SetTimeZoneL(*zoneId));	
		TESTL(err == KErrNone);
		}
		
	
	for (i = 0; i < KRepeatApiNumber; i++)
		{
		TRAPD(err,tz.SetTimeZoneL(*zoneId1));	
		TESTL(err == KErrNone);
		}
	
	CTzId* tzId = NULL;
	
	iFailAt = 1;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TRAPD(err5,tzId = tz.GetTimeZoneIdL());
		
		if ( err5 == KErrNone )
			{
			TESTL(tzId != NULL);
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of GetTimeZoneIdL()) Api is done"));
			break;
			}
				
		if ( ErrorProcess( err5) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}	
		}	

	CleanupStack::PopAndDestroy(&tz);
	CleanupStack::PopAndDestroy(zoneId1);
	CleanupStack::PopAndDestroy(zoneId);
	}

/** Retrieves numeric id for various time zones
*/
void CTestTZServer::TimeZoneNumericIdL()
	{
	iField.Zero();
	iField.Append(KNumericIds);
	GetIntFromConfig(ConfigSection(),iField,iIntNumericId); //gets expected numeric id
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	
	CTzId* tzId = NULL;

	TRAPD(err,tzId = tz.GetTimeZoneIdL());
		
	TESTL(err == KErrNone);
	
	CleanupStack::PushL(tzId);
	 
	TUint tzNumericId = tzId->TimeZoneNumericID();
	
	INFO_PRINTF2(_L("actual : %d"),tzNumericId);
	
	TESTL(tzNumericId == iIntNumericId);
	
	CleanupStack::PopAndDestroy(tzId);
	CleanupStack::PopAndDestroy(&tz);
	}

/** Retrieves daylight property for various time zones in OOM conditions
*/
void CTestTZServer::OOMDaylightSavingL()
	{
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());

	tz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);

	CTzId* tzId = NULL;

	TRAPD(err1,tzId = tz.GetTimeZoneIdL()); //the current system time zone
	TESTL ( err1 == KErrNone );
	
	CleanupStack::PushL(tzId);
		
	TBool isDaylightOn = EFalse;
	
	iFailAt = 0;
	for ( ;; )
		{
		StartHeapCheck(iFailAt);
		TRAPD(err, isDaylightOn = tz.IsDaylightSavingOnL(*tzId)); //daylight property for current system time zone
		 
		if ( err == KErrNone )
			{
			__UHEAP_RESET;
			INFO_PRINTF1(_L("OOM testing of IsDaylightSavingOnL()) Api is done"));
			break;
			}
				
		if ( ErrorProcess(err) )
			{
			EndHeapCheck();
			}
		else
			{
			break;
			}
		}	
	
	iField.Zero();
	iField.Append(KDaylightSaving);
	GetBoolFromConfig(ConfigSection(),iField,iDaylightSaving); //gets expected daylight property
	
	//compares expected daylight property with actual daylight property
	TESTL(iDaylightSaving == isDaylightOn);
	
	CleanupStack::PopAndDestroy(tzId);
	CleanupStack::PopAndDestroy(&tz);
	}

/** Attempts to construct TZ converter, without connecting to the TZ server, and should return KErrNotReady
*/
void CTestTZServer::TZConverterL()
	{
	RTz tz;
	CleanupClosePushL(tz);
	// Do not connect to the time zone server
	CTzConverter* timeConverter = NULL;
	
	TRAPD(err, timeConverter = CTzConverter::NewL(tz));
	
	TESTL(timeConverter == NULL);
	
	TESTL( err == KErrNotReady );
	
	CleanupStack::PopAndDestroy(&tz);
	}

/** Convert times using multiple TZ converter objects
*/	
void CTestTZServer::MulitpleTZConverterL()
	{
	CActiveScheduler* scheduler;
	scheduler = new CActiveScheduler();
	CleanupStack::PushL(scheduler);  
	CActiveScheduler::Install(scheduler);
	
	TPtrC	ptrSystemTime;
	TESTL(GetStringFromConfig(ConfigSection(), KTime, ptrSystemTime));
	TTime	time(ptrSystemTime);	
	
	_LIT8(KEuropeLondon, "Europe/London");
	CTzId* zoneId = CTzId::NewL(KEuropeLondon);
	
	CleanupStack::PushL(zoneId);

	RTz tz;
	CTzConverter* timeConverter = NULL;
	CTzConverter* timeConverter1 = NULL;
	CTzConverter* timeConverter2 = NULL;
	CTzConverter* timeConverter3 = NULL;
	CTzConverter* timeConverter4 = NULL;
	CTzConverter* timeConverter5 = NULL;
	CTzConverter* timeConverter6 = NULL;
	CTzConverter* timeConverter7 = NULL;

	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	
	TRAPD(err,timeConverter = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter);

	TRAP(err,timeConverter1 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter1);
	
	TRAP(err,timeConverter2 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter2);
	
	TRAP(err,timeConverter3 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter3);
	
	TRAP(err,timeConverter4 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter4);
	
	TRAP(err,timeConverter5 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter5);
	
	TRAP(err,timeConverter6 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter6);
	
	TRAP(err,timeConverter7 = CTzConverter::NewL(tz));
	TESTL(err == KErrNone);
	CleanupStack::PushL(timeConverter7);
	
	TRAP(err, timeConverter->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter1->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter2->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter3->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter4->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter5->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter6->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter7->ConvertToLocalTime(time, *zoneId));
	TESTL(err == KErrNone);
	
	TRAP(err, timeConverter->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter1->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter2->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter3->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter4->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter5->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter6->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter7->ConvertToLocalTime(time));
	TESTL(err == KErrNone);
	
	TRAP(err, timeConverter->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter1->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter2->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter3->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter4->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter5->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter6->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter7->ConvertToUniversalTime(time));
	TESTL(err == KErrNone);
	
	TRAP(err, timeConverter->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter1->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter2->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter3->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter4->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter5->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter6->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	TRAP(err, timeConverter7->ConvertToUniversalTime(time, *zoneId));
	TESTL(err == KErrNone);
	
	CleanupStack::PopAndDestroy(timeConverter7);
	CleanupStack::PopAndDestroy(timeConverter6);
	CleanupStack::PopAndDestroy(timeConverter5);
	CleanupStack::PopAndDestroy(timeConverter4);
	CleanupStack::PopAndDestroy(timeConverter3);
	CleanupStack::PopAndDestroy(timeConverter2);
	CleanupStack::PopAndDestroy(timeConverter1);
	CleanupStack::PopAndDestroy(timeConverter);
	CleanupStack::PopAndDestroy(&tz);
	CleanupStack::PopAndDestroy(zoneId);
	
	CleanupStack::PopAndDestroy(scheduler);
	
	CActiveScheduler::Install(NULL);
	}
	
/** Setting of Time Zone using different Time Zone Servers
*/	
void CTestTZServer::MulitpleTZServerL()
	{
	_LIT8(KEuropeLondon, "Europe/London");
	CTzId* zoneId = CTzId::NewL(KEuropeLondon);
	CleanupStack::PushL(zoneId);
		
	_LIT8(KPacificPortMoresby, "Pacific/Port_Moresby");
	CTzId* zoneId1 = CTzId::NewL(KPacificPortMoresby);
	CleanupStack::PushL(zoneId1);
	
	_LIT8(KAfricaDar, "Africa/Dar_es_Salaam");
	CTzId* zoneId2 = CTzId::NewL(KAfricaDar);
	CleanupStack::PushL(zoneId2);
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	
	RTz tz1;
	CleanupClosePushL(tz1);
	User::LeaveIfError(tz1.Connect());
	
	RTz tz2;
	CleanupClosePushL(tz2);
	User::LeaveIfError(tz2.Connect());
	
	CTzId* tzId = NULL;
	
	TRAPD(err,tz.SetTimeZoneL(*zoneId));
	TESTL(err == KErrNone);
	
	TRAP(err,tz1.SetTimeZoneL(*zoneId1));
	TESTL(err == KErrNone);
	
	TRAP(err,tz2.SetTimeZoneL(*zoneId2));
	TESTL(err == KErrNone);
	
	TRAP(err,tzId = tz.GetTimeZoneIdL());
	TESTL(err == KErrNone);
	TESTL(tzId != NULL);
	
	TRAP(err,tzId = tz1.GetTimeZoneIdL());
	TESTL(err == KErrNone);
	TESTL(tzId != NULL);
	
	TRAP(err,tzId = tz2.GetTimeZoneIdL());
	TESTL(err == KErrNone);
	TESTL(tzId != NULL);
	
	CleanupStack::PopAndDestroy(&tz2);
	CleanupStack::PopAndDestroy(&tz1);
	CleanupStack::PopAndDestroy(&tz);
	CleanupStack::PopAndDestroy(zoneId2);
	CleanupStack::PopAndDestroy(zoneId1);
	CleanupStack::PopAndDestroy(zoneId);
	}
	
/**
  * Parses a comma separated string and constructs a list out of the values
  */
void CTestTZServer::TokenizeStringL(const TDesC& aString, RArray<TPtrC>& aList, TChar aSeparator)
	{
	TLex lexer(aString);

	while(!lexer.Eos())
		{
		lexer.SkipSpaceAndMark();

		while(!lexer.Eos() && lexer.Get() != aSeparator)
			{
			}

		if(!lexer.Eos())
			{
			lexer.UnGet(); // Do not include trailing ','
			}
		aList.AppendL(lexer.MarkedToken());
		lexer.Inc();
		}
	}
	
/**
set heapmark and set heap failure to aFailAt
performs testing on client side
*/
#ifdef _DEBUG
void CTestTZServer::StartHeapCheck(const TInt aFailAt)
#else
void CTestTZServer::StartHeapCheck(const TInt /* aFailAt */)
#endif
	{
	__UHEAP_SETFAIL(RHeap::EDeterministic, aFailAt);
	}

/**
check value of received error 
*/
TBool CTestTZServer::ErrorProcess(const TInt aErr)
	{
	if ( aErr != KErrNoMemory )
 		{
 		__UHEAP_RESET;
 		SetTestStepResult(EFail);
 		return EFalse;
 		}
 	else
 		{
 		return ETrue;
 		}
	}

/**
reset heapmark and reset heap failure
*/
void CTestTZServer::EndHeapCheck()
	{	
	__UHEAP_RESET;
	iFailAt++;
	}
