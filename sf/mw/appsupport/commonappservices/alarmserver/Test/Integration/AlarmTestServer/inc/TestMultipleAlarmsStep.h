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



#if (!defined __TESTMULTIPLEALARMSSTEP_H__)
#define __TESTMULTIPLEALARMSSTEP_H__
#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerBaseStep.h"

class CTestMultipleAlarmsStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultipleAlarmsStep();
	~CTestMultipleAlarmsStep();
	virtual TVerdict doTestStepL();
	
private:
	void TestMultipleAlarmsL();
	};



class CTestSameTimeMultiAlarmsStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestSameTimeMultiAlarmsStep();
	~CTestSameTimeMultiAlarmsStep();
	virtual TVerdict doTestStepL();
	
private:	 
	void TestSameTimeMultipleAlarmsL();
	};



class CTestMultipleAlarmsSnoozeStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultipleAlarmsSnoozeStep();
	~CTestMultipleAlarmsSnoozeStep();
	virtual TVerdict doTestStepL();
	
private:
    TTimeIntervalMinutes iSnoozeTime;
    void TestMultipleAlarmsSnoozeL();
	};



class CTestMultiAlarmsInvalidSnoozeStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultiAlarmsInvalidSnoozeStep();
	~CTestMultiAlarmsInvalidSnoozeStep();
	virtual TVerdict doTestStepL();
	
private:	 
	TTimeIntervalMinutes iSnoozeTime;
	void TestMultipleAlarmsInvalidSnoozeL();
	};



class CTestMultiAlarmsPauseSoundStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultiAlarmsPauseSoundStep();
	~CTestMultiAlarmsPauseSoundStep();
	virtual TVerdict doTestStepL();
	
private:	
	TTimeIntervalMinutes iPauseTime;	
	void TestMultiAlarmsPauseSoundL();
	void TestMultiAlarmsPauseSound2L();
	};



class CTestMultipleAlarmsClearStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultipleAlarmsClearStep();
	~CTestMultipleAlarmsClearStep();
	virtual TVerdict doTestStepL();

private:	
	void TestMultipleAlarmsClearL();	
	};



class CTestMultiAlarmsInvalidClearStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultiAlarmsInvalidClearStep();
	~CTestMultiAlarmsInvalidClearStep();
	virtual TVerdict doTestStepL();

private:	
	void TestMultipleAlarmsInvalidClearL();	
	};



class CTestMultipleAlarmsQueueStep : public CTestCITAlarmServerBaseStep
	{
public:
	CTestMultipleAlarmsQueueStep();
	~CTestMultipleAlarmsQueueStep();
	virtual TVerdict doTestStepL();
	
private:	
	TTimeIntervalMinutes iPauseTime; 	
	void TestMultipleAlarmsQueueL();
	void TestMultAlarmsQFromSnoozeL();
	};
	
	
	
class CTestSilenceAlarmStep : public CTestCITAlarmServerBaseStep
    {
public:
	CTestSilenceAlarmStep();
	~CTestSilenceAlarmStep();
	virtual TVerdict doTestStepL();

private:
    void TestSilenceAlarmL();	
    };



class CTestBackwardsCompatibilityStep: public CTestCITAlarmServerBaseStep
	{
public:
	CTestBackwardsCompatibilityStep();
	~CTestBackwardsCompatibilityStep();
	virtual TVerdict doTestStepL();
	
private:	
	void TestBackwardsCompL();
	void TestBackwardsComp2L();
	void IsAlarmNotifying();
	void AcknowledgeAlarm();
	};


_LIT(KTestMultipleAlarms,"TestMultipleAlarmsStep");
_LIT(KTesSameTimeMultiAlarms,"TestSameTimeMultipleAlarmsStep");
_LIT(KTestMultiWithSnooze,"TestMultipleAlarmsSnoozeStep");
_LIT(KTestMultiWithInvalidSnooze,"TestMultipleAlarmsInvalidSnoozeStep");
_LIT(KTestPauseSound,"TestMultiAlarmsPauseSoundStep");
_LIT(KTestClear,"TestMultipleAlarmsClearStep");
_LIT(KTestInvalidClear,"TestMultiAlarmsInvalidClearStep");
_LIT(KTestQueue,"TestMultipleAlarmsQueueStep");
_LIT(KTestSilenceStep,"TestSilenceAlarmStep");
_LIT(KBackwardsComp,"TestBackwardsCompatibility");
#endif
