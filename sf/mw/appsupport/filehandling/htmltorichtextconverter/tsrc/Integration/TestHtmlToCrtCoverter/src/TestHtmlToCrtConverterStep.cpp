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

#include "TestHtmlToCrtConverterStep.h"

CTestHtmlToCrtConverterStep::CTestHtmlToCrtConverterStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestHtmlToCrtConverterStep);
	}

CTestHtmlToCrtConverterStep::~CTestHtmlToCrtConverterStep()
/**
 * Destructor
 */
	{
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestHtmlToCrtConverterStep::doTestStepL()
	{
	TPtrC	sourceFile;
	GetStringFromConfig(ConfigSection(), KSourceFile, sourceFile);
	RArray<TPtrC>	sourceFileList;
	CleanupClosePushL(sourceFileList);
	TokenizeStringL(sourceFile, sourceFileList);
	TPtrC	targetFile;
	GetStringFromConfig(ConfigSection(), KTargetFile, targetFile);
	RArray<TPtrC>	targetFileList;
	CleanupClosePushL(targetFileList);
	TokenizeStringL(targetFile, targetFileList);
	
	for ( TInt i = 0; i < sourceFileList.Count(); i++ )
		{
		// Get the source file which contains HTML tags
		HBufC*	temp = HBufC::NewLC(sourceFileList[i].Length());
		temp->Des().Copy(sourceFileList[i]);
		TPtr	sourceFile(temp->Des());
		// Get the target file to store the converted text
		HBufC*	temp2 = HBufC::NewLC(targetFileList[i].Length());
		temp2->Des().Copy(targetFileList[i]);
		TPtr	targetFile(temp2->Des());
		
		// Synchronous conversion of the file
		SynchronousL(sourceFile, targetFile);
		// OOM testing
		OOMSynchronousL(sourceFile, targetFile);
		// Asynchronous conversion of the file
		AsynchronousL(sourceFile, targetFile);
		// OOM testing for Asynchronous
		OOMAsynchronousL(sourceFile, targetFile);
		// Logs the time taken for synchronous conversion
		PerformanceTestForSynchronousL(sourceFile, targetFile);
		
		CleanupStack::PopAndDestroy(2);//temp, temp2
		}
	

	INFO_PRINTF2(KNumberOfFilesConverted, sourceFileList.Count());
	CleanupStack::PopAndDestroy(2); // targetFileList, sourceFileList

	return TestStepResult();
	}

/** Converts the source file which has text in HTML format to rich text format synchronously
@param	aSourceFile Source file
@param	aTargetFile Target File
@internalTechnology
@test
*/
void CTestHtmlToCrtConverterStep::SynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile)
	{
	TBool	synchronousConversion = EFalse;
	GetBoolFromConfig(ConfigSection(), KSynchronousConversion, synchronousConversion);
	// return if it is not set
	if ( synchronousConversion == EFalse )
		{
		return;
		}
	
	CHtmlToCrtConverter*	converter = new(ELeave) CHtmlToCrtConverter;
	CleanupStack::PushL(converter);

	/* Convert the source file which is in HTML format to rich text format and
	 store it in the target file */
	TRAPD( err, converter->ConvertL(aSourceFile, aTargetFile) );
	if ( err != KErrNone )
		{
		ERR_PRINTF1(KErrConversionNotSuccessful);
		SetTestStepError(err);
		SetTestStepResult(EFail);
		}

	CleanupStack::PopAndDestroy(converter);
	}

/** Converts the source file which has text in HTML format to rich text format Asynchronously
@param	aSourceFile Source file
@param	aTargetFile Target File
@internalTechnology
@test
*/
void CTestHtmlToCrtConverterStep::AsynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile)
	{
	TBool	asyncConversion = EFalse;
	GetBoolFromConfig(ConfigSection(), KAsyncConversion, asyncConversion);
	
	// return if it is not set
	if ( asyncConversion == EFalse )
		{
		return;
		}
	
	CConversionObserver*	observer = CConversionObserver::NewLC();
	CTestScheduler*	scheduler = new(ELeave) CTestScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	CActiveConv*	active = CActiveConv::NewL(aSourceFile, aTargetFile, observer);
	CleanupStack::PushL(active);
	active->StartConversionL();
	TInt	error = scheduler->GetError();
	User::LeaveIfError(error);

	CleanupStack::PopAndDestroy(3);
	}

