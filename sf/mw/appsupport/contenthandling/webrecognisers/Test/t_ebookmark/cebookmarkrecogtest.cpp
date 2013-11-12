// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cebookmarkrecogtest.h"

#include <apmrec.h>
#include <apgcli.h>
#include <flogger.h>

const TInt KTestCaseCount = 4;
_LIT8(KEBookmarkMimeType, "text/vnd.symbian.ebookmark");

CEBookmarkRecogTest* CEBookmarkRecogTest::NewL(TAny* aConstructionsParams)
	{
	TRecogTestConstructionParams* params = reinterpret_cast<TRecogTestConstructionParams*>(aConstructionsParams);
	CEBookmarkRecogTest* self = new(ELeave) CEBookmarkRecogTest(*params->iAppArcSession, *params->iLogger);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CEBookmarkRecogTest::~CEBookmarkRecogTest()
	{
	iFs.Close();
	}
	
CEBookmarkRecogTest::CEBookmarkRecogTest(RApaLsSession& aAppArc, RFileLogger& aLogger)
: CEComRecogTest(), iAppArc(aAppArc), iLogger(aLogger)
	{
	iTestCaseTitle.Set(KNullDesC());
	}
	
void CEBookmarkRecogTest::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	}
	
const TDesC& CEBookmarkRecogTest::TestPluginTitle() const
	{
	_LIT(KTxtTitle, "EBookmark Test");
	return KTxtTitle();
	}

TInt CEBookmarkRecogTest::TestCaseCount() const
	{
	return KTestCaseCount;
	}
	
const TDesC& CEBookmarkRecogTest::TestCaseTitle(TInt aIndex)
	{
	switch(aIndex)
		{
		case 0:
			{
			_LIT(KTxtTC1, "Recognise EBookmark 1.ebm");
			iTestCaseTitle.Set(KTxtTC1);
			} break;
		case 1:
			{
			_LIT(KTxtTC2, "Recognise EBookmark 2.ebm");
			iTestCaseTitle.Set(KTxtTC2);
			} break;
		case 2:
			{
			_LIT(KTxtTC3, "Recognise EBookmark 3.ebm");
			iTestCaseTitle.Set(KTxtTC3);
			} break;
		case 3:
			{
			_LIT(KTxtTC4, "Recognise EBookmark 4.notebm");
			iTestCaseTitle.Set(KTxtTC4);
			} break;
		default:
			User::Invariant();
			break;
		};

	return iTestCaseTitle;
	}
	
void CEBookmarkRecogTest::RunTestCaseL(TInt aIndex)
	{
	switch(aIndex)
		{
		case 0:
			{
			RunTestCase1L();
			} break;
		case 1:
			{
			RunTestCase2L();
			} break;
		case 2:
			{
			RunTestCase3L();
			} break;
		case 3:
			{
			RunTestCase4L();
			} break;
		default:
			User::Invariant();
			break;
		};
	}

void CEBookmarkRecogTest::RunTestL(const TDesC& aTestData)
	{
	RFile testFile;
	CleanupClosePushL(testFile);
	User::LeaveIfError(testFile.Open(iFs, aTestData, EFileRead));
	TInt fileSize = 0;
	User::LeaveIfError(testFile.Size(fileSize));
	HBufC8* fileBuf = HBufC8::NewLC(fileSize);
	TPtr8 fileBufDes = fileBuf->Des();
	
	User::LeaveIfError(testFile.Read(fileBufDes));
	
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(aTestData, *fileBuf, recogResult);
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KEBookmarkMimeType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &aTestData);
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KEBookmarkMimeType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
		LogConfidence(recogResult.iConfidence);
		}
	else
		User::Leave(KErrNotFound);
	
	CleanupStack::PopAndDestroy(2, &testFile); // fileBuf, testFile
	}

void CEBookmarkRecogTest::RunTestCase1L()
	{
	_LIT(KTxtTestFile1Path, "z:\\recogtestdata\\1.ebm");
	
	RunTestL(KTxtTestFile1Path());
	}

void CEBookmarkRecogTest::RunTestCase2L()
	{
	_LIT(KTxtTestFile2Path, "z:\\recogtestdata\\2.ebm");
	
	RunTestL(KTxtTestFile2Path());
	}
	
void CEBookmarkRecogTest::RunTestCase3L()
	{
	_LIT(KTxtTestFile3Path, "z:\\recogtestdata\\3.ebm");
	
	RunTestL(KTxtTestFile3Path());
	}
	
void CEBookmarkRecogTest::RunTestCase4L()
	{
	_LIT(KTxtTestFile4Path, "z:\\recogtestdata\\4.notebm");
	
	RunTestL(KTxtTestFile4Path());
	}

void CEBookmarkRecogTest::LogConfidence(TInt aConfidence)
	{
	TPtrC8 confidence;
	switch(aConfidence)
		{
		case CApaDataRecognizerType::ECertain:
			{
			_LIT8(KTxtCertain, "Certain");
			confidence.Set(KTxtCertain());
			} break;
		case CApaDataRecognizerType::EProbable:
			{
			_LIT8(KTxtProbable, "Probable");
			confidence.Set(KTxtProbable());
			} break;
		case CApaDataRecognizerType::EPossible:
			{
			_LIT8(KTxtPossible, "Possible");
			confidence.Set(KTxtPossible());
			} break;
		case CApaDataRecognizerType::EUnlikely:
			{
			_LIT8(KTxtUnlikely, "Unlikely");
			confidence.Set(KTxtUnlikely());
			} break;
		case CApaDataRecognizerType::ENotRecognized:
			{
			_LIT8(KTxtNotRecognized, "NotRecognized");
			confidence.Set(KTxtNotRecognized());
			} break;
		default:
			User::Invariant();
			break;
		};
	_LIT8(KTxtConfidence, "Confidence: %S");
	iLogger.WriteFormat(KTxtConfidence(), &confidence);
	}
