// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @test
 @internalComponent - Internal Symbian test code	 
*/

#ifndef		__T_LARGESTACK_H
#define		__T_LARGESTACK_H

#include "appfwk_test_appui.h"

_LIT(KT_LargeStackStep, "T_LargeStack");

class RTestApaLsSession : public RApaLsSession
	{
public:
	TInt TestExeRecognizerL(const CApaCommandLine& aCommandLine);
	};

class CT_LargeStackStep : public CTestStep
	{
public:
	CT_LargeStackStep();
	~CT_LargeStackStep();
	
	//from CTestStep
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();	
	TVerdict doTestStepL();

private:
	void TestLargeStackL();

private:
	RTestApaLsSession iApaLsSession;
	};

#endif
