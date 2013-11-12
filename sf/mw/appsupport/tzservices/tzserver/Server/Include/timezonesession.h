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

#ifndef __TIMEZONE_SESSION_H__
#define __TIMEZONE_SESSION_H__

#include <e32base.h>
#include "tzuserdatacache.h"

class CTzServer;

/*
@internalComponent
*/
class CTzServerSession : public CSession2
	{
public:
	static CTzServerSession* NewL();
	~CTzServerSession();
	
private:
	CTzServerSession();
	void ConstructL();

public:
	// completes a pending client notifier
	void NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aRequester);

private:	// from CSession2
	void ServiceL(const RMessage2& aMessage);
	
private:
	// Registers to receive notification to time and zone changes in the
	// Server
	TInt doRegisterTzChangeNotifier(const RMessage2& aMessage);
	TInt doCancelRequestForTzChangeNoticeL(const RMessage2& aMessage);
	TInt doSetTimeZoneL(const RMessage2& aMessage);
	TInt doGetLocalTimeZoneIdL(const RMessage2& aMessage);
	TInt doGetLocalEncodedTimeZoneRulesL(const RMessage2& aMessage);
	TInt doGetLocalEncodedTimeZoneRulesSizeL(const RMessage2& aMessage);
	TInt doGetForeignEncodedTimeZoneRulesL(const RMessage2& aMessage);
	TInt doGetForeignEncodedTimeZoneRulesSizeL(const RMessage2& aMessage);
	TInt doGetTimeZoneFormatL(const RMessage2& aMessage);
	TInt doConvertLocalZoneTimeL(const RMessage2& aMessage);
	TInt doConvertForeignZoneTimeL(const RMessage2& aMessage);
	TInt doGetOffsetsForTimeZoneIdsL(const RMessage2& aMessage) const;
    TInt doSetAutoUpdateBehaviorL(const RMessage2& aMessage);
	TInt doIsDaylightSavingOnL(const RMessage2& aMessage);
	TInt doAutoUpdateSettingL(const RMessage2& aMessage);
	TInt doNotifyHomeTimeZoneChanged(const RMessage2& aMessage);
	TInt doSetHomeTimeL(const RMessage2& aMessage);
	TInt doSetUnknownZoneTimeL(const RMessage2& aMessage);
	TInt doCreateUserTimeZoneL(const RMessage2& aMessage);
    TInt doUpdateUserTimeZoneL(const RMessage2& aMessage);
    TInt doDeleteUserTimeZoneL(const RMessage2& aMessage);
    TInt doGetUserTimeZoneNamesL(const RMessage2& aMessage);
    TInt doGetUserTimeZoneNamesSizeL(const RMessage2& aMessage);
    TInt doGetUserTimeZoneIdsL(const RMessage2& aMessage);
    TInt doGetUserTimeZoneIdsSizeL(const RMessage2& aMessage);
    TInt doGetHeapSizeL(const RMessage2& aMessage);
    TInt doLocalizationReadCitiesSizeL(const RMessage2& aMessage);
    TInt doLocalizationReadCitiesTzIdSizeL(const RMessage2& aMessage);
    TInt doLocalizationReadCitiesInGroupSizeL(const RMessage2& aMessage);
    TInt doLocalizationReadCitiesL(const RMessage2& aMessage);
    TInt doLocalizationReadFrequentlyUsedZoneSizeL(const RMessage2& aMessage);
    TInt doLocalizationReadFrequentlyUsedZoneL(const RMessage2& aMessage);
    TInt doLocalizationReadCachedTimeZoneCitySizeL(const RMessage2& aMessage);
    TInt doLocalizationReadCachedTimeZoneCityL(const RMessage2& aMessage);
    TInt doLocalizationCloseDb();
    TInt doLocalizationOpenDbL();
    TInt doLocalizationWriteCityL(const RMessage2& aMessage);
    TInt doLocalizationDeleteCityL(const RMessage2& aMessage);
    TInt doLocalizationWriteFrequentlyUsedZoneL(const RMessage2& aMessage);
    TInt doLocalizationWriteAllFrequentlyUsedZonesL(const RMessage2& aMessage);
    TInt doSwiObsBeginL(const RMessage2& aMessage);
    TInt doSwiObsFileChangedL(const RMessage2& aMessage);
    TInt doSwiObsEndL(const RMessage2& aMessage);
    const CTzServer* TzServer() const;
    static void CleanupTimeZonePointerArray(TAny* aArray);
    static void CleanupCityPointerArray(TAny* aArray);

private:
	RMessage2 iClientMessage;	// last client requesting change notification
	TBool iPendingRequest;
	CTzUserDataCache* iTzUserDataCache;
	/**
	Used in the functions where the client first requests the size of the result and then
	gets the result once it has allocated a buffer that can hold the result 
	(see doLocalizationReadCitiesL).
	*/
	RPointerArray<CTzLocalizedCityRecord> iCitiesResultCache;
	/**
	Used to cache the result for doLocalizationReadFrequentlyUsedZoneL.
	*/
	CTzLocalizedTimeZoneRecord* iTimeZoneResultCache;
	/**
	Used to cache the result for doLocalizationReadCachedTimeZoneCityL.
	*/
	CTzLocalizedCityRecord* iCityResultCache;
	};
	
#endif
