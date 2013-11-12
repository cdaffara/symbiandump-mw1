// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef __TIMEZONE_USERDATA_H__
#define __TIMEZONE_USERDATA_H__

#include <e32base.h>
class RTz;
class CTzId;
class CTzRules;
class CTzUserNames;
/**
The CTzUserData class allows the client to create, read, update or delete
user-defined time zones. 

A user-defined time zone is comprised of time zone rules (encapsulated in a
CTzRules object) and time zone names (encapsulated in a CTzUserNames object).

A time zone identifier (encapsulated in a CTzId object) is used to uniquely
identify a user-defined time zone.

The CTzUserData class also allows the time zone identifiers for all existing
user-defined time zones to be determined. 

@publishedPartner
@released
*/
class CTzUserData : public CBase
    {
public:
    IMPORT_C static CTzUserData* NewL(RTz& aTzServer);
    IMPORT_C ~CTzUserData();

    IMPORT_C CTzId* CreateL(const CTzRules& aTzUserRules, const CTzUserNames&
    	aTzUserNames);
    IMPORT_C CTzRules* ReadRulesL(const CTzId& aTzId) const;
	IMPORT_C CTzUserNames* ReadNamesL(const CTzId& aTzId) const;
    IMPORT_C void UpdateL(const CTzId& aTzId, const CTzRules& aTzUserRules,
    	const CTzUserNames& aTzUserNames);
    IMPORT_C void DeleteL(const CTzId& aTzId);
    IMPORT_C void GetTzIdsL(RPointerArray<CTzId>& aTzIds) const;

private:
    CTzUserData(RTz& aTzServer);
    static void CleanupPointerArray(TAny* aArray);
    	
private:
    RTz& iTzServer;
	};
#endif //__TIMEZONE_USERDATA_H__