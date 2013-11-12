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

#ifndef __TZ_CONFIG_AGENT_H__
#define __TZ_CONFIG_AGENT_H__

#include <e32base.h>
#include <e32property.h> // RProperty
#include <e32std.h>
#include <s32file.h>
#include <tz.h>
#include "mtimezonedataaccess.h"
#include "tzuserdata.h" 
#include "dataprovider.h"
#include "MBackupRestoreNotificationObserver.h"

class CTzConfigAgent;
class CRepository;
class CEnvironmentChangeNotifier;
class CVTzActualisedRules;
class CTzUserDataDb;
class CBackupRestoreNotification;	
class CTzLocalizationDb;
//-------------------------------------------------------------------------
/** MDstEventObserver
Defines an interface for observing DST change events.
Note that these methods will be called from within a RunL() method.

@internalComponent
@since 9.1
*/
class MDstEventObserver
    {
public:
    virtual void HandleSystemTimeChangeL() = 0;
    virtual void HandleDstTimerCompleteL() = 0;
    virtual void HandleDstTimerCancellationL() = 0;
    virtual void DstTimerErrorOccurredL(TInt aError) = 0;
    };

//-------------------------------------------------------------------------
/** CDstEventNotifier 
A timer for the next DST change event. 
When the timer expires, the observer is notified.
Note that times are expressed using UTC.

@internalComponent
@since 9.1
*/
class CDstEventNotifier : public CTimer
    {
public: // Construction.
    static CDstEventNotifier* NewL(MDstEventObserver& aObserver);
    
public: // Methods.
    void SetTimer(const TTime& aUtcTime);
    
private:    // Construction.
    CDstEventNotifier(MDstEventObserver& aObserver);
    
private:    // From CTimer
    void RunL();
    TInt RunError(TInt aError);
    void DoCancel();
    
private:    // Attributes.
    MDstEventObserver& iObserver;
    };
    
//-------------------------------------------------------------------------
/** MTZCfgAgentObserver
Defines an interface for observing CTzConfigAgent status
changes

@internalComponent
*/
class MTZCfgAgentObserver
{
public:
	// notifies observer about any change to Time Zone System
	virtual void NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aRequester) = 0;
};


//-------------------------------------------------------------------------
/** CTZRulesCache
Encapsulates a Time zone ID and its cache of rules 

@internalComponent
@since 9.1
*/
class CTZRulesCache : public CBase
{
public:
	static CTZRulesCache* NewL(const CTzId& aTimeZoneId, 
								CTzConfigAgent& iTimeZoneConfigAgent,
								const TTime& aTime);

	CTzRules& GetEncodedTimeZoneRulesL();
	TInt GetEncodedTimeZoneRulesSizeL(const TTime& aStartTime, const TTime& aEndTime, TTzTimeReference aTimeRef);
	CVTzActualisedRules& GetTimeZoneRulesL(const TTime& aTime,TTzTimeReference aTimerRef);

	inline CTzId& TimeZoneId() const;
	inline CVTzActualisedRules& Rules() const;

	~CTZRulesCache();

protected:
	void ConstructL(const CTzId& aTimeZoneId, const TTime& aTime);
	const CTzRules& doGetEncodedTimeZoneRulesL(TInt aStartYear, TInt aEndYear);	

    void doGetActualisedTimeZoneRulesL(const TTime& aTime);

	// template method used to specialise the construction of different
	// subclass objects.
	virtual void SetDefaultZoneIdL();
	inline CTZRulesCache(CTzConfigAgent& aTimeZoneConfigAgent);
	TBool RuleApplies(CVTzActualisedRules& actRules, const TTime& aTime, TTzTimeReference aTimerRef) const;

protected:
	CTzId* iTimeZoneId;		// time zone Id
	CTzRules* iEncodedRules;
	CTzConfigAgent& iTimeZoneConfigAgent;
	CVTzActualisedRules* iActualisedRules; // caches the last used rule for zone
};


//-------------------------------------------------------------------------
/** CSystemTzRulesCache
Encapsulates the current System Time zone ID and its cache of rules  

@internalComponent
@since 9.1
*/
class CSystemTzRulesCache : public CTZRulesCache
{
public:
	static CSystemTzRulesCache* NewL(const CTzId& aTimeZoneId, 
								CTzConfigAgent& iTimeZoneConfigAgent,
								const TTime& aTime);

	~CSystemTzRulesCache();

protected:

	// from CTZRulesCache
	virtual void SetDefaultZoneIdL();

private:
	inline void ConstructL(const CTzId& aTimeZoneId, const TTime& aTime);
	inline CSystemTzRulesCache(CTzConfigAgent& aTimeZoneConfigAgent);

};


