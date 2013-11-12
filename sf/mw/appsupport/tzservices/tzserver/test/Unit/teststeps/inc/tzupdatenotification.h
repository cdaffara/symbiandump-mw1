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


 
#ifndef __TZUPDATENOTIFICATION_H__
#define __TZUPDATENOTIFICATION_H__

#include "DstUnitStep.h"
#include <e32property.h>
#include <tzupdate.h>

const TInt KMaxLocationLength = 64;

// String for the server create test step code
_LIT(KTzUpdateNotification, "TzUpdateNotification");

// class for observing changes to time zone property
class CTzPropertyWatcher : public CActive
	{
public:
	static CTzPropertyWatcher* NewL();
	~CTzPropertyWatcher();
	
	void Start();
	NTzUpdate::TTimeZoneChange GetChangeDetails() const;
	
	// from CActive
	void RunL();
	void DoCancel();

private:
	CTzPropertyWatcher();
	void ConstructL();

private:
	NTzUpdate::TTimeZoneChange iChange;	
	RProperty iProperty;
	};

// class for unit test to check time zone property updates
class CTzUpdateNotification : public CDstUnitStep
	{
public:
	CTzUpdateNotification(CDstUnitServer &aServer);
	~CTzUpdateNotification();

	static TInt TimeOutCallBack(TAny* aPtr);
	void DoTimeOutCallBack();
	
private:
	void SetHomeTimeZoneL(const TDesC8& aZoneName);
	void SetUpTestL();
	TBool TestChangeL();
	TInt WaitForChangeL();
	TInt GetTimeZoneNumericIdL() const;
	
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	// member data
	RTz iTimeZoneServer;
	CTzPropertyWatcher* iPropertyWatcher;
	
	// time zone names
	TPtrC8 iLocation1;
	TPtrC8 iLocation2;
	
	CPeriodic* iTimeOutTimer;
	TBool iTimedOut;
	TInt iOldTimeZoneId;
	};

#endif // __TZUPDATENOTIFICATION_H__
