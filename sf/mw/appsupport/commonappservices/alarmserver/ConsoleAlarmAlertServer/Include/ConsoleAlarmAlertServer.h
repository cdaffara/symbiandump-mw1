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


 
#ifndef __CONSOLEALARMALERTSERVER_H__
#define __CONSOLEALARMALERTSERVER_H__

#include <e32base.h>

#define __FILELOGGER_ACTIVE // Include logging in release builds
#include <test/rfileloggermacro.h>
#include <asshdalarm.h>

_LIT(KLogFile, "C:\\Logs\\ConsoleAlarmAlertServer\\ConsoleAlarmAlertServer.log");

class CConsoleAlarmAlertSession;

class CConsoleAlarmAlertServer : public CPolicyServer
	{
public:
	static CConsoleAlarmAlertServer* NewLC();
	~CConsoleAlarmAlertServer();

public:
	inline void								AddResponseArray(TAny* aResponseArray);
	void									SetAttachment(HBufC8* aData);
	inline HBufC8*							Attachment() const;
	inline TAny*							ResponseArray() const;
	inline CConsoleAlarmAlertSession*		WaitingSession() const;
	inline void								SetWaitingSession(CConsoleAlarmAlertSession* aSession);
	inline void								SetNotifying(TBool aIsNotifying);
	inline TBool							IsNotifying() const;

	inline TBool							IsExtendedMode() const;
	inline void								SetExtendedMode(const TBool aMode);
	
	inline void								SetUserTime(const TTime& aTime);
	inline const TTime& 					UserTime() const;
	inline void								SetMaxAlarms(const TInt aMaxAlarms);
	inline const TInt						MaxAlarms() const;
	
	inline RMessage2*						NotifyMessage() const;
	inline void								SetNotifyMessage(RMessage2* aMessage);
	
	void									NotifyAlarmObserversL(const TASShdAlarm& aAlarm, const TFullName& aFullName, const TDes8& aData);
	void									NotifySoundStartObserversL(const TAlarmId& aId, const TDesC& aFilename);
	void									NotifySoundStopObserversL(const TAlarmId& aId);
	void									NotifyVisibleObserversL(const TAlarmId& aId, const TBool aVisible);
	void									NotifyStateObserversL(const TAlarmId& aId, const TInt aState);
	void									NotifyDeleteObserversL(const TAlarmId& aId);

	enum TObserverIndex
		{
		EAlarmObserver,
		ESoundStartObserver,
		ESoundStopObserver,
		EVisibleObserver,
		EStateObserver,
		EDeleteObserver
		};

	void									AddObserverL(TObserverIndex aIndex, CConsoleAlarmAlertSession* aObserver);
	void									RemoveObserver(TObserverIndex aIndex, CConsoleAlarmAlertSession* aObserver);
	
	
private:
	CConsoleAlarmAlertServer();
	void									ConstructL();

public: // from CServer2
	CSession2*								NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;

private:
	TAny*									iInstructionSet;
	HBufC8*									iAttachment;
	CConsoleAlarmAlertSession*				iWaitingSession;
	TBool									iIsNotifying;

	TBool									iExtendedMode;
	TInt									iMaxAlarms;
	TTime									iUserTime;
	RMessage2* 								iNotify;
		
	RPointerArray<CConsoleAlarmAlertSession> iObservers[6];
	__FLOG_DECLARATION_MEMBER;
	
	};

#include "ConsoleAlarmAlertServer.inl"

#endif
