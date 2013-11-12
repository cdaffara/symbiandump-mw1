/**
* Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __CONSOLEALARMALERTSESSION_H__
#define __CONSOLEALARMALERTSESSION_H__

#include <e32base.h>

#define __FILELOGGER_ACTIVE // Include logging in release builds
#include <test/rfileloggermacro.h>

#include <asaltdefs.h>
#include <asshdalarm.h>

class CConsoleAlarmAlertConsole;
class CConsoleAlarmAlertLEDFlasher;
class CConsoleAlarmAlertServer;

struct TAgnAlarmInfo
 	/**
 	Copy of System-Wide Unique Agenda Entry Reference defined in agmalarm.h
 	*/
  	{
	TUid iAlarmCategory;
	TUint32 iAgnUniqueId_AgnId;
	TInt64  iAgnUniqueId_AgnInstanceTime;
	TUint16 iAgnUniqueId_AgnInstanceFiller;
	TInt    iAgnUniqueId_AgnInstanceConstrError;
	TFileName iFileName;
	};

class CConsoleAlarmAlertSession : public CSession2
	{
public:
	static CConsoleAlarmAlertSession* NewL(CConsoleAlarmAlertServer* aServer);
	~CConsoleAlarmAlertSession();

private:
	CConsoleAlarmAlertSession(CConsoleAlarmAlertServer* aServer);
	void ConstructL();

public:
	TTime									DeferTime() const;
	inline const TASShdAlarm& 				Alarm() const { return iAlarm; }
	inline TInt								AlarmDataSize() const { return iAlarmAssociatedDataSize; }
	inline const TAgnAlarmInfo&				AlarmData() const { return iAlarmData; }
	void									Notify(TASAltAlertServerResponse aFlag);
	void									Notify(TASAltAlertServerResponse aFlag, const TTime& aDeferTime);
	TAny *									InstructionSet();

public: // Observers
	void NotifyAlarmL(const TASShdAlarm& aAlarm, const TFullName& aFullName, const TDes8& aData);
	void NotifySoundStartL(const TAlarmId& aId, const TDesC& aFilename);
	void NotifySoundStopL(const TAlarmId& aId);
	void NotifyVisibleL(const TAlarmId& aId, const TBool aVisible);
	void NotifyStateL(const TAlarmId& aId, const TInt aState);
	void NotifyDeleteL(const TAlarmId& aId);

private: // from CSession2
	TBool									CmdL();
	void									ServiceL(const RMessage2& aMessage);
	void 									ServiceError(const RMessage2 &aMessage, TInt aError);

private:
	void									SetAlarmL(const RMessage2& aMessage);
	void									GetUserTimeL(const RMessage2& aMessage);
	void									SetDeferTimeL(const RMessage2& aMessage);
	TBool									MultiDispatchL(const RMessage2& aMessage);
	void									CmdSetUserTimeL(const RMessage2& aMessage);
	void									CmdSetMaxAlarmsL(const RMessage2& aMessage);
	void									CmdNotifyOnAlarmL(const RMessage2& aMessage);
	void									CmdNotifyOnSoundStartL(const RMessage2& aMessage);
	void									CmdNotifyOnSoundStopL(const RMessage2& aMessage);
	void									CmdNotifyOnVisibleL(const RMessage2& aMessage);
	void									CmdNotifyOnStateL(const RMessage2& aMessage);
	void									CmdResponseSnoozeAlarmL(const RMessage2& aMessage);
	void									CmdResponseSilenceAlarmL(const RMessage2& aMessage);
	void									CmdResponseAcknowledgeAlarmL(const RMessage2& aMessage);
	void									CmdResponsePauseSoundL(const RMessage2& aMessage);
	void 									CmdSetAlarmL(const RMessage2& aMessage);
	void									CmdSetStateL(const RMessage2& aMessage);
	void									CmdVisibleL(const RMessage2& aMessage);
	void									CmdStartPlayingSoundL(const RMessage2& aMessage);
	void									CmdStopPlayingSoundL(const RMessage2& aMessage);
	void 									CmdSetDeferTimeL(const RMessage2& aMessage);
	void									CmdGetUserTimeL(const RMessage2& aMessage);
	void									CancelNotifications();
	void 									CmdResponseAcknowledgeAll();
	void 									CmdGetMaxAlarmsL(const RMessage2& aMessage);
	void									CmdNotifyOnDeleteL(const RMessage2& aMessage);
	void									CmdDeleteAlarmL(const RMessage2& aMessage);
	static TBool							SoundPlaybackCallbackL(TAny* aSelf);

private:
	TTime									iDeferTime;
	TASShdAlarm								iAlarm;
	TInt									iAlarmAssociatedDataSize;
	TAgnAlarmInfo							iAlarmData;
	TBool									iLoggedOn;
	TBool									iHasMessage;
	TBool									iMsgComplete;
	RMessage2								iMsgPtr;
	CPeriodic*								iSoundCallbackTimer;
	CConsoleAlarmAlertLEDFlasher*			iSoundLEDFlasher;
	CConsoleAlarmAlertConsole*				iConsole;
	CConsoleAlarmAlertServer*				iServer;
	TAny*									iInstructionSet;
	TBool									iInstructionSetFound;
	RMessage2								iAlarmNotify;
	RMessage2								iSoundStartNotify;
	RMessage2								iSoundStopNotify;
	RMessage2								iVisibleNotify;
	RMessage2								iStateNotify;
	RMessage2								iDeleteNotify;
	__FLOG_DECLARATION_MEMBER;
	};

#endif
