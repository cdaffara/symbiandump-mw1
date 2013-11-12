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
 
 
#if (!defined __T_FPR_STEP_H__)
#define __T_FPR_STEP_H__ 
#include "TPrintTestServer.h"
#include <prnsetup.h>
#include <gdi.h>
#include <prninf.h>
#include <f32file.h>
#include <test/appfwk_tmsteststep.h>

//!  A CT_FprStep test class derived from CTestStep class.\n 
/*!
  Performs Print tests related to Model lists and printing to files.\n
*/
class CT_FprStep : public CTmsTestStep
	{
public:
	CT_FprStep();
	~CT_FprStep();
//	virtual TVerdict doTestStepPreambleL();
//	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	void TestModelListL(); 
	void RunTestsL();
	void Test1L(CPrinterPort* aPort);
private:
	//!  Active Scheduler for Asynchronous event handling.
	CActiveScheduler* iScheduler;
	//! A handle to the CPrintSetup class
	CPrintSetup* iPrintSetup;
	//! Handle to the MPageRegionPrinter interface
	MPageRegionPrinter* iRegionPrinter;
	//! Handle to the print processor interface
	MPrintProcessObserver* iObserver;
	//! Handle to the File Server Session
	RFs iFs;
	};

//! Constant Literal for FprStep test
_LIT(KT_FprStep,"T_Fpr");
#endif



