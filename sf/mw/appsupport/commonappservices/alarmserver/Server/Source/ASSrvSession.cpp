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

#include "ASSrvSession.h"

// System includes
#include <s32mem.h>

// User includes
#include "ASShdPragma.h"
#include "ASShdOpCodes.h"
//
#include "ASSrvTimer.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvSessionEngine.h"
#include "ASSrvSoundSettings.h"
#include "ASSrvServerWideData.h"
#include "ASSrvSoundController.h"
#include "ASSrvAlarmSoundDetails.h"
#include "ASSrvNotificationCoordinator.h"

#ifdef _DEBUG
#include "ASSrvEnvironmentChangeManager.h"
#endif 

// Type definitions

// Constants
const TInt KAlarmServerTransferBufferExpandSize = 100;
const TInt KSlot0 = 0;
const TInt KSlot1 = 1;
const TInt KSlot2 = 2;

// Enumerations

// Classes referenced


//
// ----> CASSrvSession (source)
//

//*************************************************************************************
CASSrvSession::CASSrvSession(CASSrvServerWideData& aServerWideData)
:	iServerWideData(aServerWideData), iAlarmSink(aServerWideData)
	{
	}


//*************************************************************************************
CASSrvSession::~CASSrvSession()
	{
	HandleServerDestruction();
	//
	delete iTransferBuffer;
	//
	iChangeEventBuffer.Close();
	}


//*************************************************************************************
void CASSrvSession::ConstructL()
	{
	iTransferBuffer = CBufFlat::NewL(KAlarmServerTransferBufferExpandSize);
	iSessionEngine = CASSrvSessionEngine::NewL(ServerData(), *this, *this);
	}


//*************************************************************************************
CASSrvSession* CASSrvSession::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvSession* self = new(ELeave) CASSrvSession(aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//


//*************************************************************************************
/**
 * Called by the server's destructor. We need to be told that the server is
 * being destroyed, since the server owns the server-wide data, and this class
 * must destroy the session engine (which will unregister for various notifications)
 * using the server wide data instance.
 */
void CASSrvSession::HandleServerDestruction()
	{
	delete iSessionEngine;
	iSessionEngine = NULL;
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAnyEventObserver
 */
void CASSrvSession::MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId)
	{
	CompleteChangeNotificationMessage(aEvent, aAlarmId);
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvSession
 *
 * Facade to the real session
 */
TASSrvSessionId CASSrvSession::MASSrvSessionId() const
	{
	// This shouldn't ever be called
	__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultFacadeInterfaceError));
	return KNullSessionId;
	}


//*************************************************************************************
/**
 * @see MASSrvSession
 *
 * Facade to the real session
 */
void CASSrvSession::MASSrvSessionFullName(TDes& aDes) const
	{
	aDes.Zero();
	}


//
//
//


//*************************************************************************************
void CASSrvSession::ServiceL(const RMessage2& aMessage)
	{
	// Leave's are caught by CASSrvServer::RunError
	iMessage=&aMessage;
	const TBool completeMessage = DoServiceL();
	if	(completeMessage)
		aMessage.Complete(KErrNone);
	iMessage=NULL;
	}


//
//
//


