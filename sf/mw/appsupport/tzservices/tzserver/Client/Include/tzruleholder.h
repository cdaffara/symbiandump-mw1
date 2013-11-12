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

#ifndef __TZ_RULEHOLDER_H__
#define __TZ_RULEHOLDER_H__

#include <vtzrules.h>
#include "tzchangenotifier.h"

// This class owns the actualised rules, encoded rules and TZID for the current time zone.
NONSHARABLE_CLASS(CTzRuleHolder) : public CBase
	{
public:
    static CTzRuleHolder* NewL( RTz& aTzServer );
	~CTzRuleHolder();
	
	inline void SetTzActualisedRules(CVTzActualisedRules* aRules);
	inline void SetTzRules(CTzRules* aTzRules);
	inline void SetCurrentTzId(TUint16 aId);
	inline CVTzActualisedRules* TzActualisedRules();
	inline CTzRules* TzRules();
	inline TUint16 CurrentTzId();
	inline RTz& Server();

	void ClearRules();
	void DoConvertL(TTime& aTime, TTzTimeReference aTimerRef);
    void NotifyTimeZoneChangeL(RTz::TTzChanges aChange);

private:
    void ConstructL();
	CTzRuleHolder( RTz& aTzServer );

private:
	CVTzActualisedRules*    iTzActualisedRules;
	CTzRules*               iTzRules;
	TUint16                 iCurrentTzId;
	RTz&                    iTzServer;
	CTzChangeNotifier*      iTzChangeNotifier;
	};

#include "tzruleholder.inl"

#endif // __TZ_RULEHOLDER_H__

