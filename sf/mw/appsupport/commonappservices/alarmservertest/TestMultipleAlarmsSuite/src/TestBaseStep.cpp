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
// Contains implementation of CTestBaseStep class, from which all test steps
// are derived
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestBaseStep.h"
#include "ProgressCallBack.h"
/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestBaseStep::CTestBaseStep(CTestMultipleAlarmsServer& aTestServer) : iTestServer(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestBaseStep);
	}

/**
Base class virtual. Checks the alert server thread death status after 
any test is complete
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/	
TVerdict CTestBaseStep::doTestStepPostambleL()
	{
	CheckAltSrvThreadDeathStatus();
	return TestStepResult();
	}
	
/**
Returns a pointer to the Test Server
@internalTechnology
@test
*/	
CTestMultipleAlarmsServer* CTestBaseStep::TestServer()
	{
	return &iTestServer;
	}

/**
Reads strings from a particular section in the ini file
@param aSection Ini section name
@param ... A variable number of arguments, in the form <key>, <value&> where
<key> is the ini key whose value is to be read and <value&> is the address of 
the variable to put the value read from ini. The last parameter in the variable
argument must be NULL, to avoid unexpected behaviour
@return True if all values are read, false if even one fails
@internalTechnology
@test
*/
TBool CTestBaseStep::ReadStringsFromConfig(TRefByValue<const TDesC> aSection, ...)
	{
	// Mark the start of variable args
	VA_LIST listOfArgs;
	VA_START(listOfArgs, aSection);
	
	const TDesC* iniKey = NULL;
	TPtrC* iniValue = NULL;

	// Loop until NULL is encountered
	while((iniKey = VA_ARG(listOfArgs, const TDesC*)) != NULL)
		{
		iniValue = VA_ARG(listOfArgs, TPtrC*);
		// Read data from INI
		if(!GetStringFromConfig(aSection, *iniKey, *iniValue))
			{
			return EFalse;
			}
		}
	// Mark end	
	VA_END(listOfArgs);
	return ETrue;
	}

/**
Reads TInts from a particular section in the ini file
@param aSection Ini section name
@param ... A variable number of arguments, in the form <key>, <value&> where
<key> is the ini key whose value is to be read and <value&> is the address of 
the variable to put the value read from ini. The last parameter in the variable
argument must be NULL, to avoid unexpected behaviour
@return True if all values are read, false if even one fails
@internalTechnology
@test
*/
TBool CTestBaseStep::ReadIntsFromConfig(TRefByValue<const TDesC> aSection, ...)
	{
	// Mark the start of variable args
	VA_LIST listOfArgs;
	VA_START(listOfArgs, aSection);
	
	const TDesC* iniKey = NULL;
	TInt* iniValue = NULL;
	
	// Loop until NULL is encountered
	while((iniKey = VA_ARG(listOfArgs, const TDesC*)) != NULL)
		{
		iniValue = VA_ARG(listOfArgs, TInt*);
		// Read data from INI
		if(!GetIntFromConfig(aSection, *iniKey, *iniValue))
			{
			return EFalse;
			}
		}
	// Mark end	
	VA_END(listOfArgs);
	return ETrue;
	}

/**
Prints Ini-Error message and fails the test
@internalTechnology
@test
*/
void CTestBaseStep::IniProblem()
	{
	_LIT(KIniProblem, "Ini file entries not as expected...Hence failing the test");
	ERR_PRINTF1(KIniProblem);
	SetTestStepResult(EFail);
	}
	
/**
Prints a default message indicating the test step in which the error occured and
sets the test step error flag if the error value passed is not KErrNone.
@param aError The error value
@internalTechnology
@test
*/
void CTestBaseStep::PrintIfError(const TInt& aError)
	{
	_LIT(KErrorOccured, "Error occured in %S: %D");
	if(aError != KErrNone)
		{
		ERR_PRINTF3(KErrorOccured, &TestStepName(), aError);
		
		// Set the error flag of the test step.
		// TEF will decide whether it is pass or fail.
		SetTestStepError(aError); 
		}
	}
	
/**
Prints the error message passed as parameter and sets the test step error 
flag if the error value passed is not KErrNone
@param aMessage The message to be printed using INFO_PRINTF. The message must
have a format specifier %D for holding the error no.
@param aError The error value
@internalTechnology
@test
*/
void CTestBaseStep::PrintIfError(const TDesC& aMessage, const TInt& aError)
	{
	if(aError != KErrNone)
		{
		ERR_PRINTF2(aMessage, aError);
		
		// Set the error flag of the test step.
		// TEF will decide whether it is pass or fail.
		SetTestStepError(aError);
		}
	}

