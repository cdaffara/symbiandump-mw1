// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file ServerMsgSender.cpp
*/

#include <e32base.h>
#include <s32std.h>
#include "DstStep.h"
 
#include "ServerMsgSender.h"
#include "DstStep.h"
#include <tz.h>

struct TRendezvous
	{
	TThreadId iId;
	TRequestStatus* iStatus;
	};

//===========================================================================
// Class: RServerMsgSender
//===========================================================================

RServerMsgSender::RServerMsgSender()
    {
    }
    
RServerMsgSender::~RServerMsgSender()
    {
	Close();
    }
	
TInt RServerMsgSender::StartServerL()
	{
	const TInt KIntServerType = 0x10004019;
	const TUidType serverUid( KNullUid, TUid::Uid( KIntServerType ) );
	
	TRequestStatus started = KRequestPending;
	TRendezvous rendezvous;
	rendezvous.iId = RThread().Id(); // Id of this thread.
	rendezvous.iStatus = &started;

    // Name of the time zone server.
    _LIT( KServerName, "TZSERVER" );
	RProcess server;
	TInt err = server.Create( KServerName,
        TPtrC(reinterpret_cast<TText*>(&rendezvous),
		sizeof(rendezvous)/sizeof(TText)), serverUid );

    if (err != KErrNone)
        {
        return err;
        }

	server.SetPriority(EPriorityHigh);

	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat != KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
		}

	User::WaitForRequest( stat );		// wait for start or death
	server.Close();
	return stat.Int();
    }
   
TInt RServerMsgSender::Connect()
	{
    _LIT( KServerName, "!TzServer" );
    TVersion version( 0, 0, 0 );
    const TInt KNumMsgSlots = 255;
	
    TInt err = CreateSession( KServerName, version, KNumMsgSlots );
    return err;
	}
    
TInt RServerMsgSender::SendMessage(TInt aMsg, TIpcArgs &aArgs)
	{
	TInt err = SendReceive( aMsg, aArgs );
	return err;
	}
    
//===========================================================================
// Class: CServerMsgSenderTestStep
//===========================================================================

CServerMsgSenderTestStep::CServerMsgSenderTestStep()
 	{
 	SetTestStepName( KServerMsgSenderStep );
 	}
 	
CServerMsgSenderTestStep::~CServerMsgSenderTestStep()
	{
	}
	
TVerdict CServerMsgSenderTestStep::doTestStepPreambleL()
	{	
 	iMsgSender.StartServerL();
 	User::LeaveIfError( iMsgSender.Connect() );

    return TestStepResult();
	}

TVerdict CServerMsgSenderTestStep::doTestStepPostambleL()
    {
    return TestStepResult();
    }

TVerdict CServerMsgSenderTestStep::doTestStepL()
    {
    SetTestStepResult( EFail );

    _LIT( KFunction, "Function" );
    _LIT( KVal1,     "Val1" );
    _LIT( KVal2,     "Val2" );
    _LIT( KVal3,     "Val3" );
    _LIT( KVal4,     "Val4" );
    _LIT( KNumArgs,  "NumArgs" );

    // Read the data to send to the server from the ini file. The function is
    // message that is sent to the server and the arguments are 4 32 bit values
    // that are sent in the first argument of data.
    TInt func = 0;
    GetIntFromConfig( ConfigSection(), KFunction, func );

    // Read the 4 4-byte values.
    TInt val1 = 0;
    GetIntFromConfig( ConfigSection(), KVal1, val1 );

    TInt val2 = 0;
    GetIntFromConfig( ConfigSection(), KVal2, val2 );

    TInt val3 = 0;
    GetIntFromConfig( ConfigSection(), KVal3, val3 );

    TInt val4 = 0;
    GetIntFromConfig( ConfigSection(), KVal4, val4 );
    
    // Read the number of arguments.
    TInt numArgs = 1;
    GetIntFromConfig( ConfigSection(), KNumArgs, numArgs );
    if (numArgs > 4) 
        {
        numArgs = 4;
        }
    else if (numArgs < 0)
        {
        numArgs = 0;
        }

    TIpcArgs args;
    HBufC8* valBuf = NULL;
    // 'EGetOffsetsForTimeZoneIds' wants the data as a TInt in arg 0. Val2-4
    // and NumArgs is ignored for this message.
    const TInt KGetOffsetsForTimeZoneIds = 14;
    if (func == KGetOffsetsForTimeZoneIds)
        {
        args.Set( 0, val1 );
        }
    else
        {
        // All four values are sent to the server in one 16 byte long buffer.
        TUint8 valData[16];
        memcpy( &valData[0],  &val1, sizeof(TInt) );
        memcpy( &valData[4],  &val2, sizeof(TInt) );
        memcpy( &valData[8],  &val3, sizeof(TInt) );
        memcpy( &valData[12], &val4, sizeof(TInt) );
        
        // Copy the data from the array into a buffer because that's the way 
        // the server wants it.
        TInt size = sizeof(valData);
        valBuf = HBufC8::NewL( size );
        TPtr8 ptr = valBuf->Des();
        ptr.Set( &valData[0], size, size );
        
        // Copy the data into however many arguments the ini file specified.
        // Some messages the server recieves expect more than 1 argument and
        // so this is done so that the server doesn't leave just because the
        // right number of arguments wasn't received, even though the actual 
        // data may be meaningless.
        for (TInt argNum = 0; argNum < numArgs; argNum++)
            {
            args.Set( argNum, &ptr );
            }
        }

    // Send message to server.
    TInt err = iMsgSender.SendMessage( func, args );
    
    if (valBuf)
        {
        delete valBuf;
        }

    User::LeaveIfError( err );

    return TestStepResult();
    }
