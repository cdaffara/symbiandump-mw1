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

#include "ASSrvServer.h"

// System includes
#include <e32base.h>

// User includes
#include "ASSrvSession.h"
#include "ASSrvStaticUtils.h"

//Only include these in the secured platform 
#include "ASSrvAlarmQueue.h"
#include "ASShdOpCodes.h"


#define KPolicyElementWriteUserData 0
#define KPolicyElementWriteDeviceData 1


const TUint KRangeCount = 5; 

const TInt KOpCodeRanges[KRangeCount] = 
	{	
	KCapabilityNone,
	KCapabilityCustomCheckData, 
	KCapabilityWriteUserData, 
	KCapabilityWriteDeviceData, 
	EASShdOpCodeLast,
	};


const TUint8 KElementsIndex[KRangeCount] =
	{
	CPolicyServer::EAlwaysPass, 	//Allways passing no capability required 0-99
	CPolicyServer::ECustomCheck, 	//Custom check for the Alarm SID 		 100-199
	KPolicyElementWriteUserData,	//Requires WriteUserData				 200-299
	KPolicyElementWriteDeviceData,	//Requires WriteDeviceData				 300-(EASShdOpCodeLast-1)
	CPolicyServer::ENotSupported, 	//Not Supported					 		 EASShdOpCodeLast-End
	};

const CPolicyServer::TPolicyElement KPolicyElements[] = 
	{ 
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteUserData), CPolicyServer::EFailClient},
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient} 
	};

const CPolicyServer::TPolicy KAlarmServerPolicy =
	{
	CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
	KRangeCount,
	KOpCodeRanges,
	KElementsIndex, 	// what each range is compared to 
	KPolicyElements 	// what policies range is compared to
	};
 	


//
// ----> CASSrvServer (source)
//

//*************************************************************************************
CASSrvServer::CASSrvServer()
 	:CPolicyServer(CActive::EPriorityHigh, KAlarmServerPolicy)
	{
	}


//*************************************************************************************
CASSrvServer::~CASSrvServer()
	{
	// In order to prevent access violations as the server
	// shuts down, we inform all sessions that the server is
	// about to be destroyed
	iSessionIter.SetToFirst();
	//
	CASSrvSession* session = static_cast<CASSrvSession*>(iSessionIter++);
	while(session)
		{
		session->HandleServerDestruction();
		session = static_cast<CASSrvSession*>(iSessionIter++);
		}
	//
	delete iServerWideData;
	}


//*************************************************************************************
void CASSrvServer::ConstructL()
	{
	// We "start" the server here (i.e before completing construction) since
	// if two sessions attempt to connect at the same time, we will end up
	// setting up an alarm alert notification with a server instance which is
	// about to be destroyed. Calling StartL here won't actually allow any connections
	// to be created until the active scheduler has a chance to run. However, the 
	// call below will leave should there already be a started alarm server instance.
	//
	StartL(ASSrvStaticUtils::ASName());
	iServerWideData = CASSrvServerWideData::NewL();
	}


//*************************************************************************************
CASSrvServer* CASSrvServer::NewLC()
	{
	CASSrvServer* self = new(ELeave) CASSrvServer();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


//
//
//


//*************************************************************************************
CSession2* CASSrvServer::NewSessionL(const TVersion& aVersion,const RMessage2& /*aMessage*/) const
	{
	// Check client version is correct
	const TVersion alarmServerVersion(ASSrvStaticUtils::ASVersion());
	if	(!User::QueryVersionSupported(alarmServerVersion, aVersion))
		User::Leave(KErrNotSupported);

	// Get non-const reference
	CASSrvServerWideData& nonConstServerWideData = *const_cast<CASSrvServerWideData*>(iServerWideData);

	// Create new session
	CASSrvSession* session = CASSrvSession::NewL(nonConstServerWideData);
	return session;
	}


//
//
//


//*************************************************************************************
TInt CASSrvServer::RunError(TInt aError)
	{
	// A bad descriptor error implies a badly programmed client, so panic it;
	// otherwise report the error to the client
	if	(aError == KErrBadDescriptor)
		{
		ASSrvStaticUtils::PanicClient(Message(), EAlarmServerInitiatedClientPanicBadDescriptor);
		}
	else
		{
		Message().Complete(aError);
		}

	// The leave will result in an early return from CServer2::RunL(), skipping
	// the call to request another message. So do that now in order to keep the
	// server running.
	ReStart();

	// Indicate that we've handled the error fully
	return KErrNone;
	}
	
CPolicyServer::TCustomResult CASSrvServer::CustomSecurityCheckL(const RMessage2& aMsg, TInt& /*aAction*/, TSecurityInfo& /*aMissing*/)
	{
	CPolicyServer::TCustomResult returnValue = CPolicyServer::EFail;	
	
	// Initially, make sure there is alarms in the queue
	const CASSrvAlarmQueue& queue = iServerWideData->Queue();
	if(queue.QueueAlarmCount() > 0)
		{	
		// Get the alarm id from the client
		const TAlarmId alarmId = static_cast<TAlarmId>(aMsg.Int0());
		const TASSrvAlarm* theAlarm = queue.QueueAlarmById(alarmId);
		if (theAlarm)
			{
			//Now get the SID of the alarm, it should be const since we do not want to change the alarm here
			//Check that the SID of the alarm is equal to the SID of the client
			TSecureId AlarmOwnerSID = theAlarm->GetSid();
			TSecureId ClientSID = aMsg.SecureId();
			if (AlarmOwnerSID == ClientSID)
				{
				returnValue = CPolicyServer::EPass;
				}		
			}
		}
		
	// If the return value is not CPolicyServer::EPass that means that the client
	// does not own the alarm (he was not the one creating it)
	if (returnValue != CPolicyServer::EPass)
		{
		// Since different OP Codes require different capabilities
		// we then need to check the OP code
		// It is OK to 
		//		- EASShdOpCodeGetAlarmDetails
		//		- EASShdOpCodeGetAlarmData
		//		- EASShdOpCodeAlarmAtIndex
		// even if the client is not owning the alarm, as long as it has ReadUserData
		const TInt opCode = aMsg.Function();
		switch(opCode)
			{
			case EASShdOpCodeGetAlarmDetails:
			case EASShdOpCodeGetAlarmData:
			case EASShdOpCodeAlarmAtIndex:
				// Check that the client has ReadUserData
				if (aMsg.HasCapability(ECapabilityReadUserData))
					{
					returnValue = CPolicyServer::EPass;
					}
				break;
			case EASShdOpCodeAlarmNotificationCancelAndDeQueue:	
			case EASShdOpCodeAlarmDelete:						
			case EASShdOpCodeSetAlarmStatus:						
			case EASShdOpCodeSetAlarmDayOrTimed:					
			case EASShdOpCodeSetAlarmCharacteristics:			
			case EASShdOpCodeSetClientData:						
			case EASShdOpCodeAlarmDataAttach:					
			case EASShdOpCodeAlarmDataDetach:
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
			case EASShdOpCodeSetAlarmDays:
#endif
				// Check that the client has WriteDeviceData
				if (aMsg.HasCapability(ECapabilityWriteDeviceData))
					{
					returnValue = CPolicyServer::EPass;
					}
				break;
			default:
				if (aMsg.HasCapability(ECapabilityReadUserData, ECapabilityWriteDeviceData))
					{
					// If the Client has booth ReadUserData and WriteDeviceData then just return CPolicyServer::EPass
					returnValue = CPolicyServer::EPass;
					}
				break;		
			}
		}
	return(returnValue);
	}
