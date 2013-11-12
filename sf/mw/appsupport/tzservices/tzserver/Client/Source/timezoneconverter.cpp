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

#include <tz.h>
#include "tzrules.h"
#include <vtzrules.h>
#include <tzconverter.h>
#include "tzruleholder.h"
#include "tzchangenotifier.h"

//
// CTzRuleHolder
//

CTzRuleHolder::CTzRuleHolder( RTz& aTzServer ) :
    iTzServer (aTzServer)
	{
	}

CTzRuleHolder::~CTzRuleHolder()
	{
	delete iTzChangeNotifier;
	delete iTzActualisedRules;
	delete iTzRules;
	}

CTzRuleHolder* CTzRuleHolder::NewL( RTz& aTzServer )
	{
	CTzRuleHolder* self = new(ELeave) CTzRuleHolder( aTzServer );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
    }

void CTzRuleHolder::ConstructL()
    {
	// Must be connected.
	if (!iTzServer.Handle())
	    {
	    User::Leave( KErrNotReady );
	    }

	CTzId* tzId = iTzServer.GetTimeZoneIdL();
	SetCurrentTzId(tzId->TimeZoneNumericID());
	delete tzId;
	
    iTzChangeNotifier = CTzChangeNotifier::NewL( *this );
    iTzChangeNotifier->StartObserving();
    }

/**
Clears the cache.
*/
void CTzRuleHolder::ClearRules()
	{
	delete iTzActualisedRules;
	iTzActualisedRules = NULL;
	delete iTzRules;
	iTzRules = NULL;
	iCurrentTzId = 0;
	}




void CTzRuleHolder::DoConvertL(TTime& aTime, TTzTimeReference aTimerRef)
	{
	// The change notifier may not have had a chance to run yet
	// so check if it is ready to run now to force the update of the client side cache
	iTzChangeNotifier->RunIfReadyL();
	
	if (!(iTzRules && iTzRules->RulesApply(aTime) ) )
		{
		if (!iTzRules)

			{
			TTime startTime(TDateTime(0,EJanuary,0,0,0,0,0));
			TTime endTime(TDateTime(KMaxYear,EDecember,30,23,59,59,0));
			iTzRules = iTzServer.GetTimeZoneRulesL(startTime, endTime, aTimerRef);
			}
		}
		
	// adjust for aTimerRef?
	TDateTime dateTime = aTime.DateTime();
	TUint year = dateTime.Year();
	if (!(iTzActualisedRules 
			&& year >= iTzActualisedRules->StartYear() 
			&& year <= iTzActualisedRules->EndYear() ) )
		{
		// avoid over/under flow
		TUint startYear = year;
		TUint endYear = startYear;
		if (startYear >= (KRuleCacheLowerLimit+1))
			{
			startYear -= (KRuleCacheLowerLimit+1);
			}
		if (endYear < (KMaxTUint - KRuleCacheUpperLimit))
			{
			endYear += KRuleCacheUpperLimit;
			}

		CVTzActualisedRules* newRules = CVTzActualisedRules::NewL(startYear, endYear);
		CleanupStack::PushL(newRules);
		iTzRules->GetActualisedRulesL(*newRules);
		CleanupStack::Pop(newRules);

		delete iTzActualisedRules;
		iTzActualisedRules = newRules;
		}
		
	User::LeaveIfError(iTzRules->ConvertTime(*iTzActualisedRules, aTime, aTimerRef));
	}

/**
Handles notifications from the Time Zone Server
if the Time Zone Database Changes or the System Time Zone is Changed then
the cached rules are no longer valid. Discard them.
*/
void CTzRuleHolder::NotifyTimeZoneChangeL(RTz::TTzChanges aChange)
	{
	User::LeaveIfError(aChange);

	if (aChange == RTz::ETZDatabaseChanged ||
		aChange == RTz::ETZSystemTimeZoneChanged)
		{
		ClearRules();
		CTzId* tzId = iTzServer.GetTimeZoneIdL();
		SetCurrentTzId(tzId->TimeZoneNumericID());
		delete tzId;
		}
		
	// re-request notification
	iTzChangeNotifier->StartObserving();

	}
	
//
// CTzChangeNotifier
//
CTzChangeNotifier::~CTzChangeNotifier()
	{
	Cancel();
	}

CTzChangeNotifier* CTzChangeNotifier::NewL( CTzRuleHolder& aRulesHolder )

	{
	CTzChangeNotifier* self = new(ELeave) CTzChangeNotifier( aRulesHolder );
	CActiveScheduler::Add(self);
	return self;
	}

/**
Starts observing for changes to the time zone from the the Time Zone Server.
@return KErrNotReady if already active, KErrNotReady if session not
open, otherwise KErrNone.
*/
TInt CTzChangeNotifier::StartObserving()
	{
	if (IsActive())
		{
		return KErrNotReady;
		}

	iStatus = KRequestPending;
	SetActive();
	iRulesHolder.Server().RegisterTzChangeNotifier(iStatus);
	return KErrNone;
	}