//-------------------------------------------------------------------------
/** CTzConfigAgent
Sets the System Time Zone;
Caches Time Zone rule for the current system timezone 
and one other timezone

IMPORTANT NOTE:
Although this class is used in a Server, it is designed to be Server-environment
agnostic for portability reasons (so it could be used within a server, plug-in, etc). 
It should not know about CServers, CSessions, RMessages or their derivatives.
These structures find existence in the client/server boundary and should be left there.
For this same reason MTZCfgAgentObserver is used to represent the class creating 
this object. The RMessage used to deal with Asynchronous request completions are 
left with the sessions that received them. 

@internalComponent
*/
class CTzConfigAgent : public CBase, public MTzDataObserver, public MDstEventObserver, public MTzUserDataChangeObserver, public MBackupRestoreNotificationObserver
	{
public:
	static CTzConfigAgent* NewL(MTZCfgAgentObserver& aServer, CTzUserDataDb& aTzUserDataDb,
		CTzSwiObserver& aTzSwiObserver);

	void ConvertL(const CTzId& aZone, TTime& aTime, TTzTimeReference aTimerRef);
	void ConvertL(TTime& aTime, TTzTimeReference aTimerRef);
	TInt IsDaylightSavingOnL(const CTzId& aZone, TTime& aUTCTime);
	const CTzId& GetTimeZoneIdL() const;
	const CTzId& SetTimeZoneL(const CTzId& aZone, const TAny* aRequester, TBool aUpdateCentralRepository, 
								TBool aForceCacheUpdate = EFalse);
	CTzRules& GetEncodedTimeZoneRulesL();
	TInt GetEncodedTimeZoneRulesSizeL(const TTime& aStartTime, const TTime& aEndTime, 
									  TTzTimeReference aTimeRef);

	CTzRules& GetForeignEncodedTimeZoneRulesL();
	TInt GetForeignEncodedTimeZoneRulesSizeL(const CTzId& aZoneId, const TTime& aStartTime,
										const TTime& aEndTime, TTzTimeReference aTimeRef);
#if defined(_DEBUG)
	void ResetForeignTimeZoneRulesCache(void);
#endif
	CVTzActualisedRules& GetTimeZoneRulesL(const TTime& aTime,TTzTimeReference aTimerRef);
    CVTzActualisedRules& GetForeignTimeZoneRulesL(const CTzId& aZoneId, const TTime& aTime,
                                                  TTzTimeReference aTimerRef);
	CBufFlat* GetTimeZoneFormatL(const TTime& aTime, TTzTimeReference aTimerRef);
	void GetOffsetsForTimeZoneIdsL(CBufFlat& aTimeZoneIdArray);

	~CTzConfigAgent();
	
	inline CTzDataProvider& TzDataProvider();
	inline void SetLocalizationDb(CTzLocalizationDb* aTzLocalizationDb);
	inline TBool TzDbHasChanged();

public:    // New public method for PREQ 234 - Auto-update and notification.
    void SetAutoUpdateBehaviorL(TInt aUpdateEnabled);
	TInt AutoUpdateSetting();
	
		//Observers
	void AddObserverL(MTzDataObserver* aChangeObs);
 	void RemoveObserver(MTzDataObserver* aChangeObs);
		
public:    // One goal fixing for INC077804
    TInt SetHomeTimeL(const TTime& aHomeTime);
    TInt SetUnknownTimeZoneTimeL(const TTime& aUTCTime, const TInt aUTCOffset, TBool aPersistInCenRep);

private:
	CTzConfigAgent(MTZCfgAgentObserver& aServer, CTzUserDataDb& aTzUserDataDb, CTzSwiObserver& aTzSwiObserver);
	void ConstructL();
	
	// from MTzDataObserver
	virtual void NotifyTZDataStatusChangeL(RTz::TTzChanges aChange);	

private:    
    void SetDstChangeTimerL();
    void HandleDstChangeTimerL();
    void UpdateUtcOffsetL();
    TBool CalculateNextDstChangeL(TTime& aNextDstEvent);
    TBool CalculateNextDstChangeL(TTime& aNextDstEvent, TTime& aPreviousDstEvent);
    void CalculateUtcOffsetL(TTimeIntervalSeconds& aOffset);
    
private:
	//From MTzUserDataChangeObserver
    virtual void NotifyUserTzRulesChange(TTzUserDataChange aChange);
    virtual void NotifyUserTzNamesChange(TTzUserDataChange aChange);
   
    void PublishNewDstChangeInfoL();
    void NotifyUserTzRulesChangeL(TTzUserDataChange aChange);
    void PublishTzDbChangeInfoL();
    void ReadTimeStampsL(RFile& aFile, TTime& aTzdbFileTime, TTime& aUserDbFileTime, TTime& aResourceFileTimes);
    void WriteTimeStampsL(RFile& aFile, const TTime& aTzdbFileTime, const TTime& aUserDbFileTime, const TTime& aResourceFileTimes);
    TTime GetResourceFileTimeStampsL();
    
#ifdef SYMBIAN_TZ_UNIT_TEST
    // Allow access to CalculateNextDstChangeL() for unit testing.
    friend class CTZConfigAgentTest;
#endif  // SYMBIAN_TZ_UNIT_TEST

private:    // From MDstEventObserver (for PREQ 234 - Auto-update and notification).
    void HandleSystemTimeChangeL();
    void HandleDstTimerCompleteL();
    void HandleDstTimerCancellationL();
    void DstTimerErrorOccurredL(TInt aError);
    static TBool EnvironmentChangeCallBackL(TAny* aSelf);
    
    // From MBackupRestoreNotificationObserver to handle backup and restore operations.
    virtual void BackupBeginningL();
    virtual void BackupCompletedL();
    virtual void RestoreBeginningL();
    virtual void RestoreCompletedL(TInt aRestoreResult);
	
	
private:    // Enumerations.
    // Central Repository keys - see 1020383e.txt.
    // Values can be read by any client, but these values are not published.
    enum TRepositoryKeys
        {
        /** DST auto-update configuration. */
        EConfigurationKey = 0x00000001,
        /** Current time zone numeric identifier. */
        ETimeZoneKey = 0x00000002,
        /** Default Home time zone numeric identifier. */
        EDefaultHomeTimeZoneKey = 0x00000003,
        /** Default Interest time zone numeric identifier. */
        EDefaultInterestTimeZoneKey = 0x00000004,
        /** Default Recent1 time zone numeric identifier. */
        EDefaultRecent1TimeZoneKey = 0x00000005,
        /** Default Recent2 time zone numeric identifier. */
        EDefaultRecent2TimeZoneKey = 0x00000006
        };
    
private:
	MTZCfgAgentObserver& iTzServer;
	RFs iFs;
	CTzDataProvider* iTzDataProvider;
	CSystemTzRulesCache* iSystemTzCache;			// caches system zone and rules
	CTZRulesCache* iOtherTzCache;					// caches alternative zone and rules		

    // For PREQ 234 - Auto-update and notification.
    TInt iEnabled;                 // DST Auto-update configuration.
    CRepository* iRepository;       // Repository for persisting configuration.
    CDstEventNotifier* iDstTimer;   // DST Change Timer.
    CEnvironmentChangeNotifier* iChangeNotifier;    // For notification of system/locale changes.
    CTzSwiObserver& iTzSwiObserver;
    TBool iShuttingDown;        // Do not set a new timer when this flag is set.
	CTzUserDataDb& iTzUserDataDb;
	CTzLocalizationDb* iTzLocalizationDb;// Not owned
	// support for BackupRestore
	CBackupRestoreNotification* iBackupNotifier;
	RPointerArray<MTzDataObserver> iChangeObservers;
	TBool iTzDbHasChanged;
	};


