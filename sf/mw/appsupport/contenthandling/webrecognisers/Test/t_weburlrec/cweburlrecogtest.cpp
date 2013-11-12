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

#include "cweburlrecogtest.h"

#include <apmrec.h>
#include <apgcli.h>
#include <flogger.h>

const TInt KTestCaseCount = 6;

CWebUrlRecogTest* CWebUrlRecogTest::NewL(TAny* aConstructionsParams)
	{
	TRecogTestConstructionParams* params = reinterpret_cast<TRecogTestConstructionParams*>(aConstructionsParams);
	CWebUrlRecogTest* self = new(ELeave) CWebUrlRecogTest(*params->iAppArcSession, *params->iLogger);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CWebUrlRecogTest::~CWebUrlRecogTest()
	{
	}
	
CWebUrlRecogTest::CWebUrlRecogTest(RApaLsSession& aAppArc, RFileLogger& aLogger)
: CEComRecogTest(), iAppArc(aAppArc), iLogger(aLogger)
	{
	iTestCaseTitle.Set(KNullDesC());
	}
	
void CWebUrlRecogTest::ConstructL()
	{
	}
	
const TDesC& CWebUrlRecogTest::TestPluginTitle() const
	{
	_LIT(KTxtTitle, "WebUrlRec Test");
	return KTxtTitle();
	}

TInt CWebUrlRecogTest::TestCaseCount() const
	{
	return KTestCaseCount;
	}
	
const TDesC& CWebUrlRecogTest::TestCaseTitle(TInt aIndex)
	{
	switch(aIndex)
		{
		case 0:
			{
			_LIT(KTxtTC1, "Recognise http scheme");
			iTestCaseTitle.Set(KTxtTC1);
			} break;
		case 1:
			{
			_LIT(KTxtTC2, "Recognise https scheme");
			iTestCaseTitle.Set(KTxtTC2);
			} break;
		case 2:
			{
			_LIT(KTxtTC3, "Recognise file scheme");
			iTestCaseTitle.Set(KTxtTC3);
			} break;
		case 3:
			{
			_LIT(KTxtTC4, "Recognise mailto scheme");
			iTestCaseTitle.Set(KTxtTC4);
			} break;
		case 4:
			{
			_LIT(KTxtTC5, "Recognise sms scheme");
			iTestCaseTitle.Set(KTxtTC5);
			} break;
		case 5:
			{
			_LIT(KTxtTC6, "Recognise mms scheme");
			iTestCaseTitle.Set(KTxtTC6);
			} break;
		default:
			User::Invariant();
			break;
		};

	return iTestCaseTitle;
	}
	
void CWebUrlRecogTest::RunTestCaseL(TInt aIndex)
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

void CWebUrlRecogTest::RunTestCase1L()
	{
	_LIT(KTxtHttpScheme, "http://www.symbian.com");
		
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(KTxtHttpScheme(), KNullDesC8(), recogResult);
	
	_LIT8(KHttpUrlDataType, "X-Epoc-Url/http");
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KHttpUrlDataType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &KTxtHttpScheme());
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KHttpUrlDataType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);
		LogConfidence(recogResult.iConfidence);		
		}
	else
		User::Leave(KErrNotFound);
	}

void CWebUrlRecogTest::RunTestCase2L()
	{
	_LIT(KTxtHttpScheme, "https://www.symbian.com");
		
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(KTxtHttpScheme(), KNullDesC8(), recogResult);
	
	_LIT8(KHttpUrlDataType, "X-Epoc-Url/https");
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KHttpUrlDataType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &KTxtHttpScheme());
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KHttpUrlDataType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
		LogConfidence(recogResult.iConfidence);
		}
	else
		User::Leave(KErrNotFound);
	}
	
void CWebUrlRecogTest::RunTestCase3L()
	{
	_LIT(KTxtHttpScheme, "file://c:/testfile.txt");
		
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(KTxtHttpScheme(), KNullDesC8(), recogResult);
	
	_LIT8(KHttpUrlDataType, "X-Epoc-Url/file");
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KHttpUrlDataType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &KTxtHttpScheme());
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KHttpUrlDataType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
		LogConfidence(recogResult.iConfidence);
		}
	else
		User::Leave(KErrNotFound);
	}
void CWebUrlRecogTest::RunTestCase4L()
	{
	_LIT(KTxtMailtoScheme, "mailto:mtscf@microsoft.com");
		
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(KTxtMailtoScheme(), KNullDesC8(), recogResult);
	
	_LIT8(KMailtoUrlDataType, "X-Epoc-Url/mailto"); 
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KMailtoUrlDataType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &KTxtMailtoScheme());
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KMailtoUrlDataType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
		LogConfidence(recogResult.iConfidence);
		}
	else
		User::Leave(KErrNotFound);
	}
void CWebUrlRecogTest::RunTestCase5L()
	{
	_LIT(KTxtSmsScheme, "sms:9844704831");
		
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(KTxtSmsScheme(), KNullDesC8(), recogResult);
	
	_LIT8(KSmsUrlDataType, "X-Epoc-Url/sms");
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KSmsUrlDataType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &KTxtSmsScheme());
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KSmsUrlDataType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
		LogConfidence(recogResult.iConfidence);
		}
	else
		User::Leave(KErrNotFound);
	}
void CWebUrlRecogTest::RunTestCase6L()
	{
	_LIT(KTxtMmsScheme, "mms:9844704831");
		
	TDataRecognitionResult recogResult;
	
	iAppArc.RecognizeData(KTxtMmsScheme(), KNullDesC8(), recogResult);
	
	_LIT8(KMmsUrlDataType, "X-Epoc-Url/mms");
	
	TPtrC8 dataTypeResult(recogResult.iDataType.Des8());
	
	if(dataTypeResult.CompareF(KMmsUrlDataType())==0)
		{
		_LIT(KTxtData, "Data: %S");
		iLogger.WriteFormat(KTxtData(), &KTxtMmsScheme());
		_LIT8(KTxtExpected, "Expected: %S");
		iLogger.WriteFormat(KTxtExpected(), &KMmsUrlDataType());
		_LIT8(KTxtActual, "Actual  : %S");
		iLogger.WriteFormat(KTxtActual(), &dataTypeResult);		
		LogConfidence(recogResult.iConfidence);
		}
	else
		User::Leave(KErrNotFound);
	}

void CWebUrlRecogTest::LogConfidence(TInt aConfidence)
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
