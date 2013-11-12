// Copyright (c) 1999-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ConsoleAlarmAlertSession.h"
#include <e32svr.h>
#include <asaltdefs.h>
#include "test/consoleantestclient.h"
#include "ConsoleAlarmAlertSession.h"
#include "ConsoleAlarmAlertConsole.h"
#include "ConsoleAlarmAlertLEDFlasher.h"
#include "ConsoleAlarmAlertServer.h" //for inter-session communication

const TInt KSlot0 = 0;
const TInt KSlot1 = 1;
const TInt KSlot2 = 2;


// message -> string 

static const TDesC& function_to_string(TInt function);

CConsoleAlarmAlertSession::CConsoleAlarmAlertSession(CConsoleAlarmAlertServer* aServer)
:	iServer(aServer), iInstructionSet(NULL), iInstructionSetFound(EFalse)
	{
	}

CConsoleAlarmAlertSession::~CConsoleAlarmAlertSession()
	{
	CancelNotifications();
	__FLOG_CLOSE;
	
	if(iServer != NULL)
	{	
	CConsoleAlarmAlertSession* session = iServer->WaitingSession();
	session = NULL;
	}
	delete iSoundLEDFlasher;
	delete iSoundCallbackTimer;
	delete iConsole;
	}

void CConsoleAlarmAlertSession::ConstructL()
	{
	iSoundLEDFlasher = CConsoleAlarmAlertLEDFlasher::NewL();
	iSoundCallbackTimer = CPeriodic::NewL(CActive::EPriorityIdle);
	iConsole = new(ELeave) CConsoleAlarmAlertConsole(*this);
	__FLOG_CONNECT;
	__FLOG_CREATE(KLogFile, RFileFlogger::ELogModeAppend);
	}