/** Converts the source file which has text in HTML format to rich text format synchronously
and the function is called in OOM loop
@param	aSourceFile Source file
@param	aTargetFile Target File
@internalTechnology
@test
*/
void CTestHtmlToCrtConverterStep::OOMSynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile)
	{
	TBool	allocTestForSyncConversion = EFalse;
	GetBoolFromConfig(ConfigSection(), KAllocTestForSyncConversion, allocTestForSyncConversion);
	
	// return if it is not set
	if ( allocTestForSyncConversion == EFalse )
		{
		return;
		}
	
	INFO_PRINTF1(_L("starting OOM test\n"));
	TInt tryCount = 1;
	CHtmlToCrtConverter*	converter = new(ELeave) CHtmlToCrtConverter;
	CleanupStack::PushL(converter);
	for ( ;; )
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, tryCount);
	
		/* Convert the source file which is in HTML format to rich text format and
	 	store it in the target file */
		TRAPD( err, converter->ConvertL(aSourceFile, aTargetFile) );
				
		if ( err == KErrNone ) 
			{
			__UHEAP_RESET;
			break;
			}
		if ( err != KErrNoMemory )
			SetTestStepResult(EFail);
		__UHEAP_SETFAIL(RHeap::ENone, 0);		
		tryCount++;
		}	
	CleanupStack::PopAndDestroy(converter);
	INFO_PRINTF2(_L("TRYCOUNT %d"), tryCount);
	INFO_PRINTF1(_L("\nsynchronous OOM test complete\n"));
	}

/** Converts the source file which has text in HTML format to rich text format Asynchronously
and the function is called in the OOM loop
@param	aSourceFile Source file
@param	aTargetFile Target File
@internalTechnology
@test
*/
void CTestHtmlToCrtConverterStep::OOMAsynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile)
	{
	TBool	allocTestForAsyncConversion = EFalse;
	GetBoolFromConfig(ConfigSection(), KAllocTestForAsyncConversion, allocTestForAsyncConversion);
	
	// return if it is not set
	if ( allocTestForAsyncConversion == EFalse )	
		{
		return;
		}
		
	INFO_PRINTF1(_L("Starting OOM test\n"));
	TInt tryCount = 1;
	for ( ;; )
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, tryCount);
		__UHEAP_MARK;
		TRAPD(err, AsynchronousL(aSourceFile, aTargetFile));
				
		if ( err == KErrNone ) 
			{
			__UHEAP_RESET;
			break;
			}
		if ( err != KErrNone )
			{
			__UHEAP_MARKEND;
			}
		if ( err != KErrNoMemory )
			SetTestStepResult(EFail);
		__UHEAP_SETFAIL(RHeap::ENone, 0);		
		tryCount++;
		}	
	INFO_PRINTF2(_L("TRYCOUNT %d"), tryCount);
	INFO_PRINTF1(_L("\nAsynchronous OOM test complete\n"));
	}

/** Logs the time taken to convert the files synchronously
@param	aSourceFile Source file
@param	aTargetFile Target File
@internalTechnology
@test
*/
void CTestHtmlToCrtConverterStep::PerformanceTestForSynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile)
	{
	TBool	performanceTest = EFalse;
	GetBoolFromConfig(ConfigSection(), KPerformanceTest, performanceTest);
	// return if it is not set
	if ( performanceTest == EFalse )
		{
		return;
		}
		
	// Starts recording time
	TTime	startTime;
	TTime	endTime;
	startTime.HomeTime();

	SynchronousL(aSourceFile, aTargetFile);
	// Stops recording time
	endTime.HomeTime();

	// Compute the time difference and prints it in the log file
	TReal32	timeTaken = ComputeTimeDifference(startTime, endTime);
	INFO_PRINTF2(KInfoTimeTakenForConversion, timeTaken);
	}	

/**
Parses a comma separated string and constructs a list out of the values
@param aString source string
@param aList list containing the source data
@param aSeparator separator
@internalTechnology
@test
*/
void CTestHtmlToCrtConverterStep::TokenizeStringL(const TDesC& aString, RArray<TPtrC>& aList, TChar aSeparator)
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
Computes time difference between first two parameters. 
@param	aStartTime  	The start time of the operation
@param	aEndTime 		The end time of the operation
@return	TReal32	Time taken for the conversion
  */
TReal32 CTestHtmlToCrtConverterStep::ComputeTimeDifference(const TTime& aStartTime, const TTime& aEndTime)
	{
	TTimeIntervalMicroSeconds	timeElapsed = aEndTime.MicroSecondsFrom(aStartTime);

	TReal32	timeTaken = (static_cast<TReal32>(timeElapsed.Int64())) / (1000000);

	return	timeTaken;
	}

