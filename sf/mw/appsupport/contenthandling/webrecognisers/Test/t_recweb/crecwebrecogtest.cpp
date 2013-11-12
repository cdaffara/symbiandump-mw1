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

#include "crecwebrecogtest.h"

#include <apmrec.h>
#include <apgcli.h>
#include <flogger.h>

const TInt KTestCaseCount = 8;
const TInt KTestDataSize = 2048;
_LIT8(KHtmlMimeType,"text/html");
_LIT8(KXmlMimeType,"text/xml");

CRecWebRecogTest* CRecWebRecogTest::NewL(TAny* aConstructionsParams)
	{
	TRecogTestConstructionParams* params = reinterpret_cast<TRecogTestConstructionParams*>(aConstructionsParams);
	CRecWebRecogTest* self = new(ELeave) CRecWebRecogTest(*params->iAppArcSession, *params->iLogger);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CRecWebRecogTest::~CRecWebRecogTest()
	{
	iFs.Close();
	}
	
CRecWebRecogTest::CRecWebRecogTest(RApaLsSession& aAppArc, RFileLogger& aLogger)
: CEComRecogTest(), iAppArc(aAppArc), iLogger(aLogger)
	{
	iTestCaseTitle.Set(KNullDesC());
	}
	
void CRecWebRecogTest::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	}
	
const TDesC& CRecWebRecogTest::TestPluginTitle() const
	{
	_LIT(KTxtTitle, "Web Recogniser Test");
	return KTxtTitle();
	}

TInt CRecWebRecogTest::TestCaseCount() const
	{
	return KTestCaseCount;
	}
	
const TDesC& CRecWebRecogTest::TestCaseTitle(TInt aIndex)
	{
	switch(aIndex)
		{
		case 0:
			{
			_LIT(KTxtTC1, "Recognise sample1.html");
			iTestCaseTitle.Set(KTxtTC1);
			} break;
		case 1:
			{
			_LIT(KTxtTC2, "Recognise sample1");
			iTestCaseTitle.Set(KTxtTC2);
			} break;
		case 2:
			{
			_LIT(KTxtTC3, "Recognise sample2.htm");
			iTestCaseTitle.Set(KTxtTC3);
			} break;
		case 3:
			{
			_LIT(KTxtTC4, "Recognise sample2");
			iTestCaseTitle.Set(KTxtTC4);
			} break;
		case 4:
			{
			_LIT(KTxtTC5, "Recognise sample3.xhtml");
			iTestCaseTitle.Set(KTxtTC5);
			} break;
		case 5:
			{
			_LIT(KTxtTC6, "Recognise sample3");
			iTestCaseTitle.Set(KTxtTC6);
			} break;
		case 6:
			{
			_LIT(KTxtTC7, "Recognise sample4.xml");
			iTestCaseTitle.Set(KTxtTC7);
			} break;
		case 7:
			{
			_LIT(KTxtTC8, "Recognise sample4");
			iTestCaseTitle.Set(KTxtTC8);
			} break;
		default:
			User::Invariant();
			break;
		};

	return iTestCaseTitle;
	}
	
void CRecWebRecogTest::RunTestCaseL(TInt aIndex)
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
		case 4:
			{
			RunTestCase5L();
			} break;
		case 5:
			{
			RunTestCase6L();
			} break;
		case 6:
			{
			RunTestCase7L();
			} break;
		case 7:
			{
			RunTestCase8L();
			} break;
		default:
			User::Invariant();
			break;
		};
	}

void CRecWebRecogTest::RunTestL(const TDesC& aTestData, const TDesC8& aExpectedType)
	{
	RFile testFile;
	CleanupClosePushL(testFile);
	User::LeaveIfError(testFile.Open(iFs, aTestData, EFileRead));
	HBufC8* fileBuf = HBufC8::NewLC(KTestDataSize);
	TPtr8 fileBufDes = fileBuf->Des();
	
	User::LeaveIfError(testFile.Read(fileBufDes));
	
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(aTestData, *fileBuf, recogResult);
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	_LIT(KTxtData, "Data: %S");
	iLogger.WriteFormat(KTxtData(), &aTestData);
	_LIT8(KTxtExpected, "Expected: %S");
	iLogger.WriteFormat(KTxtExpected(), &aExpectedType);
	_LIT8(KTxtActual, "Actual  : %S");
	iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
	LogConfidence(recogResult.iConfidence);
	
	if(dataTypeResult.CompareF(aExpectedType)!=0)
		User::Leave(KErrNotFound);
	
	CleanupStack::PopAndDestroy(2, &testFile); // fileBuf, testFile
	}

void CRecWebRecogTest::RunTestCase1L()
	{
	_LIT(KTxtTestFile1Path, "z:\\recogtestdata\\sample1.html");
	
	RunTestL(KTxtTestFile1Path(), KHtmlMimeType());
	}

void CRecWebRecogTest::RunTestCase2L()
	{
	_LIT(KTxtTestFile2Path, "z:\\recogtestdata\\sample1");
	
	RunTestL(KTxtTestFile2Path(), KHtmlMimeType());
	}
	
void CRecWebRecogTest::RunTestCase3L()
	{
	_LIT(KTxtTestFile3Path, "z:\\recogtestdata\\sample2.htm");
	
	RunTestL(KTxtTestFile3Path(), KHtmlMimeType());
	}
	
void CRecWebRecogTest::RunTestCase4L()
	{
	_LIT(KTxtTestFile4Path, "z:\\recogtestdata\\sample2");
	
	RunTestL(KTxtTestFile4Path(), KHtmlMimeType());
	}
	
void CRecWebRecogTest::RunTestCase5L()
	{
	_LIT(KTxtTestFile5Path, "z:\\recogtestdata\\sample3.xhtml");
	
	RunTestL(KTxtTestFile5Path(), KHtmlMimeType());
	}
	
void CRecWebRecogTest::RunTestCase6L()
	{
	_LIT(KTxtTestFile6Path, "z:\\recogtestdata\\sample3");
	
	RunTestL(KTxtTestFile6Path(), KHtmlMimeType());
	}
	
void CRecWebRecogTest::RunTestCase7L()
	{
	_LIT(KTxtTestFile7Path, "z:\\recogtestdata\\sample4.xml");
	
	RunTestL(KTxtTestFile7Path(), KXmlMimeType());
	}
	
void CRecWebRecogTest::RunTestCase8L()
	{
	_LIT(KTxtTestFile8Path, "z:\\recogtestdata\\sample4");
	
	RunTestL(KTxtTestFile8Path(), KXmlMimeType());
	}

void CRecWebRecogTest::LogConfidence(TInt aConfidence)
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
