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


 
#ifndef TIMEZONE_CACHING_H
#define TIMEZONE_CACHING_H

#include <tz.h>
#include "DstStep.h"
#include "DstUnitStep.h"
#include "DstUnitServer.h"

// String for the server create test step code
_LIT( KTimezoneCacheTestName, "TimezoneCache");

/*
* Tests the caching interface introduced in 9.3. Caching can now be done 
* through RTz. 
*/
class CTzCachingTestStep : public CDstUnitStep
	{
public:
	CTzCachingTestStep( CDstUnitServer &aServer );
	~CTzCachingTestStep();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();
	
	RTz  iTzServer;
	};

#endif