//*************************************************************************************
TBool CASSrvSession::DoServiceL()
	{
	// Reset alarm sink to remove old data
	iAlarmSink.Reset();

	//
	// POSSIBLY OPTIMISE THIS LOT INTO A LOOKUP TABLE?
	//
	TBool completeMessage = EFalse;
	const TInt opCode = Message().Function();
	//
	switch(opCode)
		{
	//
	// CMD - ALARM SPECIFIC FUNCTIONALITY
	case EASShdOpCodeAlarmAdd:
		completeMessage = CmdAlarmAddL();
		break;
	case EASShdOpCodeAlarmAddWithNotification:
		completeMessage = CmdAlarmAddWithNotificationL();
		break;
	case EASShdOpCodeAlarmNotificationCancelAndDeQueue:
		completeMessage = CmdAlarmNotificationCancelAndDeQueueL();
		break;
	case EASShdOpCodeGetAlarmDetails:
		completeMessage = CmdGetAlarmDetailsL();
		break;
	case EASShdOpCodeAlarmDelete:
		completeMessage = CmdAlarmDeleteL();
		break;
	case EASShdOpCodeGetAlarmCategory:
		completeMessage = CmdGetAlarmCategoryL();
		break;
	case EASShdOpCodeGetAlarmOwner:
		completeMessage = CmdGetAlarmOwnerL();
		break;
	case EASShdOpCodeSetAlarmStatus:
		completeMessage = CmdSetAlarmStatusL();
		break;
	case EASShdOpCodeSetAlarmStatusForCalendarFile:
        completeMessage = CmdSetAlarmStatusForCalendarFileL();
        break;		
	case EASShdOpCodeGetAlarmStatus:
		completeMessage = CmdGetAlarmStatusL();
		break;
	case EASShdOpCodeSetAlarmDayOrTimed:
		completeMessage = CmdSetAlarmDayOrTimedL();
		break;
	case EASShdOpCodeGetAlarmDayOrTimed:
		completeMessage = CmdGetAlarmDayOrTimedL();
		break;
	case EASShdOpCodeGetAlarmCharacteristics:
		completeMessage = CmdGetAlarmCharacteristicsL();
		break;
	case EASShdOpCodeSetAlarmCharacteristics:
		completeMessage = CmdSetAlarmCharacteristicsL();
		break;
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	case EASShdOpCodeSetWakeup:
		completeMessage = CmdSetWakeupL();
		break;
#endif
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	case EASShdOpCodeGetAlarmDays:
		completeMessage = CmdGetAlarmDaysL();
		break;
	case EASShdOpCodeSetAlarmDays:
		completeMessage = CmdSetAlarmDaysL();
		break;
	case EASShdOpCodeSetContinuous:
		completeMessage = CmdSetContinuousL();
		break;
	case EASShdOpCodeGetContinuous:
		completeMessage = CmdContinuousL();
		break;
#endif
	case EASShdOpCodeOrphanAlarm:
		completeMessage = CmdOrphanAlarmL();
		break;
	case EASShdOpCodeSetClientData:
		completeMessage = CmdSetClientDataL();
		break;
	//
	// CMD - ALARM DATA FUNCTIONALITY
	case EASShdOpCodeAlarmDataAttach:
		completeMessage = CmdAlarmDataAttachL();
		break;
	case EASShdOpCodeAlarmDataDetach:
		completeMessage = CmdAlarmDataDetachL();
		break;
	case EASShdOpCodeAlarmDataSize:
		completeMessage = CmdAlarmDataSizeL();
		break;
	case EASShdOpCodeGetAlarmData:
		completeMessage = CmdGetAlarmDataL();
		break;
	//
	// CMD - CATEGORY-SPECIFIC FUNCTIONALITY
	case EASShdOpCodeSetAlarmStatusByCategory:
		completeMessage = CmdSetAlarmStatusByCategoryL();
		break;
	case EASShdOpCodeGetAlarmCountForCategory:
		completeMessage = CmdGetAlarmCountForCategoryL();
		break;
	case EASShdOpCodeAlarmDeleteAllByCategory:
		completeMessage = CmdAlarmDeleteAllByCategoryL();
		break;
	case EASShdOpCodeAlarmDeleteByCategory:
		completeMessage = CmdAlarmDeleteByCategoryL();
		break;
	case EASShdOpCodeAlarmDeleteByCalendarFile:
        completeMessage = CmdAlarmDeleteByCalendarFileL();
        break;
	case EASShdOpCodeGetAvailableCategoryList:
		completeMessage = CmdGetAvailableCategoryListL();
		break;
	case EASShdOpCodeGetAlarmIdListForCategory:
		completeMessage = CmdGetAlarmIdListForCategoryL();
		break;
	//
	// CMD - MISC FUNCTIONALITY
	case EASShdOpCodeAlarmCountByState:
		completeMessage = CmdAlarmCountByStateL();
		break;
	case EASShdOpCodeGetAlarmIdListByState:
		completeMessage = CmdGetAlarmIdListByStateL();
		break;
	case EASShdOpCodeGetAlarmIdList:
		completeMessage = CmdGetAlarmIdListL();
		break;
	case EASShdOpCodeGetNextDueAlarmId:
		completeMessage = CmdGetNextDueAlarmIdL();
		break;
	case EASShdOpCodeNumberOfAlarmsActiveInQueue:
		completeMessage = CmdNumberOfActiveAlarmsInQueueL();
		break;
	//
	// CMD - SOUND CONTROL
	case EASShdOpCodeSetAlarmSoundState:
		completeMessage = CmdSetAlarmSoundStateL();
		break;
	case EASShdOpCodeGetAlarmSoundState:
		completeMessage = CmdGetAlarmSoundStateL();
		break;
	case EASShdOpCodeSetAlarmSoundsSilentUntil:
		completeMessage = CmdSetAlarmSoundsSilentUntilL();
		break;
	case EASShdOpCodeSetAlarmSoundsSilentFor:
		completeMessage = CmdSetAlarmSoundsSilentForL();
		break;
	case EASShdOpCodeGetAlarmSoundsSilentUntil:
		completeMessage = CmdGetAlarmSoundsSilentUntilL();
		break;
	case EASShdOpCodeCancelAlarmSilence:
		completeMessage = CmdCancelAlarmSilenceL();
		break;
	case EASShdOpCodeAlarmSoundsTemporarilySilenced:
		completeMessage = CmdGetAlarmSoundsTemporarilySilencedL();
		break;
	case EASShdOpCodeSetAlarmPlayIntervals:
		completeMessage = CmdSetAlarmPlayIntervalsL();
		break;
	case EASShdOpCodeGetAlarmPlayIntervals:
		completeMessage = CmdGetAlarmPlayIntervalsL();
		break;
	//
	// CMD - CHANGE NOTIFICATION
	case EASShdOpCodeNotifyChange:
		completeMessage = CmdNotifyChangeL();
		break;
	case EASShdOpCodeNotifyChangeCancel:
		completeMessage = CmdNotifyChangeCancelL();
		break;
	//
	// CMD - DEBUG ONLY
	case EASShdOpCodeDbgShutDownServer:
		completeMessage = CmdDbgShutDownServerL();
		break;
	case EASShdOpCodeDbgFailAlloc:
		completeMessage = CmdDbgFailAllocL();
		break;
	case EASShdOpCodeDbgPreventUserNotify:
		completeMessage = CmdDbgPreventUserNotifyL();
		break;
	case EASShdOpCodeDbgSnoozeAlarm:
		completeMessage = CmdDbgSnoozeAlarmL();
		break;
	case EASShdOpCodeDbgSetEnvironmentChangesHandling:
		completeMessage = CmdDbgSetEnvChgHandling();
		break;	
	//
	// CMD - MISC
	case EASShdOpCodeFetchTransferBuffer:
		completeMessage = CmdFetchTransferBufferL();
		break;
	//
	// CMD - FROM MASShdAlarmInfoProvider
	case EASShdOpCodeAlarmCount:
		completeMessage = CmdInfoAlarmCountL();
		break;
	case EASShdOpCodeAlarmAtIndex:
		completeMessage = CmdInfoAlarmByIndexL();
		break;
	case EASShdOpCodeFlushServer:
		completeMessage = ETrue;
		break;
 

	default:
		ASSrvStaticUtils::PanicClient(Message(), EAlarmServerInitiatedClientPanicInvalidOperation);
		break;
		}
		
	return completeMessage;
	}


