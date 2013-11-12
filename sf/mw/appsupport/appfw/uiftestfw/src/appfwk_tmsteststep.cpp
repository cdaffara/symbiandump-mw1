/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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


/**
@file
@test
@internalComponent
*/

#include <bautils.h>
#include <test/appfwk_tmsteststep.h>
#include "f32file.h"
#include <e32std.h>


_LIT(KLogLocation,"c:\\logs\\testexecute\\TMS_Log.txt");
_LIT8(KLogPass," PASS");
_LIT8(KLogFail," FAIL");
_LIT8(KRet, "\n");
_LIT8 (KBlankSpace," ");

// Special notes about using this class
//
// In any derived CTxxxxStep class after the __CONSTRUCT_STEP__(xxxx) macro (generally at the end of the file)
// if the methods void CTxxxxxStep::TestSetupL() and void CTxxxxStep::TestClose() do not exist
// they should be added as follows :-
//
//	void CTxxxxStep::TestSetupL()
//		{
//		MultipleResultsForSameID(ETrue); // true or false set on test requirements
//		}
//
//	void CTxxxxStep::TestClose()
//		{
//		iTestStepNames.Close();
//		}
//
// if the methods do already exist add the MultipleResultsForSameID and iTestStepNames.Close lines to them respectivly
//

TInt CTmsTestStep::ReadNextLineL( RFile &aFile, TDes8 &aLine )
// read a cr/lf limiited line from the file,  assumes file is a valid file
// and that aLine is of sufficient length to hold the data
	{
	aLine.Zero();
	TBuf8<1> chr;
	for (;;)
		{
		aFile.Read(chr);
		if ( chr.Length() == 0 )
			{
			break;
			}
		if (chr.CompareF(KRet) == 0)
			{
			// got a line, exctract newline as well
			aFile.Read(chr);
			break;
			}
		else
			{
			aLine.Append(chr);
			}
		}

	return aLine.Length();
	}

void CTmsTestStep::CleanUpAndWriteResults()
	{
	RFs fs;
	fs.Connect();
	CleanupClosePushL(fs);

	iTestStepPositions.Close();
	if (iTestStepNames != NULL)
		{
		iTestStepNames->Reset();
		delete iTestStepNames;
		iTestStepNames = NULL;

		if (BaflUtils::FolderExists(fs, KLogLocation))
		   	{
			RFile file;
			if (BaflUtils::FileExists( fs, KLogLocation ))
				{
				// open the temporary tms log
				TInt err = file.Open( fs, KLogLocation, EFileRead|EFileShareAny);
			    if(err == KErrInUse)
			    	{
			       	CleanupStack::PopAndDestroy();
			       	return;
				   	}
			    if (err == KErrNone)
			    	{
			    	CleanupClosePushL(file);
			    	TBuf8<256> line;
			    	TBuf<250> testID;
			    	TBuf<6> testResult;
			    	// get a line from the temporary tms log
			    	while (CTmsTestStep::ReadNextLineL(file,line))
			    		{
			    		TInt blankPos = line.Find(KBlankSpace);

			    		// get the ID from the line
			    		testID.Copy(line.Left(blankPos));

			    		//get the pass or fail result from the line
			    		testResult.Copy(line.Mid(blankPos+1,4));

			    		// print into the standard tef log the id and the result in the correct format
			    		// so that they are correctly parsed into the TMS csv file
			    		INFO_PRINTF2(_L("START_TESTCASE %S"),&testID);
			    		INFO_PRINTF2(_L("Line = 1 Command = START_TESTCASE %S"),&testID);
			    		INFO_PRINTF2(_L("END_TESTCASE %S"),&testID);
			    		INFO_PRINTF3(_L("Line = 1 Command = END_TESTCASE %S ***TestCaseResult = %S"),&testID,&testResult);
			    		}
			    	CleanupStack::PopAndDestroy();
			    	}
			    // remove the temporary tms log
			    fs.Delete(KLogLocation);
			    }
			}
		}
	CleanupStack::PopAndDestroy();
	}

