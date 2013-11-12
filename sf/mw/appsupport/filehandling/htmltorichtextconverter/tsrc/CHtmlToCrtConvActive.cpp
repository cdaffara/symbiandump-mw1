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
#include <e32cons.h>

#include <confndr.h>
#include <conlist.h>
#include <concnf.h>
#include "CHtmlToCrtConvActive.h"
#include "CHtmlToCrtConverter.h"

//=========================
// CActiveConv
//=========================	
CActiveConv* CActiveConv::NewL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver)
	{
	CActiveConv* self = new(ELeave) CActiveConv(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aSourceFile, aTargetFile);
	CleanupStack::Pop(self);
	return self;
	}

CActiveConv::CActiveConv(MConverterUiObserver* aObserver)
:CActive(0),
iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CActiveConv::ConstructL(const TFileName& aSourceFile, const TFileName& aTargetFile)
	{
	iSourceFile = aSourceFile.AllocL();
	iTargetFile = aTargetFile.AllocL();
	//get converter
	CCnaConverterList* convList=CCnaConverterList::NewLC();
	const TUid KUidCHtmlToCrtConverter = {0x1000a90e};
	iConverter = convList->NewConverterL(KUidCHtmlToCrtConverter);
	__ASSERT_DEBUG(iConverter != NULL, User::Invariant());
	CleanupStack::PopAndDestroy(convList);
	
	}

CActiveConv::~CActiveConv()
	{
	Cancel();
	delete iSourceFile;
	delete iTargetFile;
	delete iConverter;
	}

void CActiveConv::DoCancel()
	{
	iConverter->CancelConvert();
	}

void CActiveConv::RunL()
	{
	if (iConverter->DoConvertL())
		{
			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			SetActive();	
		}
	else
		CActiveScheduler::Stop();
	}


void CActiveConv::StartConversionL()
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	iConverter->ConvertAL(*iSourceFile, *iTargetFile, iObserver);
	CActiveScheduler::Start();
	}

//====================================================
//=========================
// CTestScheduler
//=========================
void CTestScheduler::Error(TInt anError) const
	 {
//	 _LIT(KTestPanic,"CTestScheduler RunL leave");
//	 User::Panic(KTestPanic,anError);

	 CONST_CAST(CTestScheduler*,this)->iError = anError;
	 CActiveScheduler::Stop();
	 }

TInt CTestScheduler::GetError()
	{
	return iError;
	}

//====================================================
//=========================
// CConversionObserver
//=========================
CConversionObserver* CConversionObserver::NewLC()
	{
	CConversionObserver* self = new(ELeave) CConversionObserver();
	CleanupStack::PushL(self);
	return self;
	}

CConversionObserver::~CConversionObserver()
	{
	delete iCharsetName;
	}

void CConversionObserver::MaxSteps(TInt /*aMax*/, TInt /*aObject*/)
	{}

TInt CConversionObserver::Result(TInt aResult, TInt /*aObject*/)
	{
	switch(aResult)
		{
		case ESuccess:
			return ERetain;
		case EPartialSuccess:
			return ERepeat;
		case EFailure:
		default:
			return ERemove;
		};
	}

HBufC* CConversionObserver::QueryPasswordL(const TDesC& /*aClue*/)
	{
	iCharsetName=HBufC::NewL(40);
	*iCharsetName=_L("us-ascii");
//	iCharsetName=NULL;
	return iCharsetName;
	}