//
inline CVTzActualisedRules& CTZRulesCache::Rules() const
	{
	return *iActualisedRules;
	}

inline CTZRulesCache::CTZRulesCache(CTzConfigAgent& aTimeZoneConfigAgent) :
	iTimeZoneConfigAgent(aTimeZoneConfigAgent)
	{
	}

inline CTzId& CTZRulesCache::TimeZoneId() const 
	{
	return *iTimeZoneId;
	}

inline void CSystemTzRulesCache::ConstructL(const CTzId& aTimeZoneId, const TTime& aTime)
	{
	CTZRulesCache::ConstructL(aTimeZoneId, aTime);
	}

inline CSystemTzRulesCache::CSystemTzRulesCache(CTzConfigAgent& aTimeZoneConfigAgent) :
		CTZRulesCache(aTimeZoneConfigAgent)
	{
	}

inline CTzDataProvider& CTzConfigAgent::TzDataProvider()
	{
	return *iTzDataProvider;
	}

inline void CTzConfigAgent::SetLocalizationDb(CTzLocalizationDb* aTzLocalizationDb)
	{
	iTzLocalizationDb = aTzLocalizationDb;
	}

inline TBool CTzConfigAgent::TzDbHasChanged()
	{
	return iTzDbHasChanged;
	}

#endif  // __TZ_CONFIG_AGENT_H__
