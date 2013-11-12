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



#ifndef __CONVERTTIME_H__
#define __CONVERTTIME_H__

#include "DstIntStep.h"

// String for the server create test step code
_LIT(KConvertTime, "ConvertTime");

class CConvertTime : public CDstIntStep
	{
public:
	enum TConversion
		{
		EConvertUTC = 0,
		EConvertLocal,
		EConvertBoth
		};
public:
	CConvertTime();
	~CConvertTime();

protected:
	// From CDstStep
	void PrepareTestEnvironmentL();

	TInt CompareTimesL(TTime t, TConversion aConversion);

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	TTime iUTCTime;
	TTime iLocalTime;
	TTime iSystemLocalTime;
	TConversion iConversion;
	};

///////////////////////////////////////////

// Introduced for defect: INC085691

// String for the server create test step code
_LIT(KConvertTimeViaServer, "ConvertTimeViaServer");

class CConvertTimeViaServer : public CDstIntStep
	{
public:
	enum TConversion
		{
		EConvertUTC = 0,
		EConvertLocal,
		EConvertBoth
		};
public:
	CConvertTimeViaServer();
	~CConvertTimeViaServer();

protected:
	// From CDstStep
	void PrepareTestEnvironmentL();

	TInt CompareTimesL(TTime t, TConversion aConversion);

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	TTime iUTCTime;
	TTime iLocalTime;
	TTime iSystemLocalTime;
	TConversion iConversion;
	};

///////////////////////////////////////////



// String for the server to create test step code
_LIT(KUnknownZone, "UnknownZone");

class CUnknownZoneTest : public CDstIntStep
	{
public:
	CUnknownZoneTest();
	~CUnknownZoneTest();

protected:
	// From CDstStep
	void PrepareTestEnvironmentL();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();
	};

#endif
