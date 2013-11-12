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



#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

#include "DstIntStep.h"
#include <tz.h>

// String for the server create test step code
_LIT(KPerformance, "Performance");

class CPerformance : public CDstIntStep
	{
public:
	CPerformance();
	~CPerformance();

protected:
	// From CDstStep
	void PrepareTestEnvironmentL();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	void LogAverageL(const TDesC& aTxt, TTime aTotal, TInt aRepetitions);

	TInt iIterations;

	};

#endif
