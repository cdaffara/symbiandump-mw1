/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



void CConsoleAlarmAlertServer::AddResponseArray(TAny* aResponseArray)		
	{ 
	iInstructionSet = aResponseArray; 
	}
	
HBufC8* CConsoleAlarmAlertServer::Attachment() const						
	{ 
	return iAttachment;
	}
	
TAny* CConsoleAlarmAlertServer::ResponseArray() const						
	{ 
	return iInstructionSet;
	}
	
CConsoleAlarmAlertSession* CConsoleAlarmAlertServer::WaitingSession() const	
	{ 
	return iWaitingSession;
	}
	
void CConsoleAlarmAlertServer::SetWaitingSession(CConsoleAlarmAlertSession* aSession)
	{ 
	iWaitingSession = aSession;
	}
	
void CConsoleAlarmAlertServer::SetNotifying(TBool aIsNotifying)				
	{
	iIsNotifying = aIsNotifying;
	}
	
TBool CConsoleAlarmAlertServer::IsNotifying()	const						
	{
	return iIsNotifying;
	}

/**
Asks the Console Alarm Alert Server if it's in extended mode or not.

@return ETrue if in extended mode, EFalse otherwise.
*/

TBool CConsoleAlarmAlertServer::IsExtendedMode() const
	{
	return iExtendedMode;
	}

/**
Sets the Console Alarm Alert Server to extended mode. Use this mode to enable
new RANTestClient API calls, enable logging and test multiple alarm functionality.

@param aMode, ETrue enables extended mode. EFalse disables extended mode.
*/

void CConsoleAlarmAlertServer::SetExtendedMode(const TBool aMode)		
	{
	iExtendedMode = aMode;
	iMaxAlarms = 1;
	}

/**
Sets the Console Alarm Alert Server user time value.

@param aTime, time to use as user time.
*/	

void CConsoleAlarmAlertServer::SetUserTime(const TTime& aTime)
	{
	iUserTime = aTime;
	}

/**
Gets the Console Alarm Alert Server user time value.
The default value is Time::NullTTime()

@return the user time.
*/

const TTime& CConsoleAlarmAlertServer::UserTime() const
	{
	return iUserTime;
	}
	
/**
Sets the Console Alarm Alert Server max alarms value.

@param aMaxAlarms, value to use as max alarms.
*/	
	
void CConsoleAlarmAlertServer::SetMaxAlarms(const TInt aMaxAlarms)
	{
	iMaxAlarms = aMaxAlarms;
	}

/**
Gets the Console Alarm Alert Server max alarms value.

@return value used as max alarms.

*/	

const TInt CConsoleAlarmAlertServer::MaxAlarms() const
	{
	return iMaxAlarms;
	}

/**
Gets a pointer to the latest EASAltOpCodeNotify message.

@return a pointer to the message.
*/	

RMessage2* CConsoleAlarmAlertServer::NotifyMessage() const
	{
	return iNotify;
	}

/**
Sets the pointer to the latest EASAltOpCodeNotify message.

@param aMessage, the address of the latest EASAltOpCodeNotify message.
*/	

void CConsoleAlarmAlertServer::SetNotifyMessage(RMessage2* aMessage)
	{
	iNotify = aMessage;
	}
	
