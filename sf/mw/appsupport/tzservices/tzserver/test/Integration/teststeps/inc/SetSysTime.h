/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __SET_SYS_TIME_H__
#define __SET_SYS_TIME_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

/**
 * Teststep that sets the system time to a time mentioned in the ini
 * Assumes that the time mentioned in the INI is local to the current
 * timezone
 */
class CSetSysTime: public CTestStep
	{
public:
	CSetSysTime();
	~CSetSysTime();
	
	TVerdict 		doTestStepL();
	
	void 					PrintHomeTimeL();
	void 					PrintTimeL(const TTime& aTime);
	};

_LIT(KSetSysTime,"SetSysTime");

#endif // __SET_SYS_TIME_H__
