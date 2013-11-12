// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 
#ifndef __IS_DAYLIGHT_SAVINGS_ON__
#define __IS_DAYLIGHT_SAVINGS_ON__

#include "DstUnitStep.h"

// String for the server create test step code
_LIT( KDaylightSavingsStep, "DaylightSavingsTest");

class CDaylightSavingsTest : public CDstUnitStep
	{
public:
	CDaylightSavingsTest( CDstUnitServer &aServer );
	~CDaylightSavingsTest();
	
    TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

private:
	void TestDaylightSavingsOnL();
    };

#endif
