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

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#if (!defined __T_STREAM_STEP_H__)
#define __T_STREAM_STEP_H__
#include "TPrintTestServer.h"
#include <test/appfwk_tmsteststep.h>

//!  A CT-StreamStep test class derived from CTestStep class.\n 
/**
  Performs Print tests related to Streaming conversions of CPrintSetup class.\n
*/
class CT_StreamStep : public CTmsTestStep
	{
public:
	CT_StreamStep();
	~CT_StreamStep();
//	virtual TVerdict doTestStepPreambleL();
//	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	void TestHeaderFooterL();
	void TestModelRestoreL();
	void TestPrintSetupL();
	TInt IsEqual(const CPrintSetup* aCopy,const CPrintSetup* anOriginal);
	TBool EquateBools(TBool aFirstBool, TBool aSecondBool);
	void TestStoreRestoreL(CPrintSetup* aCopy, CPrintSetup* aOriginal);
private:
	//! Handle to Active Scheduler for Asynchronous event handling
	CActiveScheduler* iScheduler;
	//! Handle to the File Server session
	RFs iFs;
	};
//! Constant Literal for StreamStep test
_LIT(KT_StreamStep,"T_Stream");
#endif


