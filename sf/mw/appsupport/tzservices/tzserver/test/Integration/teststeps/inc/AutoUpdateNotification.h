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




#ifndef __AUTOUPDATENOTIFICATION_H__
#define __AUTOUPDATENOTIFICATION_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

_LIT(KAutoUpdateNotification, "AutoUpdateNotification");

const TInt KLocationNameLength = 64;

enum TTestType 
	{
	EAutoDSTUpdate,
	ESystemTimeChanged,
	ETimeZoneChanged
	};

class CAutoUpdateNotification : public CTestStep
	{
public:
	CAutoUpdateNotification();
	~CAutoUpdateNotification();

private:
	// from CTestStep
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	
	void SetTestType();
	void CreateUTCOffsetChangeEventL();
	void InitialiseTestStepL();
 	
 	TVerdict TestAutoUpdateNotificationL(RTz::TTzAutoDSTUpdateModes aAutoUpdateMode);	

private:
	// system time to be change to
	TTime iInitialTime;
	
	TTime iChangedTime;
	
	// time zone by name
	TBuf8<KLocationNameLength> iLocation; 
	
	TBuf8<KLocationNameLength> iChangedLocation;
	
	RTz iTzServer;
	
	TTestType iTestType;
	
	CActiveScheduler* iActiveScheduler;
	};
 
#endif // __AUTOUPDATENOTIFICATION_H_