CConsoleAlarmAlertSession* CConsoleAlarmAlertSession::NewL(CConsoleAlarmAlertServer* aServer)
	{
	CConsoleAlarmAlertSession* self = new(ELeave) CConsoleAlarmAlertSession(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

TTime CConsoleAlarmAlertSession::DeferTime() const
	{
	return iDeferTime;
	}

void CConsoleAlarmAlertSession::Notify(TASAltAlertServerResponse aFlag)
	{
	if	(iHasMessage)
		{
		TPckgC<TAlarmId> pId(iAlarm.Id());
		TRAPD(err, iMsgPtr.WriteL(KSlot0, pId));
		
		__ASSERT_ALWAYS(err == KErrNone, User::Invariant());
		
		iMsgPtr.Complete(aFlag);
		iHasMessage = EFalse;
		}
	}

void CConsoleAlarmAlertSession::Notify(TASAltAlertServerResponse aFlag, const TTime& aDeferTime)
	{
	if	(iHasMessage)
		{	
		TPckgC<TTime> time(aDeferTime);
		TRAPD(err, iMsgPtr.WriteL(KSlot1, time));
		__ASSERT_ALWAYS(err == KErrNone, User::Invariant());

		Notify(aFlag);
		}
	}

void CConsoleAlarmAlertSession::ServiceL(const RMessage2& aMessage)
	{
	_LIT(KMsgFmt, "CConsoleAlarmAlertSession::ServiceL: Message: %s (%i) (0x%x 0x%x 0x%x 0x%x)");

	__FLOG_INFO7(KMsgFmt, function_to_string(aMessage.Function()).Ptr(), aMessage.Function(), 
				aMessage.Int0(), aMessage.Int1(), aMessage.Int2(), aMessage.Int3());
	
	if (iServer->IsExtendedMode()) 
		{
		if (MultiDispatchL(aMessage)) 
			{
			aMessage.Complete(KErrNone);
			}
		return;
		}
	
	iMsgComplete = ETrue;
	const TInt function = aMessage.Function();
	
	//see if the server holds a set of instructions to be followed,
	//rather than waiting for user input.
	if(!iInstructionSetFound) //go look for it now.
		{
		//we will enter into this code only once
		const CServer2* tmpServ = Server();
		const CConsoleAlarmAlertServer* specific = reinterpret_cast<const CConsoleAlarmAlertServer*>(tmpServ);
		iInstructionSet = const_cast<CConsoleAlarmAlertServer*>(specific)->ResponseArray();
		iInstructionSetFound = ETrue;
		}

	//
	switch(function)
		{
	case EASAltTestOpCodeSetExtendedMode:
		iServer->SetExtendedMode(ETrue);
		iMsgComplete = ETrue;
		break;
		
	case EASAltOpCodeGetMaxAlarms:
		CmdGetMaxAlarmsL(aMessage);
		iMsgComplete = ETrue;
		break;
		
	case EASAltOpCodeDeleteAlarm:
	case EASAltOpCodeDeleteAlarmAll:
	case EASAltOpCodeVisibleAll:
	case EASAltOpCodeSetStateAll:
		iMsgComplete = ETrue;
		break;
		
		
    case EASAltOpCodeNotify:
    	{
		__ASSERT_ALWAYS(!iHasMessage, User::Invariant());
		iMsgComplete = EFalse;
		iMsgPtr = aMessage;
		iHasMessage = ETrue;
		iServer->SetWaitingSession(this);
		iServer->SetNotifyMessage(&iMsgPtr);
		break;
		}

    case EASAltOpCodeNotifyCancel:
		if	(iHasMessage)
			{
			iHasMessage=EFalse;
			iMsgPtr.Complete(KErrCancel);
			iServer->SetNotifyMessage(NULL);
			}
		break;

	case EASAltOpCodeVisible:
		{
		TBool isVisible = aMessage.Int0();
		iConsole->SetVisibilityL(isVisible);
		iServer->SetNotifying(isVisible);
		break;
		}

    case EASAltOpCodeSetState:
		iConsole->SetAlertStateL(aMessage.Int0());
		break;

    case EASAltOpCodeSetAlarm:
		SetAlarmL(aMessage);
		iConsole->iTimeInterval=0;
		iConsole->UpdateDisplayL();
		// Forces the Console to get response for the next alarm.
		iConsole->CancelKey();
		break;

	case EASAltOpCodeGetUserTime:
	case EASAltOpCodeGetEndQuietTime:
		GetUserTimeL(aMessage);
		break;

	case EASAltOpCodeSetDeferTime:
		SetDeferTimeL(aMessage);
		iConsole->UpdateDisplayL();
		break;

    case EASAltOpCodeLogon:
		__ASSERT_ALWAYS(!iLoggedOn, User::Invariant());
		iMsgComplete=EFalse;
		iLoggedOn=ETrue;
		break;

	case EASAltOpCodeStartPlayingSound:
		{
		iSoundLEDFlasher->Start();
		TCallBack callback(SoundPlaybackCallbackL, this);
		iSoundCallbackTimer->Start(TTimeIntervalMicroSeconds32(0), TTimeIntervalMicroSeconds32(3000000), callback);
		break;
		}

	case EASAltOpCodeStopPlayingSoundAll: // Fallthrough
	case EASAltOpCodeStopPlayingSound:
		iSoundLEDFlasher->Stop();
		iSoundCallbackTimer->Cancel();
		break;

//Test code
	case EASAltTestOpCodeAcknowledgeAlarm:
		if (iServer->WaitingSession())
			iServer->WaitingSession()->Notify(EASAltAlertServerResponseClear);
		break;

	case EASAltTestOpCodeGetAttachment:
		{
		if (iServer->Attachment() != NULL)
			{
			const TDes8& data = iServer->Attachment()->Des();
			const TInt maxLength = static_cast<TInt>(aMessage.Int0());
			if	(data.Size() > maxLength)
				User::Panic(_L("Client's buffer is too short"), KErrOverflow);
			else
				{
				if (data.Length() == 0)
					{
					_LIT8(KEmpty,"Empty");
					aMessage.WriteL(KSlot1, KEmpty);
					}
				else
					{
					aMessage.WriteL(KSlot1, data);
					}
				iServer->SetAttachment(NULL);
				}
			}
		}
		break;

	case EASAltTestOpCodeGetIsNotifying:
		{
		TPckgC<TBool> ret = iServer->IsNotifying();
		aMessage.WriteL(KSlot0, ret);
		break;
		}

	default:
		User::Invariant();
		break;
		}

	if	(iMsgComplete)
		aMessage.Complete(KErrNone);
	}

TBool CConsoleAlarmAlertSession::CmdL()
	{
	return KErrNone;
	}

void CConsoleAlarmAlertSession::ServiceError(const RMessage2 &aMessage, TInt aError)
	{
	_LIT(KErrFmt, "CConsoleAlarmAlertSession::ServiceError: Message: %s (0x%x, 0x%x, 0x%x, 0x%x) Error: %i");
	__FLOG_INFO7(KErrFmt, function_to_string(aMessage.Function()).Ptr(), 
				aMessage.Int0(), aMessage.Int1(), aMessage.Int2(), aMessage.Int3(),
				aError); 
	}


//
//
//


//*************************************************************************************
void CConsoleAlarmAlertSession::SetAlarmL(const RMessage2& aMessage)
	{
	TPckg<TASShdAlarm> pA(iAlarm);  
	aMessage.ReadL(KSlot0, pA);
	
	   	//These lines will make sure the alert server is given the alarm in local time, if the alarm is set in local time (a floating alarm)
  	if (iAlarm.IsFloating())  				
  		{						
  			TTimeIntervalSeconds offset = User::UTCOffset();
  			if (iAlarm.NextDueTime() != Time::NullTTime())
  			    {
  			    iAlarm.NextDueTime() += offset;
  			    }
  			if (iAlarm.OriginalExpiryTime() != Time::NullTTime())
  			    {
  			    iAlarm.OriginalExpiryTime() += offset;
  			    } 		
  		}	
	
	
	if	(iAlarm.HasAssociatedData())
		{
		//Storing the data in the server for the test session to read..
		iAlarmAssociatedDataSize = aMessage.GetDesLengthL(2);
		HBufC8* data = HBufC8::NewLC(iAlarmAssociatedDataSize);
		TPtr8 pData(data->Des());
		aMessage.ReadL(KSlot2, pData);
		iServer->SetAttachment(data);  //Server takes an ownership
		CleanupStack::Pop(data);
		}
	else
		iAlarmAssociatedDataSize = 0;
	}


//*************************************************************************************
void CConsoleAlarmAlertSession::GetUserTimeL(const RMessage2& aMessage)
	{
	TTime time;
	time.UniversalTime();
	time+=TTimeIntervalMinutes(iConsole->GetTimeInterval());
	TPtrC8 pTime((TUint8 *)&time,sizeof(TTime));
	aMessage.WriteL(KSlot0,pTime);
	}


//*************************************************************************************
void CConsoleAlarmAlertSession::SetDeferTimeL(const RMessage2& aMessage)
	{
	TPckg<TTime> pTime(iDeferTime);
	aMessage.ReadL(KSlot0, pTime);
	}


//*************************************************************************************
TBool CConsoleAlarmAlertSession::SoundPlaybackCallbackL(TAny* /*aSelf*/)
	{
//	_LIT(KPlayingSound, "Playing sound");
//	User::InfoPrint(KPlayingSound);
	return ETrue;
	}

//**************************************************************************************
TAny * CConsoleAlarmAlertSession::InstructionSet()
	{
	return iInstructionSet;
	}
	
/**
Completes the asynchronous EASAltTestOpCodeNotifyOnAlarm request.

@param aAlarm, alarm received with the EASAltOpCodeSetAlarm message.
@param aFullName, owner of the alarm.
@param aData, any data attached with the EASAltOpCodeSetAlarm message.
*/

void CConsoleAlarmAlertSession::NotifyAlarmL(const TASShdAlarm& aAlarm, const TFullName& aFullName, const TDes8& aData)
	{
	TPckgC<TASShdAlarm> pAlarm(aAlarm);
	
	iAlarmNotify.WriteL(KSlot0, pAlarm);
	
	if (iAlarmNotify.Int1() != 0) 
		{
		iAlarmNotify.WriteL(KSlot1, aFullName);
		}
	
	if (aData.Length() <= iAlarmNotify.GetDesLength(KSlot2))
		{
		iAlarmNotify.WriteL(KSlot2, aData);
		}
		
	iAlarmNotify.Complete(KErrNone);
	}

/**
Completes the asynchronous EASAltTestOpCodeNotifyOnSoundStart request.

@param aId, id of alarm received with the EASAltOpCodeStartPlayingSound message.
@param aFilename, name of sound file to play.
*/

void CConsoleAlarmAlertSession::NotifySoundStartL(const TAlarmId& aId, const TDesC& aFilename)
	{
	TPckgC<TAlarmId> pId(aId);
	
	iSoundStartNotify.WriteL(KSlot0, pId);
	
	if(aFilename.Length() <= iSoundStartNotify.GetDesLength(KSlot1))
		{
		iSoundStartNotify.WriteL(KSlot1, aFilename);
		}
	
	iSoundStartNotify.Complete(KErrNone);
	}

/**
Completes the asynchronous EASAltTestOpCodeNotifyOnSoundStop request.

@param aId, id of alarm received with the EASAltOpCodeStopPlayingSound message.
*/

void CConsoleAlarmAlertSession::NotifySoundStopL(const TAlarmId& aId)
	{
	TPckgC<TAlarmId> pId(aId);
	
	iSoundStopNotify.WriteL(KSlot0, pId);
	iSoundStopNotify.Complete(KErrNone);
	}

/**
Completes the asynchronous EASAltTestOpCodeNotifyOnVisible request.

@param aId, id of alarm received with the EASAltOpCodeVisible message.
@param aVisible, visible status received with the EASAltOpCodeVisible message.
*/

void CConsoleAlarmAlertSession::NotifyVisibleL(const TAlarmId& aId, const TBool aVisible)
	{
	TPckgC<TAlarmId> pId(aId);
	TPckgC<TBool> pVisible(aVisible);
	
	iVisibleNotify.WriteL(KSlot0, pId);
	iVisibleNotify.WriteL(KSlot1, pVisible);
	iVisibleNotify.Complete(KErrNone);
	}

/**
Completes the asynchronous EASAltTestOpCodeNotifyOnState request.

@param aId, id of alarm received with the EASAltOpCodeSetState message.
@param aState, state received with the EASAltOpCodeSetState message.
*/

void CConsoleAlarmAlertSession::NotifyStateL(const TAlarmId& aId, const TInt aState)
	{
	TPckgC<TAlarmId> pId(aId);
	TPckgC<TInt> pState(aState);
	
	iStateNotify.WriteL(KSlot0, pId);
	iStateNotify.WriteL(KSlot1, pState);
	iStateNotify.Complete(KErrNone);
	}

/**
Completes the asynchronous EASAltTestOpCodeNotifyOnDelete request.

@param aId, id of alarm received with the EASAltOpCodeDelete message.
*/
	
void CConsoleAlarmAlertSession::NotifyDeleteL(const TAlarmId& aId)
	{
	TPckgC<TAlarmId> pId(aId);
	
	iDeleteNotify.WriteL(KSlot0, pId);
	iDeleteNotify.Complete(KErrNone);
	}

/**
When in multiple alarm mode this method handles the dispatching of messages.

@param aMessage, the message to dispatch.
@return ETrue if this message should be completed with KErrNone on return.
		EFalse if this message shouldn't be completed on return.
*/

TBool CConsoleAlarmAlertSession::MultiDispatchL(const RMessage2& aMessage)
	{
	TBool complete = ETrue;
	
	switch(aMessage.Function())
		{
	case EASAltOpCodeNotify:
		iMsgPtr = aMessage;
		iServer->SetNotifyMessage(&iMsgPtr);
		complete = EFalse;	
		break;

	case EASAltOpCodeNotifyCancel:
		iServer->NotifyMessage()->Complete(KErrCancel);
		iServer->SetNotifyMessage(NULL);
		complete = EFalse;
		break;

	case EASAltOpCodeVisible:
		CmdVisibleL(aMessage);
		break;

	case EASAltOpCodeSetState:
		CmdSetStateL(aMessage);
		break;
		
	case EASAltOpCodeSetAlarm:
		CmdSetAlarmL(aMessage);
		break;
		
	case EASAltOpCodeSetDeferTime:
		CmdSetDeferTimeL(aMessage);
		break;

	case EASAltOpCodeGetEndQuietTime:
	case EASAltOpCodeGetUserTime:
		CmdGetUserTimeL(aMessage);
		break;

	case EASAltOpCodeLogon:
		complete = EFalse;	
		break;

	case EASAltOpCodeStartPlayingSound:
		CmdStartPlayingSoundL(aMessage);
		break;

	case EASAltOpCodeStopPlayingSound:
		CmdStopPlayingSoundL(aMessage);
		break;
		
	case EASAltOpCodeVisibleAll:
	case EASAltOpCodeSetStateAll:
	case EASAltOpCodeStopPlayingSoundAll:
		break;
		
	case EASAltOpCodeDeleteAlarm:
	case EASAltOpCodeDeleteAlarmAll:
		CmdDeleteAlarmL(aMessage);
		break;
		
	case EASAltOpCodeGetMaxAlarms:
		CmdGetMaxAlarmsL(aMessage);
		break;
	
	case EASAltTestOpCodeUnsetExtendedMode:
		iServer->SetExtendedMode(EFalse);
		break;
		
	case EASAltTestOpCodeSetExtendedMode: // Already in extended mode
		break;
			
	case EASAltTestOpCodeSetUserTime:
		CmdSetUserTimeL(aMessage);
		break;
				
	case EASAltTestOpCodeSetMaxAlarms:
		CmdSetMaxAlarmsL(aMessage);
		break;
	
	case EASAltTestOpCodeNotifyOnAlarm:
		CmdNotifyOnAlarmL(aMessage);
		complete = EFalse;
		break;
		
	case EASAltTestOpCodeNotifyOnSoundStart:
		CmdNotifyOnSoundStartL(aMessage);
		complete = EFalse;
		break;
		
	case EASAltTestOpCodeNotifyOnSoundStop:
		CmdNotifyOnSoundStopL(aMessage);
		complete = EFalse;
		break;
		
	case EASAltTestOpCodeNotifyOnVisible:
		CmdNotifyOnVisibleL(aMessage);
		complete = EFalse;
		break;
		
	case EASAltTestOpCodeNotifyOnState:
		CmdNotifyOnStateL(aMessage);
		complete = EFalse;
		break;
		
	case EASAltTestOpCodeNotifyOnDelete:
		CmdNotifyOnDeleteL(aMessage);
		complete = EFalse;
		break;
		
	case EASAltTestOpCodeResponseSnoozeAlarm:
		CmdResponseSnoozeAlarmL(aMessage);
		break;
		
	case EASAltTestOpCodeResponseSilenceAlarm:
		CmdResponseSilenceAlarmL(aMessage);
		break;
		
	case EASAltTestOpCodeResponseAcknowledgeAlarm:
		CmdResponseAcknowledgeAlarmL(aMessage);
		break;
		
	case EASAltTestOpCodeResponsePauseSound:
		CmdResponsePauseSoundL(aMessage);
		break;
		
	case EASAltTestOpCodeCancelNotifications:
		CancelNotifications();
		break;
	
	case EASAltTestOpCodeResponseAcknowledgeAll:
		CmdResponseAcknowledgeAll();
		break;
		
	default:
		User::Panic(_L("Unsupported op-code sent!"), KErrNotSupported);
		}
		
	return complete;
	}

/**
Sets the user time of the Console Alarm Alert Server.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdSetUserTimeL(const RMessage2& aMessage)
	{
	TTime time;
	TPckg<TTime> pTime(time);
	
	aMessage.ReadL(KSlot0, pTime);
	iServer->SetUserTime(time);
	}

/**
Sets the max alarms value of the Console Alarm Alert Server.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdSetMaxAlarmsL(const RMessage2& aMessage)
	{
	TInt maxAlarms;
	TPckg<TInt> pMaxAlarms(maxAlarms);
	
	aMessage.ReadL(KSlot0, pMaxAlarms);
	
	_LIT(KSetMaxFmt, "SetMaxAlarms: %i");
	__FLOG_INFO2(KSetMaxFmt, maxAlarms);
	
	iServer->SetMaxAlarms(maxAlarms);
	}

/**
Adds the session to the list of alarm observers.

@param aMessage message to be completed when the asynchronous request is to be completed.
*/

void CConsoleAlarmAlertSession::CmdNotifyOnAlarmL(const RMessage2& aMessage)
	{
	if(!iAlarmNotify.IsNull())
		{
		aMessage.Complete(KErrInUse);
		}

	iAlarmNotify = aMessage;
	iServer->AddObserverL(CConsoleAlarmAlertServer::EAlarmObserver, this);
	}

/**
Adds the session to the list of sound start observers.

@param aMessage message to be completed when the asynchronous request is to be completed.
*/

void CConsoleAlarmAlertSession::CmdNotifyOnSoundStartL(const RMessage2& aMessage)
	{
	if(!iSoundStartNotify.IsNull())
		{
		aMessage.Complete(KErrInUse);
		}

	iSoundStartNotify = aMessage;
	iServer->AddObserverL(CConsoleAlarmAlertServer::ESoundStartObserver, this);
	}

/**
Adds the session to the list of sound stop observers.

@param aMessage message to be completed when the asynchronous request is to be completed.
*/
	
void CConsoleAlarmAlertSession::CmdNotifyOnSoundStopL(const RMessage2& aMessage)
	{
	if(!iSoundStopNotify.IsNull())
		{
		aMessage.Complete(KErrInUse);
		}

	iSoundStopNotify = aMessage;
	iServer->AddObserverL(CConsoleAlarmAlertServer::ESoundStopObserver, this);
	}

/**
Adds the session to the list of visible observers.

@param aMessage message to be completed when the asynchronous request is to be completed.
*/
	
void CConsoleAlarmAlertSession::CmdNotifyOnVisibleL(const RMessage2& aMessage)
	{
	if(!iVisibleNotify.IsNull())
		{
		aMessage.Complete(KErrInUse);
		}

	iVisibleNotify = aMessage;
	iServer->AddObserverL(CConsoleAlarmAlertServer::EVisibleObserver, this);
	}
	
/**
Adds the session to the list of state observers.

@param aMessage message to be completed when the asynchronous request is to be completed.
*/

void CConsoleAlarmAlertSession::CmdNotifyOnStateL(const RMessage2& aMessage)
	{
	if(!iStateNotify.IsNull())
		{
		aMessage.Complete(KErrInUse);
		}
	
	iStateNotify = aMessage;
	iServer->AddObserverL(CConsoleAlarmAlertServer::EStateObserver, this);
	}
	
void CConsoleAlarmAlertSession::CmdNotifyOnDeleteL(const RMessage2& aMessage)
	{
	if(!iDeleteNotify.IsNull())
		{
		aMessage.Complete(KErrInUse);
		}
		
	iDeleteNotify = aMessage;
	iServer->AddObserverL(CConsoleAlarmAlertServer::EDeleteObserver, this);
	}
	
/**
Completes the notify message with a EASAltAlertServerResponseSnooze response.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdResponseSnoozeAlarmL(const RMessage2& aMessage)
	{
	if (iServer->NotifyMessage())
		{
		TTime time;
		TPckg<TTime> pTime(time);
		aMessage.ReadL(KSlot1, pTime);

		iServer->NotifyMessage()->WriteL(KSlot0, TPckgC<TAlarmId>(aMessage.Int0()));
		iServer->NotifyMessage()->WriteL(KSlot1, pTime);
		iServer->NotifyMessage()->Complete(EASAltAlertServerResponseSnooze);
		iServer->SetNotifyMessage(NULL);
		}
	}
	
/**
Completes the notify message with a EASAltAlertServerResponseSilence response.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdResponseSilenceAlarmL(const RMessage2& aMessage)
	{
	if (iServer->NotifyMessage())
		{
		iServer->NotifyMessage()->WriteL(KSlot0, TPckgC<TAlarmId>(aMessage.Int0()));
		iServer->NotifyMessage()->Complete(EASAltAlertServerResponseSilence);
		iServer->SetNotifyMessage(NULL);
		}
	}
	
/**
Completes the notify message with a EASAltAlertServerResponseClear response.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdResponseAcknowledgeAlarmL(const RMessage2& aMessage)
	{
	if (iServer->NotifyMessage())
		{
		iServer->NotifyMessage()->WriteL(KSlot0, TPckgC<TAlarmId>(aMessage.Int0()));
		iServer->NotifyMessage()->Complete(EASAltAlertServerResponseClear);
		iServer->SetNotifyMessage(NULL);
		}
	}
	
/**
Completes the notify message with a EASAltAlertServerResponsePauseSound response.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdResponsePauseSoundL(const RMessage2& aMessage)
	{
	if (iServer->NotifyMessage())
		{
		TTime time;
		TPckg<TTime> pTime(time);
		aMessage.ReadL(KSlot1, pTime);

		iServer->NotifyMessage()->WriteL(KSlot0, TPckgC<TAlarmId>(aMessage.Int0()));
		iServer->NotifyMessage()->WriteL(KSlot1, pTime);
		iServer->NotifyMessage()->Complete(EASAltAlertServerResponsePauseSound);
		iServer->SetNotifyMessage(NULL);
		}
	}

/**
Retrieves alarm, owner, any attached data and notifies all alarm observers.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdSetAlarmL(const RMessage2& aMessage)
	{
	TASShdAlarm alarm;
	TPckg<TASShdAlarm> pAlarm(alarm);
	aMessage.ReadL(KSlot0, pAlarm);
	
	TFullName name;
	aMessage.ReadL(KSlot1, name);
	
	HBufC8 *buf = HBufC8::NewLC(aMessage.GetDesLengthL(KSlot2));
	TPtr8 data(buf->Des());
	aMessage.ReadL(KSlot2, data);
	
	TBuf<KMaxAlarmMessageLength + 1> msg(alarm.Message());
	_LIT(KSetAlarmFmt, "Alarm id: %i message: %s");
	__FLOG_INFO3(KSetAlarmFmt, alarm.Id(), msg.PtrZ());
	
	iServer->NotifyAlarmObserversL(alarm, name, data);
	
	CleanupStack::PopAndDestroy(buf);
	}
	
/**
Notifies all state observers.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdSetStateL(const RMessage2& aMessage)
	{
	iServer->NotifyStateObserversL(aMessage.Int1(), aMessage.Int0());
	}
	
/**
Notifies all visible observers.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdVisibleL(const RMessage2& aMessage)
	{
	iServer->NotifyVisibleObserversL(aMessage.Int1(), aMessage.Int0());
	}
	
/**
Retrieves sound file name and notifies all sound start observers.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdStartPlayingSoundL(const RMessage2& aMessage)
	{
	HBufC* buf = HBufC::NewLC(aMessage.GetDesLengthL(KSlot0) + 1);
	TPtr data(buf->Des());
	aMessage.ReadL(KSlot0, data);
	
	_LIT(KPlaySoundFmt, "Sound: %s");
	__FLOG_INFO2(KPlaySoundFmt, data.PtrZ());
	
	iServer->NotifySoundStartObserversL(aMessage.Int1(), data);
	
	CleanupStack::PopAndDestroy(buf);
	}

/**
Notifies all sound stop observers.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdStopPlayingSoundL(const RMessage2& aMessage)
	{
	iServer->NotifySoundStopObserversL(aMessage.Int0());
	}
	
/**
Logs the received defer time.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdSetDeferTimeL(const RMessage2& aMessage)
	{
	TTime time;
	TPckg<TTime> pTime(time);
	aMessage.ReadL(KSlot0, pTime);
	
	TBuf<32> buf;
	time.FormatL(buf, _L("%Y:%M:%D - %H:%T:%S"));
	_LIT(KDeferTimeFmt, "Set Defer Time: %s");
	
	__FLOG_INFO2(KDeferTimeFmt, buf.PtrZ());
	}
	
/**
Sends the Console Alarm Alert Server's user time.

@param aMessage message to dispatch.
*/

void CConsoleAlarmAlertSession::CmdGetUserTimeL(const RMessage2& aMessage)
	{
	TBuf<32> buf;
	iServer->UserTime().FormatL(buf, _L("%Y:%M:%D - %H:%T:%S"));
	_LIT(KGetUserTimeFmt, "Get User Time: %s");
	
	__FLOG_INFO2(KGetUserTimeFmt, buf.PtrZ());
	
	TPckgC<TTime> pTime(iServer->UserTime());
	aMessage.WriteL(KSlot0, pTime);
	}
	
/**
Cancels all outstanding notifications.
*/	
	
void CConsoleAlarmAlertSession::CancelNotifications()
	{
	iServer->RemoveObserver(CConsoleAlarmAlertServer::EAlarmObserver, this);
	iServer->RemoveObserver(CConsoleAlarmAlertServer::ESoundStopObserver, this);
	iServer->RemoveObserver(CConsoleAlarmAlertServer::ESoundStartObserver, this);
	iServer->RemoveObserver(CConsoleAlarmAlertServer::EVisibleObserver, this);
	iServer->RemoveObserver(CConsoleAlarmAlertServer::EStateObserver, this);
	iServer->RemoveObserver(CConsoleAlarmAlertServer::EDeleteObserver, this);

	if(!iAlarmNotify.IsNull())
		{
		iAlarmNotify.Complete(KErrCancel);
		}	
	if(!iSoundStartNotify.IsNull())
		{
		iSoundStartNotify.Complete(KErrCancel);
		}
	if(!iSoundStopNotify.IsNull())
		{
		iSoundStopNotify.Complete(KErrCancel);
		}
	if(!iVisibleNotify.IsNull())
		{
		iVisibleNotify.Complete(KErrCancel);
		}
	if(!iStateNotify.IsNull())
		{
		iStateNotify.Complete(KErrCancel);
		}
	if(!iDeleteNotify.IsNull())
		{
		iDeleteNotify.Complete(KErrCancel);
		}
	}
	
/**
Sends a EASAltAlertServerResponseClearAll to the Alarm Server.
*/	
	
void CConsoleAlarmAlertSession::CmdResponseAcknowledgeAll()
	{
	if (iServer->NotifyMessage())
		{
		iServer->NotifyMessage()->Complete(EASAltAlertServerResponseClearAll);
		iServer->SetNotifyMessage(NULL);
		}
	}
	
void CConsoleAlarmAlertSession::CmdGetMaxAlarmsL(const RMessage2& aMessage)
	{
	TPckg<TInt> pMax(iServer->MaxAlarms());
	aMessage.WriteL(KSlot0, pMax);
	}
	
void CConsoleAlarmAlertSession::CmdDeleteAlarmL(const RMessage2& aMessage)
	{
	if (aMessage.Function() == EASAltOpCodeDeleteAlarmAll)
		{
		iServer->NotifyDeleteObserversL(KNullAlarmId);
		}
	else
		{
		iServer->NotifyDeleteObserversL(aMessage.Int0());
		}
	}

// Op-codes are from TASAltTestOpCode in ConsoleANTestClient.h

_LIT(KOpCode0, "EASAltOpCodeNotify\0");
_LIT(KOpCode1, "EASAltOpCodeVisible\0");
_LIT(KOpCode2, "EASAltOpCodeSetState\0");
_LIT(KOpCode3, "EASAltOpCodeSetAlarm\0");
_LIT(KOpCode4, "EASAltOpCodeSetDeferTime\0");
_LIT(KOpCode5, "EASAltOpCodeGetUserTime\0");
_LIT(KOpCode6, "EASAltOpCodeLogon\0");
_LIT(KOpCode7, "EASAltOpCodeStartPlayingSound\0");
_LIT(KOpCode8, "EASAltOpCodeStopPlayingSound\0");
_LIT(KOpCode9, "EASAltTestOpCodeAcknowledgeAlarm\0");
_LIT(KOpCode10, "EASAltOpCodeVisibleAll\0");
_LIT(KOpCode11, "EASAltOpCodeSetStateAll\0");
_LIT(KOpCode12, "EASAltOpCodeStopPlayingSoundAll\0");	
_LIT(KOpCode13, "EASAltOpCodeDeleteAlarm\0");
_LIT(KOpCode14, "EASAltOpCodeGetEndQuietTime\0");	
_LIT(KOpCode15, "EASAltOpCodeGetMaxAlarms\0");
_LIT(KOpCode16, "EASAltTestOpCodeGetAttachment\0");
_LIT(KOpCode17, "EASAltTestOpCodeGetIsNotifying\0");
_LIT(KOpCode18, "EASAltTestOpCodeSetExtendedMode\0");
_LIT(KOpCode19, "EASAltTestOpCodeUnsetExtendedMode\0");
_LIT(KOpCode20, "EASAltTestOpCodeSetUserTime\0");
_LIT(KOpCode21, "EASAltTestOpCodeSetMaxAlarms\0");
_LIT(KOpCode22, "EASAltTestOpCodeNotifyOnAlarm\0");
_LIT(KOpCode23, "EASAltTestOpCodeNotifyOnSoundStart\0");
_LIT(KOpCode24, "EASAltTestOpCodeNotifyOnSoundStop\0");
_LIT(KOpCode25, "EASAltTestOpCodeNotifyOnVisible\0");
_LIT(KOpCode26, "EASAltTestOpCodeNotifyOnState\0");
_LIT(KOpCode27, "EASAltTestOpCodeResponseSnoozeAlarm\0");
_LIT(KOpCode28, "EASAltTestOpCodeResponseSilenceAlarm\0");
_LIT(KOpCode29, "EASAltTestOpCodeResponseAcknowledgeAlarm\0");
_LIT(KOpCode30, "EASAltTestOpCodeResponsePauseSound\0");
_LIT(KOpCode31, "EASAltTestOpCodeCancelNotifications\0");
_LIT(KOpCode32, "EASAltTestOpCodeResponseAcknowledgeAll\0");
_LIT(KOpCode33, "EASAltTestOpCodeNotifyOnDelete\0");
_LIT(KOpCode34, "EASAltOpCodeNotifyCancel\0");
_LIT(KOpCode35, "EASAltOpCodeDeleteAlarmAll\0");
_LIT(KUnknown, "Unknown op-code\0");

/**
Maps "op-codes" to textual description.
@param function, "op-code" to map.
@return textual description of "op-code".
*/

static const TDesC& function_to_string(TInt function)
	{
	switch(function)
		{
	case EASAltOpCodeNotify: 
		return KOpCode0;
	case EASAltOpCodeVisible: 
		return KOpCode1;
	case EASAltOpCodeSetState: 
		return KOpCode2;
	case EASAltOpCodeSetAlarm: 
		return KOpCode3;
	case EASAltOpCodeSetDeferTime: 
		return KOpCode4;
	case EASAltOpCodeGetUserTime: 
		return KOpCode5;
	case EASAltOpCodeLogon: 
		return KOpCode6;
	case EASAltOpCodeStartPlayingSound: 
		return KOpCode7;
	case EASAltOpCodeStopPlayingSound: 
		return KOpCode8;
	case EASAltTestOpCodeAcknowledgeAlarm: 
		return KOpCode9;
	case EASAltOpCodeVisibleAll:
		return KOpCode10;
	case EASAltOpCodeSetStateAll:
		return KOpCode11;
	case EASAltOpCodeStopPlayingSoundAll:
		return KOpCode12;	
	case EASAltOpCodeDeleteAlarm:
		return KOpCode13;
	case EASAltOpCodeGetEndQuietTime:
		return KOpCode14;
	case EASAltOpCodeGetMaxAlarms:
		return KOpCode15;	
	case EASAltTestOpCodeGetAttachment: 
		return KOpCode16;
	case EASAltTestOpCodeGetIsNotifying: 
		return KOpCode17;
	case EASAltTestOpCodeSetExtendedMode: 
		return KOpCode18;
	case EASAltTestOpCodeUnsetExtendedMode: 
		return KOpCode19;
	case EASAltTestOpCodeSetUserTime: 
		return KOpCode20;
	case EASAltTestOpCodeSetMaxAlarms: 
		return KOpCode21;
	case EASAltTestOpCodeNotifyOnAlarm: 
		return KOpCode22;
	case EASAltTestOpCodeNotifyOnSoundStart: 
		return KOpCode23;
	case EASAltTestOpCodeNotifyOnSoundStop: 
		return KOpCode24;
	case EASAltTestOpCodeNotifyOnVisible: 
		return KOpCode25;
	case EASAltTestOpCodeNotifyOnState: 
		return KOpCode26;
	case EASAltTestOpCodeResponseSnoozeAlarm: 
		return KOpCode27;
	case EASAltTestOpCodeResponseSilenceAlarm: 
		return KOpCode28;
	case EASAltTestOpCodeResponseAcknowledgeAlarm: 
		return KOpCode29;
	case EASAltTestOpCodeResponsePauseSound: 
		return KOpCode30;
	case EASAltTestOpCodeCancelNotifications:
		return KOpCode31;
	case EASAltTestOpCodeResponseAcknowledgeAll:
		return KOpCode32;
	case EASAltTestOpCodeNotifyOnDelete:
		return KOpCode33;
	case EASAltOpCodeNotifyCancel:
		return KOpCode34;
	case EASAltOpCodeDeleteAlarmAll:
		return KOpCode35;
	default:
		return KUnknown;
		}
	}
	