//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdAlarmAddL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	// Read the client specified alarm object
	TPckg<TASShdAlarm> package(iAlarmSink);
	Message().ReadL(KSlot0, package);

	if (iAlarmSink.IsFloating())
		{
		ConvertFromLocalToUtc(iAlarmSink);			
		}
	
	//Add the Client SID to the AlarmData, only in the EKA2 platform
	iAlarmSink.SetSid(Message().SecureId());
	// Add the alarm
	iSessionEngine->AlarmAddL(iAlarmSink);

	// Get an up to date representation of the alarm (which 
	// we will write back to the client's address space).
	TASSrvAlarm* newAlarm = ServerData().Queue().QueueAlarmById(iAlarmSink.Id());
	AlarmDataAttachL(*newAlarm);
	// Write back to the client's address space 
	TASSrvAlarm copyAlarm(*newAlarm);
	
	if (copyAlarm.IsFloating())
		{
		ConvertFromUtcToLocal(copyAlarm);
		}
		
	TPckgC<TASShdAlarm> pNewAlarm(copyAlarm);
	
	Message().WriteL(KSlot0, pNewAlarm);
	//
	return ETrue;
	}


//*************************************************************************************
void CASSrvSession::RequestExpiryNotificationL(TASSrvAlarm& aAlarm)
	{
	TRAPD(err, aAlarm.RequestExpiryNotificationL(Message()));
	if	(err != KErrNone)
		{
		// Couldn't setup the notification, so cancel the alarm
		ServerData().Queue().DeQueueAlarm(aAlarm);

		// This will complete the request
		User::Leave(err);
		}
	else	
		{
		//Clear sound pause flag if it is set in the case that there is a snoozed alarm is waiting. 
		//This is to fix defect WAG-5B8H7J "Wrong behaviour if create an alarm before the snoozed alarm goes off."
		ServerData().SoundSettings().ClearSoundPauseFlag();

		TASSrvAlarm copyAlarm(aAlarm);
		
		if(copyAlarm.IsFloating())
			{
			ConvertFromUtcToLocal(copyAlarm);
			}
		
		TPckgC<TASShdAlarm> pNewAlarm(copyAlarm);		
		// Write back to the client's address space
		Message().WriteL(KSlot0, pNewAlarm);

		// Set the session id
		aAlarm.SetOriginatingSessionId(iSessionEngine->MASSrvSessionId());
		}
	}


TBool CASSrvSession::CmdAlarmAddWithNotificationL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	// Read and add the alarm, and updates iAlarmSink with the newly-allocated
	// Alarm Id.
	TAlarmId allocatedId = ServerData().Queue().NextFreeAlarmId();
	TPckg<TASShdAlarm> package(iAlarmSink);
	Message().ReadL(KSlot0, package);
	
	if (iAlarmSink.IsFloating())
		{
		ConvertFromLocalToUtc(iAlarmSink);			
		}
	
	//Add the Client SID to the AlarmData, only in the EKA2 platform
	iAlarmSink.SetSid(Message().SecureId());
	//Add the Client SID to the AlarmData
	iSessionEngine->AlarmAddL(iAlarmSink, allocatedId);
	// Get a handle on the alarm
	TASSrvAlarm* newAlarm = ServerData().Queue().QueueAlarmById(iAlarmSink.Id());
	AlarmDataAttachL(*newAlarm);	
	RequestExpiryNotificationL(*newAlarm);
	// Don't complete the message - it's asynchronously completed when the alarm
	// expires, or is cancelled.
	return EFalse;
	}

void CASSrvSession::AlarmDataAttachL(TASSrvAlarm& aAlarm)
	{
	const TInt dataSizeInBytes = static_cast<TInt>(Message().Int1());
	if(dataSizeInBytes>0)
		{
		HBufC8* data = HBufC8::NewLC(dataSizeInBytes);
		TPtr8 pData(data->Des());
		Message().ReadL(KSlot2, pData);
		// Give ownership of the data to the alarm. This will leave with 
		// KErrInUse should the specified alarm already have attached data.
		CleanupStack::Pop(data);
		aAlarm.DataAttachL(data);
		}
	}