/*
Gets the rich data from the file whose name is passed as parameter
@param	aFileName Name of the file 
@param	aAlarmContentBuf Empty descriptor which will hold the data read from the file
@internalTechnology
@test
*/
void CTestBaseStep::GetAlarmContentFromFileL(const TDesC& aFileName, HBufC8*& aAlarmContentBuf)
	{
	RFs fs;
	RFile file;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(file);
	User::LeaveIfError(file.Open(fs, aFileName, EFileRead | EFileShareReadersOnly));
	TInt size;
	User::LeaveIfError(file.Size(size));
	aAlarmContentBuf = HBufC8::NewLC(size);
	TPtr8 pointer(aAlarmContentBuf->Des());
	User::LeaveIfError(file.Read(pointer, size));
	CleanupStack::Pop(aAlarmContentBuf);
	CleanupStack::PopAndDestroy(2, &fs); // file, fs
	}
	
	
/*
Converts a string to TBool
@return	The boolean equivalent
@param	aString Descriptor containing the string to be converted
@internalTechnology
@test
*/
TBool CTestBaseStep::GetBool(const TPtrC& aString)
	{
	_LIT(KTrue, "true");
	return (aString.Compare(KTrue) == KErrNone) ? ETrue : EFalse;
	}
	
/*
Converts a string to TReal32
@return	The TReal32 equivalent
@param	aString Descriptor containing the string to be converted
@internalTechnology
@test
*/
TReal32 CTestBaseStep::GetTReal32(const TPtrC& aString)
	{
	TReal32 real32;
	TLex lex(aString);
	lex.Val(real32);
	return real32;
	}

/**
Checks the alert server thread death status and prints the exit-category
and reason if the thread is dead
@internalTechnology
@test
*/	
void CTestBaseStep::CheckAltSrvThreadDeathStatus()
	{
	// Prints in Bold
	_LIT(KAltSrvThreadDead, "<B>Alert Server Thread has died. Category = %S; Reason = %D</B>");

	TPtrC exitCategory(TestServer()->AltSrvThread().ExitCategory());
	if(exitCategory.Length() != 0)
		{// Thread has ended. Diagnose and print
		INFO_PRINTF3(KAltSrvThreadDead, &exitCategory, TestServer()->AltSrvThread().ExitReason());
		TestServer()->AltSrvThread().Close();
		}
	}

/**
Starts the active scheduler and waits for the request whose TRequestStatus has
been passed as parameter is complete
@param aStatus The TRequestStatus for whose completion the thread must wait.
@internalTechnology
@test
*/	
void CTestBaseStep::StartActiveSchedAndWaitL(TRequestStatus& aStatus)
	{
	CActiveScheduler::Start();
	User::WaitForRequest(aStatus);
	User::LeaveIfError(aStatus.Int());
	}
	
/**
Checks the time difference, with the allowed max seconds passed as parameter
@param aStartTime The start time of the operation
@param aEndTime The end time of the operation
@param aMaxSecondsLimit Maximum seconds allowed for the operation
@internalTechnology
@test
*/	
void CTestBaseStep::CheckTimeDifference(const TTime& aStartTime, const TTime& aEndTime, const TReal32 aMaxSecondsLimit)
	{
	TTimeIntervalMicroSeconds time = aEndTime.MicroSecondsFrom(aStartTime);
	TReal32 rate = (static_cast<TReal32>(time.Int64())) / (1000000);
	INFO_PRINTF3(_L("Expected completion time = %f seconds. Operation completed in %f seconds"), aMaxSecondsLimit, rate);
	if(rate > aMaxSecondsLimit)
		{
		ERR_PRINTF1(_L("Operation took more time than expected...Failing the test"));
		SetTestStepResult(EFail);
		}
	}

/**
Creates and initialises the cal entry session
@param aCreateNewFile EFalse by default. If ETrue, the old file is deleted if it
already exists
@return aCalSession Pointer to the cal entry session for which the view is to
be created
@internalTechnology
@test
*/
CCalSession* CTestBaseStep::CreateAndInitializeCalSessionL(const TBool& aCreateNewFile)
	{
	CCalSession* calSession = CCalSession::NewL();
	CleanupStack::PushL(calSession);
	
	if(aCreateNewFile)
		{// Create new Cal file
		TRAPD(error, calSession->CreateCalFileL(KTestCalFile));
		if(error == KErrAlreadyExists)
			{// Already exists, delete and create new
			calSession->DeleteCalFileL(KTestCalFile);
			calSession->CreateCalFileL(KTestCalFile);	
			}
		else
			{
			User::LeaveIfError(error);	
			}
		}
		
	// Open file	
	calSession->OpenL(KTestCalFile);
	CleanupStack::Pop(calSession);
	return calSession;
	}

/**
Creates the cal entry view
@param aCalSession Pointer to the cal entry session for which the view is to
be created
@return A pointer to the created cal entry view
@internalTechnology
@test
*/
CCalEntryView* CTestBaseStep::CreateEntryViewL(CCalSession* aCalSession)
	{
	TRequestStatus status;
	CProgressCallBack* progress = new (ELeave) CProgressCallBack(status);
	CleanupStack::PushL(progress);
	CCalEntryView* calEntryView = CCalEntryView::NewL(*aCalSession, *progress);
	CleanupStack::PushL(calEntryView);
	StartActiveSchedAndWaitL(status);
	CleanupStack::Pop(calEntryView);
	CleanupStack::PopAndDestroy(progress);
	return calEntryView;
	}
