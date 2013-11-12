// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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


#if (!defined __T_CAPTION_STEP_H__)
#define __T_CAPTION_STEP_H__


#include <s32file.h>
#include <apgnotif.h>
#include "apparctestserver.h"
#include "testableapalssession.h"

 
//
//
//		CT_CaptionStep definition
//
//


class CT_CaptionStep :public CTestStep
	{
public:
	CT_CaptionStep();
	~CT_CaptionStep();
	virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	void DoLanguageTestL();
	void DoShortCaptionTestL();
	void ChangeLocaleL(TLanguage aLanguage);
	void DoIconCaptionOverridesTestL();
private:
	void TestTApaAppInfoStreamsL();
	void TestTApaAppInfoL();
	void TestCApaDoorL();
	void TestCApaSystemControlListL();
	void TestIconCaptionOverridesL();
	void TestApiPrecedenceOverCenRepConfigInfoL();
	void TestCenRepChangeNotificationL();
	void TestIconCaptionOverridesWithChangeLangL();
	void TestIconCaptionOverridesMemoryLeaksL();
	void ForceApplistUpdate();
	void ChangeLocaleWaitForApplistUpdate(TLanguage aLanguage);
private:
	RFs iFs;
	RTestableApaLsSession iLs;
	};

_LIT(KT_CaptionStep,"T_Caption");

#endif
