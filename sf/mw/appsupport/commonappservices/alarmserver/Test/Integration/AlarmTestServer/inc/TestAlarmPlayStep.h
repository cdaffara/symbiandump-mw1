/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#if (!defined __TESTALARMPLAYSTEP_H__)
#define __TESTALARMPLAYSTEP_H__
#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerBaseStep.h"

class CTestPlayBaseStep : public CTestCITAlarmServerBaseStep
    {
public:
    CTestPlayBaseStep();
    ~CTestPlayBaseStep();
	
protected:
    void TestPlayIntervalDiffL(TInt aAlarmNo, TInt aMaxOffsetIndex);
    void StoreIniPlayOffsetsL();
    void SetPlayIntervalsL();
    void CheckPlayIntervalsL();
    void CheckForNoSoundL(TInt aAlarmNo);
    
	RArray<TInt> iIniOffsets;
    TInt iIniDuration;
    TInt iIniMaxOffsetIndex;
    };




class CTestDefaultAlarmPlayStep : public CTestPlayBaseStep
	{
public:
	CTestDefaultAlarmPlayStep();
	~CTestDefaultAlarmPlayStep();
	virtual TVerdict doTestStepL();
	
private:
    void TestDefaultAlarmPlayStepL();    
	};





class CTestChangePlayIntervalStep : public CTestPlayBaseStep
	{
public:
	CTestChangePlayIntervalStep();
	~CTestChangePlayIntervalStep();
	virtual TVerdict doTestStepL();

private:	
	void TestChangeIntervalstepL();	
	};
	
	

	
class CTestInvalidIntervalStep : public CTestPlayBaseStep
	{
public:
	CTestInvalidIntervalStep();
	~CTestInvalidIntervalStep();
	virtual TVerdict doTestStepL();
	
private:
	void TestInvalidIntervalStepL();
	void SetEmptyPlayIntervalsL();	
	};
	



class CTestRepeatLastIntervalStep : public CTestPlayBaseStep
	{
public:
	CTestRepeatLastIntervalStep();
	~CTestRepeatLastIntervalStep();
	virtual TVerdict doTestStepL();
	
private:
	void TestRepeatLastIntervalL();
	void SetSimpleSequence();
	};
	

	

class CTestStopPlayStep : public CTestPlayBaseStep
	{
public:
	CTestStopPlayStep();
	~CTestStopPlayStep();
	virtual TVerdict doTestStepL();
	
private:
	void TestStopPlayL();
	void SetSimpleSequence();
	};


	
	
class CTestDisablePlayStep : public CTestPlayBaseStep
	{
public:
	CTestDisablePlayStep();
	~CTestDisablePlayStep();
	virtual TVerdict doTestStepL();
	
private:
	void TestDisablePlayL();
	};




class CTestOverrideResourceStep : public CTestPlayBaseStep
	{
public:
	CTestOverrideResourceStep();
	~CTestOverrideResourceStep();
	virtual TVerdict doTestStepL();
	
private:
	void TestOverrideResourceL();
	};



_LIT(KTestDefPlay,"TestDefaultAlarmPlayStep");
_LIT(KTestPlayChange,"TestChangePlayIntervalsStep");
_LIT(KTestPlayInvalid,"TestInvalidIntervalStep");
_LIT(KTestLastPlay,"TestRepeatLastIntervalStep");
_LIT(KTestStopPlay,"TestStopPlayStep");
_LIT(KTestDisable,"TestDisablePlayStep");
_LIT(KTestOverride,"TestOverrideResourceFileStep");

#endif
