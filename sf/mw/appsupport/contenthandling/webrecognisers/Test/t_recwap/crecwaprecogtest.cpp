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

#include "crecwaprecogtest.h"

#include <apmrec.h>
#include <apgcli.h>
#include <flogger.h>

const TInt KTestCaseCount = 6;
const TInt KTestDataSize = 2048;

_LIT8(KWMLMimeType,"text/vnd.wap.wml");
_LIT8(KWMLCMimeType,"application/vnd.wap.wmlc");
_LIT8(KWMLSCMimeType,"application/vnd.wap.wmlscriptc");
_LIT8(KWMLSMimeType,"text/vnd.wap.wmlscript");
_LIT8(KWMLDTDMimeType,"text/vnd.symbian.wml.dtd");
_LIT8(KWBXMLMimeType,"application/vnd.wap.wbxml");

CRecWapRecogTest* CRecWapRecogTest::NewL(TAny* aConstructionsParams)
	{
	TRecogTestConstructionParams* params = reinterpret_cast<TRecogTestConstructionParams*>(aConstructionsParams);
	CRecWapRecogTest* self = new(ELeave) CRecWapRecogTest(*params->iAppArcSession, *params->iLogger);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CRecWapRecogTest::~CRecWapRecogTest()
	{
	iFs.Close();
	}
	
CRecWapRecogTest::CRecWapRecogTest(RApaLsSession& aAppArc, RFileLogger& aLogger)
: CEComRecogTest(), iAppArc(aAppArc), iLogger(aLogger)
	{
	iTestCaseTitle.Set(KNullDesC());
	}
	
void CRecWapRecogTest::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	}
	
const TDesC& CRecWapRecogTest::TestPluginTitle() const
	{
	_LIT(KTxtTitle, "Wap Recogniser Test");
	return KTxtTitle();
	}

TInt CRecWapRecogTest::TestCaseCount() const
	{
	return KTestCaseCount;
	}
	
const TDesC& CRecWapRecogTest::TestCaseTitle(TInt aIndex)
	{
	switch(aIndex)
		{
		case 0:
			{
			_LIT(KTxtTC1, "Recognise a.wml");
			iTestCaseTitle.Set(KTxtTC1);
			} break;
		case 1:
			{
			_LIT(KTxtTC2, "Recognise a.wmlc");
			iTestCaseTitle.Set(KTxtTC2);
			} break;
		case 2:
			{
			_LIT(KTxtTC3, "Recognise a.wmlsc");
			iTestCaseTitle.Set(KTxtTC3);
			} break;
		case 3:
			{
			_LIT(KTxtTC4, "Recognise a.wmls");
			iTestCaseTitle.Set(KTxtTC4);
			} break;
		case 4:
			{
			_LIT(KTxtTC5, "Recognise a.dtd");
			iTestCaseTitle.Set(KTxtTC5);
			} break;
		case 5:
			{
			_LIT(KTxtTC6, "Recognise a.wbxml");
			iTestCaseTitle.Set(KTxtTC6);
			} break;
		default:
			User::Invariant();
			break;
		};

	return iTestCaseTitle;
	}
	
void CRecWapRecogTest::RunTestCaseL(TInt aIndex)
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
		default:
			User::Invariant();
			break;
		};
	}

void CRecWapRecogTest::RunTestL(const TDesC& aTestData, const TDesC8& aExpectedType)
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

void CRecWapRecogTest::RunTestCase1L()
	{
	_LIT(KTxtTestFile1Path, "z:\\recogtestdata\\a.wml");
	
	RunTestL(KTxtTestFile1Path(), KWMLMimeType());
	}

void CRecWapRecogTest::RunTestCase2L()
	{
	_LIT(KTxtTestFile2Path, "z:\\recogtestdata\\a.wmlc");
	
	RunTestL(KTxtTestFile2Path(), KWMLCMimeType());
	}
	
void CRecWapRecogTest::RunTestCase3L()
	{
	_LIT(KTxtTestFile3Path, "z:\\recogtestdata\\a.wmlsc");
	
	RunTestL(KTxtTestFile3Path(), KWMLSCMimeType());
	}
	
void CRecWapRecogTest::RunTestCase4L()
	{
	_LIT(KTxtTestFile4Path, "z:\\recogtestdata\\a.wmls");
	
	RunTestL(KTxtTestFile4Path(), KWMLSMimeType());
	}
	
void CRecWapRecogTest::RunTestCase5L()
	{
	_LIT(KTxtTestFile5Path, "z:\\recogtestdata\\a.dtd");
	
	RunTestL(KTxtTestFile5Path(), KWMLDTDMimeType());
	}
	
void CRecWapRecogTest::RunTestCase6L()
	{
	_LIT(KTxtTestFile6Path, "z:\\recogtestdata\\a.wbxml");
	
	RunTestL(KTxtTestFile6Path(), KWBXMLMimeType());
	}
	

void CRecWapRecogTest::LogConfidence(TInt aConfidence)
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