//*************************************************************************************
TBool CASSrvSession::CmdAlarmNotificationCancelAndDeQueueL()
	{
	// want write access to alarm data
	TRAPD(err, ServerData().Queue().CheckAlarmQueueWritableL());
	if (err != KErrNone && err != KErrLocked)
		{
		User::Leave(err);	
		}
 

	// Get the alarm id from the client
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	// Get the alarm from the queue
	TASSrvAlarm* alarm = ServerData().Queue().QueueAlarmById(alarmId);
	if (alarm)
		{
		if(err == KErrNone)
			{
			alarm->RequestExpiryNotificationComplete(KErrCancel);
			// Dequeue the alarm
			alarm->DeQueue();	
			}
		else
			{
			alarm->RequestExpiryNotificationComplete(KErrLocked);	
			}
		}
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmDetailsL()
	{
	// Get the alarm id from the client
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());

	// Populate the alarm sink with the details
	iSessionEngine->AlarmDetailsL(alarmId, iAlarmSink);

	if (iAlarmSink.IsFloating())
		{
		ConvertFromUtcToLocal(iAlarmSink);			
		}

	// Write back the alarm to the client's address space
	
	TPckgC<TASShdAlarm> package(iAlarmSink);	
	
	Message().WriteL(KSlot1, package);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdAlarmDeleteL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	iSessionEngine->AlarmDeleteL(alarmId);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmCategoryL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	TPckgBuf<TAlarmCategory> category(iSessionEngine->AlarmCategoryL(alarmId));
	Message().WriteL(KSlot1, category);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmOwnerL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	//
	TFullName owner;
	const TInt error = ServerData().SessionCollection().MASSessionCollectionAlarmOwner(alarmId, owner);
	User::LeaveIfError(error);
	//
	Message().WriteL(KSlot1, owner);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmStatusL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	const TAlarmStatus status = static_cast<TAlarmStatus>(Message().Int1());
	iSessionEngine->SetAlarmStatusL(alarmId, status);
	//
	return ETrue;
	}

//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmStatusForCalendarFileL()
    {
    // want write access to alarm data
    ServerData().Queue().CheckAlarmQueueWritableL();
    
    HBufC* filename(HBufC::NewLC(Message().GetDesLengthL(KSlot0)));
    TPtr fileNamePtr(filename->Des());
    Message().ReadL(KSlot0, fileNamePtr);
    
    const TAlarmStatus status = static_cast<TAlarmStatus>(Message().Int1());
    iSessionEngine->SetAlarmStatusForCalendarFileL(*filename, status);
        
    CleanupStack::PopAndDestroy(filename);
    
    return ETrue;
    }

//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmStatusL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	TPckgBuf<TAlarmStatus> status(iSessionEngine->AlarmStatusL(alarmId));
	Message().WriteL(KSlot1, status);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmDayOrTimedL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	const TAlarmDayOrTimed dayOrTimed = static_cast<TAlarmDayOrTimed>(Message().Int1());
	iSessionEngine->SetAlarmDayOrTimedL(alarmId, dayOrTimed);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmDayOrTimedL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	TPckgBuf<TAlarmDayOrTimed> dayOrTimed(iSessionEngine->AlarmDayOrTimedL(alarmId));
	Message().WriteL(KSlot1, dayOrTimed);
	//
	return ETrue;
	}

//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmCharacteristicsL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	TPckgBuf<TAlarmCharacteristicsFlags> flags(iSessionEngine->AlarmCharacteristicsL(alarmId));
	Message().WriteL(KSlot1, flags);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmCharacteristicsL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	//
	TAlarmCharacteristicsFlags flags;
	TPckg<TAlarmCharacteristicsFlags> pFlags(flags);
	Message().ReadL(KSlot1, pFlags);
	//
	iSessionEngine->SetAlarmCharacteristicsL(alarmId, flags);
	//
	return ETrue;
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
TBool CASSrvSession::CmdSetWakeupL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	const TBool enable = static_cast<TBool>(Message().Int1());
	iSessionEngine->SetWakeupL(alarmId, enable);
	//
	return ETrue;
	}
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
TBool CASSrvSession::CmdGetAlarmDaysL()
	{
	const TAlarmId KAlarmId = static_cast<TAlarmId>(Message().Int0());
	TPckgBuf<TUint8> alarmDays = iSessionEngine->AlarmDaysL(KAlarmId);
	Message().WriteL(KSlot1, alarmDays);

	return ETrue;
	}

TBool CASSrvSession::CmdSetAlarmDaysL()
	{
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId KAlarmId = static_cast<TAlarmId>(Message().Int0());
	const TUint8 KAlarmDays = static_cast<TUint8>(Message().Int1());
	iSessionEngine->SetAlarmDaysL(KAlarmId, KAlarmDays);

	return ETrue;
	}

TBool CASSrvSession::CmdSetContinuousL()
	{
	ServerData().Queue().CheckAlarmQueueWritableL();
	
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	const TBool enable = static_cast<TBool>(Message().Int1());
	iSessionEngine->SetContinuousL(alarmId, enable);
	
	return ETrue;
	}

TBool CASSrvSession::CmdContinuousL()
	{
	const TAlarmId KAlarmId = static_cast<TAlarmId>(Message().Int0());
	TPckgBuf<TBool> continuous = iSessionEngine->ContinuousL(KAlarmId);
	Message().WriteL(KSlot1, continuous);
	
	return ETrue;
	}