/**
Stops Observing the Time Zone Server.
*/
void CTzChangeNotifier::StopObserving()
	{
	Cancel();
	}

void CTzChangeNotifier::RunIfReadyL()
	{
	if (IsActive() && iStatus != KRequestPending)
		{
		RunL();
		Cancel();
		StartObserving();
		}
	}

void CTzChangeNotifier::RunL()
	{
	iRulesHolder.NotifyTimeZoneChangeL( static_cast<RTz::TTzChanges>(
	    iStatus.Int()) );
	}

void CTzChangeNotifier::DoCancel()
	{
	iRulesHolder.Server().CancelRequestForNotice();
	}

//
// CTzConverter
//
const RTz& CTzConverter::Server() const
	{
	return iTzServer;
	}

CTzConverter::CTzConverter(RTz& aTzServer) : iTzServer(aTzServer)
	{
	}

void CTzConverter::ConstructL()
	{
	// Caching is now done through RTz so enable its caching. This will
	// do nothing if connected, but if unconnected, then this will leave 
	// with KErrNotReady.
	iTzServer.StartCachingL();

	}

// Handles notifications from the Time Zone Server
// if the Time Zone Database Changes or the System Time Zone is Changed then
// the cached rules is no longer valid. discard it.
void CTzConverter::NotifyTimeZoneChangeL(RTz::TTzChanges /*aChange*/)
	{
	// No longer does anything. This is done in RTz.
	}

/** 
CTzConverter factory method.

Provides client side caching for conversions in the local time zone. Caching 
is done through RTz but enabled through this interface. When caching has been 
enabled by this class and the RTz functions for local time zone conversion are
used, they too will use the cache instead of the server. Multiple CTzConverter's
can use the same RTz session. Each converter will be notified by the server if
the time zone changes, and the cache will be updated appropriately.


To disable caching in RTz, the session must be closed. Caching will be 
re-enabled if a CTzConverter associated with the RTz is later used again 
for local time zone conversions.
 
This will leave with KErrNotReady if the RTz passed in is not connected.

@param aTzServer Connection to the time zone server. 
@return A pointer to the new CTzConverter object. The caller takes ownership.
*/
EXPORT_C CTzConverter* CTzConverter::NewL(RTz& aTzServer)
	{
	CTzConverter* self = new(ELeave) CTzConverter(aTzServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Destructor.
*/
EXPORT_C CTzConverter::~CTzConverter()
	{
	}




/** 
Converts a time from UTC to local (wall-clock) time 
for the current system time zone.
@param aTime On entry, contains the UTC time to be converted 
and the converted value on exit. 
@return KErrNone if successful, otherwise another system error code.
*/
EXPORT_C TInt CTzConverter::ConvertToLocalTime(TTime& aTime)
	{


	// Ensure that caching is enabled. It will be disabled if the TZ server
	// has become disconnected since this object was created.
	TRAPD( error, iTzServer.StartCachingL() );
	if (error == KErrNone)
	    {
	    return iTzServer.ConvertToLocalTime( aTime );
	    }
	else
	    {
	    return error;
	    }
	}

/** 
Converts a time from UTC to the local (wall-clock) time 
for the specified time zone.

@param aTime On entry, contains the UTC time to be converted, 
and the converted value on exit.
@param aZone Time zone ID.
@return KErrNone if successful, otherwise another system error code.
*/
EXPORT_C TInt CTzConverter::ConvertToLocalTime(TTime& aTime,
													 const CTzId& aZone)
	{
	return (iTzServer.ConvertToLocalTime(aTime, aZone));
	}

/** 
Converts a local (wall-clock) time for the current system time zone into UTC.
@param aTime On entry, contains the local time to be converted, 
and the converted value on exit.
@return KErrNone if successful, otherwise another system error code.
*/
EXPORT_C TInt CTzConverter::ConvertToUniversalTime(TTime& aTime)
	{


	// Ensure that caching is enabled. It will be disabled if the TZ server
	// has become disconnected since this object was created.
	TRAPD( error, iTzServer.StartCachingL() );
	if (error == KErrNone)
	    {
    	return iTzServer.ConvertToUniversalTime( aTime );
    	}
    else
        {
        return error;
        }
	}

/** 
Converts a local (wall-clock) time for the specified time zone into UTC.
@param aTime On entry, contains the local time to be converted, 
and the converted value on exit. 
@param aZone The time zone ID.
@return KErrNone if successful, otherwise another system error code.
*/
EXPORT_C TInt CTzConverter::ConvertToUniversalTime(TTime& aTime,
													const CTzId& aZone)
	{

	return (iTzServer.ConvertToUniversalTime(aTime, aZone));
	}

/** 
Get the numerical ID of the current time zone.
@return The time zone ID.
*/
EXPORT_C TUint16 CTzConverter::CurrentTzId()
	{
	TUint16 tzId = iTzServer.CurrentCachedTzId();
	return tzId;
	}