EXPORT_C CTmsTestStep::CTmsTestStep() : iStepPassFail(ETrue), iShowMultipleResults(EFalse)
	{
	iCurrentTestStepName.Copy(KUnknownSYMTestCaseIDName);
	iTestStepNames=new CDesCArrayFlat(1);
	RFs fs;
	fs.Connect();
	CleanupClosePushL(fs);
	if (BaflUtils::FolderExists(fs, KLogLocation))
		   {
		   if (BaflUtils::FileExists( fs, KLogLocation ))
			   {
			   // remove the temporary tms log
			   fs.Delete(KLogLocation);
			   }
		   }
	CleanupStack::PopAndDestroy();
	}

EXPORT_C void CTmsTestStep::CloseTMSGraphicsStep()
	{
	CTmsTestStep::CleanUpAndWriteResults();

	}

EXPORT_C CTmsTestStep::~CTmsTestStep()
	{
	CTmsTestStep::CleanUpAndWriteResults();
	}

// writes out the results of a test that has just been executed to the log file(s)
// the first time a test result is written it is also added to the iTestStepNames array.
// This array is then checked on subsequent tests to see if the test has already been written to log
// if it has and multiple test results are not wanted further logging of this test is omitted.


void CTmsTestStep::DebugLogL(const TDesC8& aTestCaseID, const TDesC8& aResult, TInt aPos = -1)
	{
    TInt pos = 0;
	RFs fs;
	fs.Connect();
	CleanupClosePushL(fs);

	if (BaflUtils::FolderExists(fs, KLogLocation))
	   {
		RFile file;
		if (!BaflUtils::FileExists( fs, KLogLocation ))
			{
		    User::LeaveIfError( file.Create( fs, KLogLocation, EFileWrite|EFileShareAny));
			}
		else
			{
		    TInt err = file.Open( fs, KLogLocation, EFileWrite|EFileShareAny);
		    if(err == KErrInUse)
			   {
		       CleanupStack::PopAndDestroy();
			   return;
			   }
		    if (aPos == -1)
		    	file.Seek(ESeekEnd, pos);
		    else
		    	file.Seek(ESeekCurrent, aPos);
	        }
			file.Write(aTestCaseID);
			file.Write(aResult);
			file.Write(_L8("\r\n "));
			file.Flush();
			file.Close();
	   }
		CleanupStack::PopAndDestroy();
	}

TInt CTmsTestStep::DebugLogPosL()
	{
    TInt pos = 0;
    TInt pos1 = 0;
	RFs fs;
	fs.Connect();
	CleanupClosePushL(fs);

	if (BaflUtils::FolderExists(fs, KLogLocation))
	   {
		RFile file;
		if (!BaflUtils::FileExists( fs, KLogLocation ))
			{
		    User::LeaveIfError( file.Create( fs, KLogLocation, EFileWrite|EFileShareAny));
			}
		else
			{
		    TInt err = file.Open( fs, KLogLocation, EFileWrite|EFileShareAny);
		    if(err == KErrInUse)
			   {
		       CleanupStack::PopAndDestroy();
			   return KErrGeneral;
			   }
		    file.Seek(ESeekEnd, pos);
		    file.Seek(ESeekCurrent, pos1);
		    file.Close();
	        }


	   }
	   CleanupStack::PopAndDestroy();
	   return pos1;
	}

