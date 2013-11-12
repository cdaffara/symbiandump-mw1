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



/**
 @file SetAndCheckLocation.h
*/

#ifndef __SETANDCHECKLOCATION_H__
#define __SETANDCHECKLOCATION_H__

#include "DstIntStep.h"

// String for the server create test step code
_LIT(KSetAndCheckLocation, "SetAndCheckLocation");

class CSetAndCheckLocation : public CDstIntStep
	{
public:
	CSetAndCheckLocation();
	~CSetAndCheckLocation();

protected:
	// From CDstStep
	void PrepareTestEnvironmentL();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	TBuf8<KMaxLocationChars> iLocation;		// Location string (eg 'Europe/London')
	TInt iLocationId;						// Location ID for above location
	TInt iExpectedErr;						// Error expected at end of test
	};

#endif
