// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TIMEZONE_SERVER_H__
#define __TIMEZONE_SERVER_H__

#include <e32base.h>
#include <e32std.h>
#include <s32file.h>
#include <tz.h>
#include "tzconfigagent.h"
#include "tzlocalizationdb.h"
#include "tzswiobserver.h"

const TUint KTimeZoneServerMajorVersion = 0;
const TUint KTimeZoneServerMinorVersion = 1;
const TUint KTimeZoneServerBuildVersion = 1;

// Structures
struct TRendezvous
	{
	TThreadId iId;
	TRequestStatus* iStatus;
	};

class CTzServer;
class CTzUserDataDb;
class CTzUserDataCache;

#define KCapabilityNone 0
#define KCapabilityWriteDeviceData 1000
#define KCapabilityReadUserData 2000
#define KCapabilityWriteUserData 3000

_LIT(KTimeZoneServerName,"!TzServer");

/* CTzServer
@internalComponent
*/
class CTzServer : public CPolicyServer, public MTZCfgAgentObserver
	{
public:
	enum TFunctionCode
		{
		EGetLocalTimeZoneId = KCapabilityNone,
		EConvertLocalZoneTime,
		EConvertForeignZoneTime,
		EGetLocalEncodedTimeZoneRulesSize,
		EGetLocalOlsenEncodedTimeZoneRulesSize,
		EGetForeignEncodedTimeZoneRulesSize,
		EGetLocalEncodedTimeZoneRules,
		EGetLocalOlsenEncodedTimeZoneRules,
		EGetForeignEncodedTimeZoneRules,
		EGetTimeZoneFormat,
		ERegisterTimeChangeNotifier,
		ECancelRequestforNotice,
		ESrvOpcodeResourceCount,
		ESrvOpcodeSetHeapFailure,
		EGetOffsetsForTimeZoneIds,
		EIsDaylightSavingOn,
		EAutoUpdate,
		EGetUserTimeZoneNamesSize,
		EGetUserTimeZoneNames,
		EGetUserTimeZoneIds,
		EGetUserTimeZoneIdsSize,
		ESwiObsBegin,
		ESwiObsFileChanged,
		ESwiObsEnd,
		ESrvOpcodeResetHeap,
		ESetTimeZone = KCapabilityWriteDeviceData,
		EEnableAutoUpdate,  // Also has WriteDeviceData capability.
		ENotifyHomeTimeZoneChanged,
		ESetHomeTime,
		ESetUnknownZoneTime,
		ECreateUserTimeZone,
        EUpdateUserTimeZone,
        EDeleteUserTimeZone,
        ELocalizationReadCitiesSize = KCapabilityReadUserData,
        ELocalizationReadCitiesTzIdSize,
        ELocalizationReadCitiesInGroupSize,
        ELocalizationReadCities,
        ELocalizationReadFrequentlyUsedZoneSize,
        ELocalizationReadFrequentlyUsedZone,
        ELocalizationReadCachedTimeZoneCitySize,
        ELocalizationReadCachedTimeZoneCity,
        ELocalizationOpenDb = KCapabilityWriteUserData,
        ELocalizationCloseDb,
        ELocalizationWriteCity,
        ELocalizationDeleteCity,
        ELocalizationWriteFrequentlyUsedZone,
        ELocalizationWriteAllFrequentlyUsedZones,
		ENotSupported
		};

public:
	static CTzServer* NewL();
	~CTzServer();
	
private:
	CTzServer(TInt aPriority);
	void ConstructL();

public:
	inline CTzConfigAgent& TimeZoneManager() const;
	inline CTzUserDataDb& UserTimeZoneDb() const; 
	inline CTzLocalizationDb& LocalizationDb() const;
	inline CTzSwiObserver& SwiObserver() const;
	
	void SessionClosed() const;
	void SessionAdded() const;	// required to be const to avoid compiler error
	void Error(TInt aError);

private:
	// from MTZCfgAgentObserver
	void NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aRequester);

	virtual CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;

private:
	mutable TInt iSessionCount;
	CTzConfigAgent* iTimeZoneMgr;
	CTzUserDataDb* iTzUserDataDb;
	CTzLocalizationDb* iTzLocalizationDb;
	CTzSwiObserver* iTzSwiObserver;
	};
	
//
// Active scheduler
//
class CTZSrvScheduler : public CActiveScheduler
	{
public:
	~CTZSrvScheduler();
	static CTZSrvScheduler* NewL();

private:
	// from CActiveScheduler
	virtual void Error(TInt aError) const;

private:
	CTzServer* iServer;
	};

inline CTzConfigAgent& CTzServer::TimeZoneManager() const 
	{
	return *iTimeZoneMgr;
	}
	
inline CTzUserDataDb& CTzServer::UserTimeZoneDb() const 
	{
	return *iTzUserDataDb;
	}

inline CTzLocalizationDb& CTzServer::LocalizationDb() const
	{
	return *iTzLocalizationDb;
	}

inline CTzSwiObserver& CTzServer::SwiObserver() const
	{
	return *iTzSwiObserver;
	}

#endif