#endif

TBool CASSrvSession::CmdOrphanAlarmL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	iSessionEngine->SetAlarmOrphanedL(alarmId);
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetClientDataL()
	{
	TPckg<TASShdAlarm> package(iAlarmSink);
	Message().ReadL(KSlot1, package);

  	TASSrvAlarm& alarm = ServerData().Queue().QueueAlarmByIdL(iAlarmSink.Id());
  	alarm.ClientData1() = iAlarmSink.ClientData1();
  	alarm.ClientData2() = iAlarmSink.ClientData2();
  	TBitFlags16 tempFlags = alarm.ClientFlags();
  	tempFlags.SetValue(iAlarmSink.ClientFlags().Value());
  	alarm.ClientFlags() = tempFlags;
	
	return ETrue;
	}


//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdAlarmDataAttachL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	const TInt dataSizeInBytes = static_cast<TInt>(Message().Int1());
	//
	HBufC8* data = HBufC8::NewLC(dataSizeInBytes);
	TPtr8 pData(data->Des());
	Message().ReadL(KSlot2, pData);
	
	// Give ownership of the data to the alarm. This will leave with 
	// KErrInUse should the specified alarm already have attached data.
	// Or KErrLocked if Alarm Server Backup or Restore are in progress.
	CleanupStack::Pop(data);
	iSessionEngine->AlarmDataAttachL(alarmId, data);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdAlarmDataDetachL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	//
	// Remove the data from the pool - if the specified alarm doesn't exist
	// within the data pool, then this will leave with KErrNotFound.
	// Or KErrLocked if Alarm Server Backup or Restore are in progress.
	iSessionEngine->AlarmDataDetachL(alarmId);	
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdAlarmDataSizeL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	//
	TASSrvAlarm& alarm = ServerData().Queue().QueueAlarmByIdL(alarmId);
	Message().Complete(alarm.DataSizeL());
	
	// Already completed above
	return EFalse;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmDataL()
	{
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	//
	TASSrvAlarm& alarm = ServerData().Queue().QueueAlarmByIdL(alarmId);
	const TPtrC8 pData(alarm.DataL());
	//
	const TInt maxLength = static_cast<TInt>(Message().Int1());
	if	(pData.Size() > maxLength)
		{
		ASSrvStaticUtils::PanicClient(Message(), EAlarmServerInitiatedClientPanicInsufficientRoomForAlarmData);
		return EFalse;
		}
	
	Message().WriteL(KSlot2, pData);
	//
	return ETrue;
	}

//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmStatusByCategoryL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmCategory category = { static_cast<TInt>(Message().Int0()) };
	const TAlarmStatus status = static_cast<TAlarmStatus>(Message().Int1());
	//
	iSessionEngine->SetAlarmStatusByCategoryL(category, status);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmCountForCategoryL()
	{
	const TAlarmCategory category = { static_cast<TInt>(Message().Int0()) };
	const TInt countForCategory = iSessionEngine->AlarmCountByCategory(category);
	//
	TPckg<TInt> pCount(countForCategory);
	Message().WriteL(KSlot1, pCount);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdAlarmDeleteAllByCategoryL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmCategory category = { static_cast<TInt>(Message().Int0()) };
	const TBool restrictToOrphanedAlarms = static_cast<TBool>(Message().Int1());
	iSessionEngine->DeleteAllAlarmsByCategoryL(category, restrictToOrphanedAlarms);
	//
	return ETrue;
	}

//*************************************************************************************
TBool CASSrvSession::CmdAlarmDeleteByCalendarFileL()
    {
    // want write access to alarm data
    ServerData().Queue().CheckAlarmQueueWritableL();
    
    HBufC* filename(HBufC::NewLC(Message().GetDesLengthL(KSlot0)));
    TPtr fileNamePtr(filename->Des());
    Message().ReadL(KSlot0, fileNamePtr);

    const TDeleteType whatToDelete = static_cast<TInt>(Message().Int1());
    iSessionEngine->DeleteAllAlarmsByCalendarFileL(*filename, whatToDelete);
    
    CleanupStack::PopAndDestroy(filename);

    return ETrue;
    }

//*************************************************************************************
TBool CASSrvSession::CmdAlarmDeleteByCategoryL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	const TAlarmCategory category = { static_cast<TInt>(Message().Int0()) };
	const TDeleteType whatToDelete = static_cast<TInt>(Message().Int1());
	iSessionEngine->DeleteAllAlarmsByCategoryL(category, EFalse, whatToDelete);
	//
	return ETrue;
	}

