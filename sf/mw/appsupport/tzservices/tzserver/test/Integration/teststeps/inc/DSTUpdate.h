/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __DSTUPDATE_H__
#define __DSTUPDATE_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

const TInt KMaxLocationLength = 64;

// String for the server create test step code
_LIT(KDSTUpdate, "DSTAutoUpdate");


class CDSTUpdate : public CTestStep
	{
public:
	CDSTUpdate();
	~CDSTUpdate();

private:
	// from CTestStep
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	TVerdict TestAutoUpdateL(RTz::TTzAutoDSTUpdateModes aAutoUpdateMode);
	
private:
	RTz iTimeZoneServer;
	// initial local time to be set
	TTime iInitialTime;
	// expected local time after DST change event
	TTime iExpectedTime;
	// time zone by name
	TBuf8<KMaxLocationLength> iLocation;
	TBool iIsUtc;
	};

#endif //__DSTUPDATE_H__