EXPORT_C void CTmsTestStep::RecordTestResultL()
	{
	TBuf<64> unknownTMSID(KUnknownSYMTestCaseIDName);
	TBuf8<64> unknownID8;
	unknownID8.Copy(unknownTMSID);

	TBuf<64> undefinedTMSID(KUndefinedSYMTestCaseIDName);
	TBuf8<64> undefinedID8;
	undefinedID8.Copy(undefinedTMSID);

	TBuf<64> notaTMSID(KNotATestSYMTestCaseIDName);
	TBuf8<64> notaID8;
	notaID8.Copy(notaTMSID);

	TBuf<128> currentTSName;
	currentTSName.Copy(iCurrentTestStepName);

	if (iCurrentTestStepName.Compare(unknownID8) == 0) 	// unknown tests logged to warning file
		{
		INFO_PRINTF1(_L("ID NOT SET"));
		}
	else if (iCurrentTestStepName.Compare(undefinedID8) == 0)	// undefined tests logged to warning file
		{
		INFO_PRINTF1(_L("ID UNDEFINED"));
		}
	else if (iCurrentTestStepName.Compare(notaID8) == 0)
		{
		}
	else if(iShowMultipleResults)	// multiple results for same test should be logged
		{
		if( iStepPassFail == EFalse)
			{
			INFO_PRINTF2(_L("%S FAILED"), &currentTSName );
			DebugLogL(iCurrentTestStepName, KLogFail);
			}
		else
			{
			INFO_PRINTF2(_L("%S PASSED"), &currentTSName );
			DebugLogL(iCurrentTestStepName, KLogPass);
			}
		}
	else if (!iShowMultipleResults)
		{
		TInt existsInArray;
		if (iTestStepNames == NULL)
			iTestStepNames=new CDesCArrayFlat(1);

		if( iTestStepNames->Find(currentTSName,existsInArray) == 0)
			{
			if( iStepPassFail == EFalse)
				{
					INFO_PRINTF2(_L("%S FAILED"), &currentTSName );
					DebugLogL(iCurrentTestStepName, KLogFail, iTestStepPositions[existsInArray] );
				}
			}
		else
			{
			iTestStepPositions.AppendL(DebugLogPosL());
			iTestStepNames->AppendL(currentTSName);

			if( iStepPassFail == EFalse)
				{
					INFO_PRINTF2(_L("%S FAILED"), &currentTSName );
					DebugLogL(iCurrentTestStepName, KLogFail );
				}
				else
				{
					INFO_PRINTF2(_L("%S PASSED"), &currentTSName );
					DebugLogL(iCurrentTestStepName, KLogPass);
				}
			}
		}
	}

// sets the name and pass/fail state of the individual test that is about to be executed
EXPORT_C void CTmsTestStep::SetTestStepID(const TDesC& aStepName)
	{
	iCurrentTestStepName.Copy(aStepName);
	iStepPassFail = ETrue;
	}

EXPORT_C void CTmsTestStep::SetOverallTestStepID(const TDesC& aStepName)
	{
	iCurrentTestStepName.Copy(aStepName);
	iStepPassFail = ETrue;
	if (!TestStepResult() == EPass)
				TEST(EFalse);
	}

// needed for the overide of CTGraphicsBase::testBooleanTrue to allow us to set
// whether an individual test has passed or failed
EXPORT_C void CTmsTestStep::MQCTest(TBool aCondition, const TText8* aFile, TInt aLine)
	{
	if(!aCondition)
		{
		iStepPassFail = EFalse;
		}
	testBooleanTrue( aCondition, aFile, aLine, ETrue);
	}

// needed for the overide of CTGraphicsBase::testBooleanTrue to allow us to set
// whether an individual test has passed or failed
EXPORT_C void CTmsTestStep::MQCTestL(TBool aCondition, const TText8* aFile, TInt aLine)
	{
	if(!aCondition)
		{
		iStepPassFail = EFalse;
		}
	testBooleanTrueL( aCondition, aFile, aLine, ETrue);
	}

// sets whether tests with the same @SYMTestCaseID xxxxxx name are printed multiple times in the log files
EXPORT_C void CTmsTestStep::MultipleResultsForSameID(TBool aShowMultipleResults)
	{
	iShowMultipleResults = aShowMultipleResults;
	}
