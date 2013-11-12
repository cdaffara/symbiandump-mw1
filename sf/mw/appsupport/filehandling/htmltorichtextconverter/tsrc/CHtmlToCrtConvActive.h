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

#ifndef __CHTMLTOCRTCONVACTIVE_H__
#define __CHTMLTOCRTCONVACTIVE_H__

#include <e32base.h>
#include <e32cons.h>
#include <e32std.h>
#include <f32file.h>
#include <txtrich.h> 
#include <txtfrmat.h>
#include <conarc.h>
#include <s32stor.h>
#include <s32file.h>

class CHtmlToCrtConverter;

//=========================
// CActiveConv
//=========================
class CActiveConv : public CActive
	{
public:
	static CActiveConv* NewL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver=NULL);
	~CActiveConv();

	//from CActive
	virtual void DoCancel();
	virtual void RunL();
	void StartConversionL();

private:
	CActiveConv(MConverterUiObserver* aObserver);
	void ConstructL(const TFileName& aSourceFile, const TFileName& aTargetFile);

private:
	CConverterBase* iConverter;
	MConverterUiObserver* iObserver;
	HBufC* iSourceFile;
	HBufC* iTargetFile;
	};

//=========================
// CTestScheduler
//=========================
class CTestScheduler : public CActiveScheduler
 {
public:
	
	virtual void Error(TInt anError) const;
	TInt GetError();

private:
	
	TInt iError;
 };

//=========================
// CConversionObserver
//=========================
class CConversionObserver : public CBase, public MConverterUiObserver
	{
public:
	static CConversionObserver* NewLC();
	~CConversionObserver();

	//from MConverterUiObserver
	void MaxSteps(TInt aMax, TInt aObject);
	TInt Result(TInt aResult, TInt aObject); // Passes in a TResult, returns a TAction
	HBufC* QueryPasswordL(const TDesC& aClue);

private:
	HBufC* iCharsetName;
	};

#endif
