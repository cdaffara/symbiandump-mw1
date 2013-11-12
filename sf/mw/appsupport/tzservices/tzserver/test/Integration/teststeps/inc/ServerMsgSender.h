/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __SERVER_MSG_SENDER_H
#define __SERVER_MSG_SENDER_H

#include <tz.h>
#include <test/testexecutestepbase.h>

// Name of the test step.
_LIT( KServerMsgSenderStep, "ServerMsgSender" );

/**
* Interface to the time zone server. 
*/
class RServerMsgSender : public RSessionBase
{
public:
    RServerMsgSender();
    ~RServerMsgSender();
    
    TInt StartServerL();
    TInt Connect();
    TInt SendMessage(TInt aMsg, TIpcArgs &aArgs);
};

/**
* Reads data from the ini file that is used to create a message. The message 
* is sent to the time zone server in the test step. The data consists of a 
* function which defines the type of message, up to 4 4-byte values that are
* combined into a 16 byte buffer, and the number of arguments the message 
* contains. There can be a maximum of 4 arguments and the same 16 byte value
* defined by the values (Val1-4) are used for each argument.
*/
class CServerMsgSenderTestStep : public CTestStep
	{
public:
	CServerMsgSenderTestStep();
	~CServerMsgSenderTestStep();

private:
	// Methods from CTestStep.
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();

private:
    RServerMsgSender iMsgSender;
	};

#endif
