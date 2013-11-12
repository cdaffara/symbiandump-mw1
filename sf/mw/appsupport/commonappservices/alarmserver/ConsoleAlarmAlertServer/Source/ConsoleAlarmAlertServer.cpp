// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

 
#include "ConsoleAlarmAlertServer.h"
#include <asaltdefs.h>
#include "ConsoleAlarmAlertSession.h"

// The Console Alarm Alert Server is migrated to a secure server in EKA2
#include <e32base.h>

const TUint KRangeCount = 1; 

const TInt KOpCodeRanges[KRangeCount] = 
	{	
	0 // All Op Codes  from 0 to KMaxInt should pass. No restrictions	
	};


const TUint8 KElementsIndex[KRangeCount] =
	{
	CPolicyServer::EAlwaysPass, 	//All OP Codes Allways passing no capability required 
	};


const CPolicyServer::TPolicy KConsoleAlarmAlertServerPolicy =
	{
	CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
	KRangeCount,
	KOpCodeRanges,
	KElementsIndex, 	// what each range is compared to 
	0					//Since we have no specific policy checking
	};
 	

//
// ----> CConsoleAlarmAlertServer (source)
//

//*************************************************************************************
CConsoleAlarmAlertServer::CConsoleAlarmAlertServer()
 	:CPolicyServer(CActive::EPriorityStandard, KConsoleAlarmAlertServerPolicy), iInstructionSet(NULL)
	, iExtendedMode(EFalse), iMaxAlarms(1), iUserTime(Time::NullTTime())
	{
	}



//*************************************************************************************
CConsoleAlarmAlertServer::~CConsoleAlarmAlertServer()
	{
	delete iAttachment;
	iObservers[EAlarmObserver].Close();
	iObservers[ESoundStartObserver].Close();
	iObservers[ESoundStopObserver].Close();
	iObservers[EVisibleObserver].Close();
	iObservers[EStateObserver].Close();
	iObservers[EDeleteObserver].Close();
	}


//*************************************************************************************
void CConsoleAlarmAlertServer::ConstructL()
	{
	StartL(KAlarmAlertServerName);
	__FLOG_CONNECT;
	__FLOG_CREATE(KLogFile, RFileFlogger::ELogModeOverWrite);
	_LIT(KStartMsg, "Console Alarm Alert Server started");
	__FLOG_INFO1(KStartMsg);
	__FLOG_CLOSE;
	}


//*************************************************************************************
CConsoleAlarmAlertServer* CConsoleAlarmAlertServer::NewLC()
	{
	CConsoleAlarmAlertServer* self = new(ELeave) CConsoleAlarmAlertServer();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

//
//
//


//*************************************************************************************
CSession2* CConsoleAlarmAlertServer::NewSessionL(const TVersion& aVersion,const RMessage2& /*aMessage*/) const
	{
	const TVersion KServerVersion(KASAltVersionMajor, KASAltVersionMinor, KASAltVersionBuild);
	if	(!User::QueryVersionSupported(KServerVersion, aVersion))
		User::Leave(KErrNotSupported);
	//
	return CConsoleAlarmAlertSession::NewL((CConsoleAlarmAlertServer*)this);
	}


//
//
//


//*************************************************************************************

void CConsoleAlarmAlertServer::SetAttachment(HBufC8* data)
	{
	delete iAttachment;
	iAttachment = data;
	}

	

/**
Adds the observer to the list of observers.

@param aIndex type of observer, see CConsoleAlarmAlertServer::TObserverIndex.
@param aObserver observer to add.
*/

void CConsoleAlarmAlertServer::AddObserverL(TObserverIndex aIndex, CConsoleAlarmAlertSession* aObserver)
	{
	iObservers[aIndex].AppendL(aObserver);
	}
	
/**
Removes the alarm observer from the list of alarm observers. It is safe to call this method
even if the observer isn't registered. It will remove all entries of this observer.

@param aIndex type of observer, see CConsoleAlarmAlertServer::TObserverIndex.
@param aObserver observer to remove.
*/
	
void CConsoleAlarmAlertServer::RemoveObserver(TObserverIndex aIndex, CConsoleAlarmAlertSession* aObserver)
	{
	TInt i = iObservers[aIndex].Find(aObserver);
	
	while (i != KErrNotFound) 
		{
		iObservers[aIndex].Remove(i);
		i = iObservers[aIndex].Find(aObserver);
		}
	}

/**
Notifies all registered alarm observers and then clears the list of registered alarm observers.

@param aAlarm alarm this notification is for.
@param aFullName owner of alarm.
@param aData data attached to the alarm.
*/

void CConsoleAlarmAlertServer::NotifyAlarmObserversL(const TASShdAlarm& aAlarm, const TFullName& aFullName, const TDes8& aData)
	{
	TInt count = iObservers[EAlarmObserver].Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iObservers[EAlarmObserver][i]->NotifyAlarmL(aAlarm, aFullName, aData);
		}
		
	iObservers[EAlarmObserver].Reset(); // Objects not deleted, array 'truncated'
	}

/**
Notifies all registered sound start observers and then clears the list of registered sound start observers.

@param aId id of alarm this event affects.
@param aFilename sound file to play.
*/

void CConsoleAlarmAlertServer::NotifySoundStartObserversL(const TAlarmId& aId, const TDesC& aFilename)
	{
	TInt count = iObservers[ESoundStartObserver].Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iObservers[ESoundStartObserver][i]->NotifySoundStartL(aId, aFilename);
		}
	
	iObservers[ESoundStartObserver].Reset(); // Objects not deleted, array 'truncated'
	}
	
/**
Notifies all registered sound stop observers and then clears the list of registered sound stop observers.

@param aId id of alarm this event affects.
*/

void CConsoleAlarmAlertServer::NotifySoundStopObserversL(const TAlarmId& aId)
	{
	TInt count = iObservers[ESoundStopObserver].Count();

	for (TInt i = 0; i < count; ++i)
		{
		iObservers[ESoundStopObserver][i]->NotifySoundStopL(aId);
		}
	
	iObservers[ESoundStopObserver].Reset(); // Objects not deleted, array 'truncated'
	}

/**
Notifies all registered visible observers and then clears the list of registered visible observers.

@param aId id of alarm this event affects.
@param aVisible visible status of the alarm.
*/

void CConsoleAlarmAlertServer::NotifyVisibleObserversL(const TAlarmId& aId, const TBool aVisible)
	{
	TInt count = iObservers[EVisibleObserver].Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iObservers[EVisibleObserver][i]->NotifyVisibleL(aId, aVisible);
		}
		
	iObservers[EVisibleObserver].Reset(); // Objects not deleted, array 'truncated'
	}

/**
Notifies all registered state observers and then clears the list of registered state observers.

@param aId id of alarm this event affects.
@param aState new state of alarm.
*/

void CConsoleAlarmAlertServer::NotifyStateObserversL(const TAlarmId& aId, const TInt aState)
	{
	TInt count = iObservers[EStateObserver].Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iObservers[EStateObserver][i]->NotifyStateL(aId, aState);
		}
		
	iObservers[EStateObserver].Reset(); // Objects not deleted, array 'truncated'
	}

/**
Notifies all registered delete alarm observers and then clears the list of registered delete alarm observers.

@param aId id of alarm this event affects.
*/
	
void CConsoleAlarmAlertServer::NotifyDeleteObserversL(const TAlarmId& aId)
	{
	TInt count = iObservers[EDeleteObserver].Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iObservers[EDeleteObserver][i]->NotifyDeleteL(aId);
		}
		
	iObservers[EDeleteObserver].Reset(); // Objects not deleted, array 'truncated'
	}