//*************************************************************************************
TBool CASSrvSession::CmdGetAvailableCategoryListL()
	{
	// Get a list of available categories
	RArray<TAlarmCategory>* list = iSessionEngine->AlarmCategoryListLC();

	// Stream them out to a buffer
	const TInt count = list->Count();
	TransferBuffer().Reset();
	RBufWriteStream stream(TransferBuffer());
	CleanupClosePushL(stream); // don't think this is needed but adding it just to be sure
	
	// Externalize the count, since this is needed client-side
	stream.WriteInt32L(count);
	
	// Now externalize the alarm categories, one by one.
	for(TInt i=0; i<count; i++)
		{
		stream << (*list)[i];
		}
	stream.CommitL();
	CleanupStack::Pop(&stream);

	// Finished with the list now
	CleanupStack::PopAndDestroy(list);

	// Write the size of the list back to the client
	TPckgBuf<TInt> size(TransferBuffer().Size());
	Message().WriteL(KSlot0, size);

	// Client will now fetch the contents of the transfer buffer in
	// another request.
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmIdListForCategoryL()
	{
	// Get a list of available categories
	const TAlarmCategory category = { static_cast<TInt>(Message().Int0()) };
	RArray<TAlarmId>* list = iSessionEngine->AlarmIdListByCategoryLC(category);

	// Stream to transfer buffer
	StreamAlarmIdsToTransferBufferL(*list);
	CleanupStack::PopAndDestroy(list);

	// Client will now fetch the contents of the transfer buffer in
	// another request.
	// coverity [leaked_storage]
	return ETrue;
	}


//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdAlarmCountByStateL()
	{
	const TAlarmState state = static_cast<TAlarmState>(Message().Int0());
	const TInt count = iSessionEngine->AlarmCountByState(state);
	//
	TPckg<TInt> pCount(count);
	Message().WriteL(KSlot1, pCount);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmIdListByStateL()
	{
	// Get a list of available categories
	const TAlarmState state = static_cast<TAlarmState>(Message().Int0());
	RArray<TAlarmId>* list = iSessionEngine->AlarmIdListByStateLC(state);

	// Stream to transfer buffer
	StreamAlarmIdsToTransferBufferL(*list);
	CleanupStack::PopAndDestroy(list);

	// Client will now fetch the contents of the transfer buffer in
	// another request.
	// coverity [leaked_storage]
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmIdListL()
	{
	// Get a list of available categories
	RArray<TAlarmId>* list = iSessionEngine->AlarmIdListLC();

	// Stream to transfer buffer
	StreamAlarmIdsToTransferBufferL(*list);
	CleanupStack::PopAndDestroy(list);

	// Client will now fetch the contents of the transfer buffer in
	// another request.
	// coverity [leaked_storage]
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetNextDueAlarmIdL()
	{
	// Fetch the head item from the queue. If there isn't one, then we return KNullAlarmID
	const TAlarmId id = ServerData().Timer().NextDueAlarmId();
	TPckgC<TAlarmId> nextDueAlarm(id);
	Message().WriteL(KSlot0, nextDueAlarm);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdNumberOfActiveAlarmsInQueueL()
	{
	const TInt count = iSessionEngine->NumberOfActiveAlarmsInQueue();
	TPckgC<TInt> pCount(count);
	Message().WriteL(KSlot0, pCount);
	//
	return ETrue;
	}


//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmSoundStateL()
	{
	const TAlarmGlobalSoundState state = static_cast<TAlarmGlobalSoundState>(Message().Int0());
	ServerData().SoundSettings().SetGlobalSoundState(state);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmSoundStateL()
	{
	TPckgBuf<TAlarmGlobalSoundState> soundState(ServerData().SoundSettings().GlobalSoundState());
	Message().WriteL(KSlot0, soundState);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmSoundsSilentUntilL()
	{
	TTime time;
	TPckg<TTime> pTime(time);
	Message().ReadL(KSlot0, pTime);
	//
	
	time -= User::UTCOffset(); // Value is in local time format
	TDateTime t = time.DateTime();
	ASSrvStaticUtils::RoundTimeDownToTheMinute(time);
	t = time.DateTime();
	
	// Can't defer time until a point in the past
	TTime now(ASSrvStaticUtils::UtcTimeNow());
	if	(time < now)
		{
		User::Leave(KErrArgument);
		}
	else
		{
		ServerData().SoundController().SetFloating();
		ServerData().SoundController().MakeAllSoundsQuietUntil(time);
	//
		}
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmSoundsSilentForL()
	{
	TTimeIntervalMinutes timeIntervalMinutes;
	TPckg<TTimeIntervalMinutes> pTimeIntervalMinutes(timeIntervalMinutes);
	Message().ReadL(KSlot0, pTimeIntervalMinutes);

	// Can't defer time until a point in the past
	TTime now(ASSrvStaticUtils::UtcTimeNow());	
	TTime time(now);
		
	time += timeIntervalMinutes;
	
	if	(time < now)
		{
		User::Leave(KErrArgument);
		}
	else
		{
		ServerData().SoundController().SetFixed();
		ServerData().SoundController().MakeAllSoundsQuietUntil(time);
		}
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmSoundsSilentUntilL()
	{
	if	(ServerData().SoundController().InQuietPeriod())
		{
		// Return as local time
		TPckgBuf<TTime> time(ServerData().SoundController().SoundsQuietUntil() + User::UTCOffset());
		Message().WriteL(KSlot0, time);
		}
	else
		{
		// Not in a silent period
		User::Leave(KErrGeneral);
		}
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdCancelAlarmSilenceL()
	{
	CASSrvSoundController& soundController = ServerData().SoundController();
	soundController.CancelSilence();
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmSoundsTemporarilySilencedL()
	{
	CASSrvSoundController& soundController = ServerData().SoundController();
	const TBool inQuietPeriod = soundController.InQuietPeriod();
	TPckgC<TBool> package(inQuietPeriod);
	Message().WriteL(KSlot0, package);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdSetAlarmPlayIntervalsL()
	{
	// want write access to alarm data
	ServerData().Queue().CheckAlarmQueueWritableL();

	// Make the transfer buffer big enough to hold the serialized 
	// play intervals. Int1 holds the size in bytes of the buffer required.
	TransferBuffer().Reset();
	//
	const TInt transferBufferSizeRequired = Message().Int1();
	TransferBuffer().ResizeL(transferBufferSizeRequired);

	// Now read the buffer
	TPtr8 pBuffer(TransferBuffer().Ptr(0));
	Message().ReadL(KSlot0, pBuffer);

	// Create stream view on buffer
	RBufReadStream stream(TransferBuffer());
	CleanupClosePushL(stream);

	// Now internalize the play interval
	RArray<TASSrvAlarmSoundDetails>* newIntervals = ASSrvStaticUtils::InternalizeSoundPlayIntervalsLC(stream);
	
	// Check the sound play intervals are valid
	const TInt error = ASSrvStaticUtils::ValidateSoundPlayIntervals(*newIntervals);
	User::LeaveIfError(error);

	// They're okay, so update the sound settings
	const TInt setError = ServerData().SoundSettings().SetSoundIntervals(newIntervals);
	User::LeaveIfError(setError);

	CleanupStack::Pop(newIntervals);
	CleanupStack::PopAndDestroy(&stream);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdGetAlarmPlayIntervalsL()
	{
	// Externalize all the sound play intervals to the transfer buffer.
	TransferBuffer().Reset();
	RBufWriteStream stream(TransferBuffer());
	CleanupClosePushL(stream);
	ServerData().SoundSettings().ExternalizeSoundIntervalsL(stream);
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);

	// Write the size of the list back to the client
	TPckgBuf<TInt> size(TransferBuffer().Size());
	Message().WriteL(KSlot0, size);
	//
	return ETrue;
	}

//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdNotifyChangeL()
	{
	if	(iFlags.IsSet(EAlarmSessionFlagsNotifyPending))
		{
		ASSrvStaticUtils::PanicClient(Message(), EAlarmServerInitiatedClientPanicChangeNotificationAlreadyOutstanding);
		return EFalse;
		}

	// If we have something in the buffer, notify the client straight away
	if	(iChangeEventBuffer.Count())
		{
		// Oldest event is at the head of the queue
		const TASSrvBufferredEvent& event = iChangeEventBuffer[0];

		// BC: Modification to maintain BC with older version of alarm server
		// the new alarm server must not write to the client because there is nothing in the 
		// client address space to write to
		if(Message().Int1() == EFalse)
			{
			TPckgC<TAlarmId> package(event.AlarmId());
			Message().WriteL(KSlot0, package);
			}
		Message().Complete(event.Code());
		iChangeEventBuffer.Remove(0);
		}
	else
		{
		// Wait for something
		iFlags.Set(EAlarmSessionFlagsNotifyPending);
		iAlarmQueueChangeNotificationMessage = Message();
		}
	//
	return EFalse;
	}


//*************************************************************************************
TBool CASSrvSession::CmdNotifyChangeCancelL()
	{
	// Can't panic the client if there isn't a pending request,
	// because its possible for the client thread to request
	// cancellation (when it still has an outstanding request)
	// but the alarm server thread runs because an event occurs,
	// hence the scheduler doesn't return to idle (to service
	// the cancellation request) until after an event has just
	// been completed. The TT_AMOD test code from TimeW exhibits
	// this problem.
	if	(iFlags.IsSet(EAlarmSessionFlagsNotifyPending))
		CompleteChangeNotificationMessage(KErrCancel, KNullAlarmId);
	return ETrue;
	}

//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdDbgShutDownServerL()
	{
#ifdef _DEBUG
	CActiveScheduler::Stop();
#endif
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdDbgFailAllocL()
	{
#ifdef _DEBUG
	const TInt count = Message().Int0();
	if	(count)
		{
		__UHEAP_FAILNEXT(count);
		}
	else
		__UHEAP_RESET;
#endif
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdDbgPreventUserNotifyL()
	{
#ifdef _DEBUG
	const TBool preventUserNotifications = static_cast<TBool>(Message().Int0());
	ServerData().NotificationCoordinator().__DbgPreventUserNotify(preventUserNotifications);
#endif
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdDbgSnoozeAlarmL()
	{
#ifdef _DEBUG
	const TAlarmId alarmId = static_cast<TAlarmId>(Message().Int0());
	//
	TTime timeWhenAlarmShouldAwaken;
	TPckg<TTime> package(timeWhenAlarmShouldAwaken);
	Message().ReadL(KSlot1, package);
	//

	TASSrvAlarm& alarm = ServerData().Queue().QueueAlarmByIdL(alarmId);
	alarm.Snooze(timeWhenAlarmShouldAwaken);
#endif
	return ETrue;
	}
//*************************************************************************************

TBool CASSrvSession::CmdDbgSetEnvChgHandling()
	{
#ifdef _DEBUG
	TInt val = Message().Int0();
	ServerData().EnvironmentChangeManager().SetEnvironmentChangesHandling(val);
#endif
	return ETrue;
	}
//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdInfoAlarmCountL()
	{
	TPckg<TInt> pCount(ServerData().Queue().QueueAlarmCount());
	Message().WriteL(KSlot0, pCount);
	//
	return ETrue;
	}


//*************************************************************************************
TBool CASSrvSession::CmdInfoAlarmByIndexL()
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();

	// Get the alarm index from the client
	const TInt count = queue.QueueAlarmCount();
	const TInt index = Message().Int0();
	if	(index < count)
		{
		// Populate the alarm sink with the details
		const TASShdAlarm& alarm = queue.QueueAlarmAt(index);

		// Write back the alarm to the client's address space
		TPckgC<TASShdAlarm> package(alarm);
		Message().WriteL(KSlot1, package);
		}
	else
		User::Leave(KErrNotFound);
	//
	return ETrue;
	}


//
//
//


//*************************************************************************************
TBool CASSrvSession::CmdFetchTransferBufferL()
	{
	TPtr8 pBuffer(iTransferBuffer->Ptr(0));
	Message().WriteL(KSlot0, pBuffer);
	//
	return ETrue;
	}


//*************************************************************************************
/**
 * Externalize the specified array to the transfer buffer
 */
void CASSrvSession::StreamAlarmIdsToTransferBufferL(RArray<TAlarmId>& aArray)
	{
	// Stream them out to a buffer
	const TInt count = aArray.Count();
	TransferBuffer().Reset();
	RBufWriteStream stream(TransferBuffer());
	CleanupClosePushL(stream); // don't think this is needed but adding it just to be sure
	
	// Externalize the count, since this is needed client-side
	stream.WriteInt32L(count);
	
	// Now externalize the alarm categories, one by one.
	for(TInt i=0; i<count; i++)
		{
		stream.WriteInt32L(aArray[i]);
		}
	stream.CommitL();
	CleanupStack::Pop(&stream);

	// Write the size of the list back to the client
	TPckgBuf<TInt> size(TransferBuffer().Size());
	Message().WriteL(KSlot1, size);
	}


//*************************************************************************************
/**
 * If there isn't memory to buffer an event then we just overwrite the 
 * head event.
 */
void CASSrvSession::CompleteChangeNotificationMessage(TInt aCompletionCode, TAlarmId aAlarmId)
	{
	// If the client hasn't set up a pending request then we buffer the event.
	// Events are always added to the end of the buffer. The oldest event is
	// at the head.
	if	(!iFlags.IsSet(EAlarmSessionFlagsNotifyPending))
		{
		// Add event to buffer, replacing the oldest if the buffer is full.
		TInt count = iChangeEventBuffer.Count();
		if	(count == KAlarmServerMaxNumberOfChangeEventsToBuffer)
			{
			// Delete the oldest event
			--count;
			iChangeEventBuffer.Remove(0);
			}

		// Try and insert. If there isn't memory to buffer the event then
		// we replace the most recent event with this new one.
		const TInt error = iChangeEventBuffer.Append(TASSrvBufferredEvent(aCompletionCode, aAlarmId));
		if	(error != KErrNone && count)
			{
			TASSrvBufferredEvent& event = iChangeEventBuffer[count - 1];
			event = TASSrvBufferredEvent(aCompletionCode, aAlarmId);
			}
		}
	else
		{
		// Report event straight away
		iFlags.Clear(EAlarmSessionFlagsNotifyPending);

		// BC: Modification to enable BC with old alarm server; 
		// this is so that the server does not write to the client address space;
		// the old API does not include a member variable that the new alarm server
		// assumes in the client and so it is important that the server does not write
		// to this address in the old client.
		if(iAlarmQueueChangeNotificationMessage.Int1() != EFalse)
			{
			iAlarmQueueChangeNotificationMessage.Complete(aCompletionCode);
			}
		else
			{
			// Write to client's address space
			TPckgC<TAlarmId> package(aAlarmId);
			TRAPD(error, iAlarmQueueChangeNotificationMessage.WriteL(0, package));
			if	(error != KErrNone && error == KErrBadDescriptor)
				ASSrvStaticUtils::PanicClient(iAlarmQueueChangeNotificationMessage, EAlarmServerInitiatedClientPanicBadDescriptor);
			else
				iAlarmQueueChangeNotificationMessage.Complete(aCompletionCode);
			}
		}
	}

/**
Converts a UTC alarm to local alarm
*/
void CASSrvSession::ConvertFromUtcToLocal(TASShdAlarm& aAlarm) const
    {
	// converting from UTC to local.
    TTimeIntervalSeconds cachedoffset = ServerData().CachedUtcOffset();

	if (aAlarm.NextDueTime() != Time::NullTTime())
	    {
	    aAlarm.NextDueTime() += cachedoffset;
	    }
	if (aAlarm.OriginalExpiryTime() != Time::NullTTime())
	    {
	    aAlarm.OriginalExpiryTime() += cachedoffset;
	    } 
	}

/**
Converts a local alarm to UTC alarm
*/
void CASSrvSession::ConvertFromLocalToUtc(TASShdAlarm& aAlarm) const
    {
	// converting times from local to UTC.
    TTimeIntervalSeconds cachedoffset = ServerData().CachedUtcOffset();

	if (aAlarm.NextDueTime() != Time::NullTTime())
	    {
	    aAlarm.NextDueTime() -= cachedoffset;
	    }	
	if (aAlarm.OriginalExpiryTime() != Time::NullTTime())
	    {
	    aAlarm.OriginalExpiryTime() -= cachedoffset;
	    }
	}

