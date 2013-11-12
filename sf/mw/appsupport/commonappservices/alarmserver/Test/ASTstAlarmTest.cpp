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

#include "ASTstAlarmTest.h"
#include "testserver.h"

#ifdef __WINS__ 
const TUid KServerUid2={0x1000008D};
const TUid KServerUid3={0x101F502A};
#endif


//
// ----> ASTstAlarmTest (source)
//

//*************************************************************************************
EXPORT_C ASTstAlarmTest::ASTstAlarmTest()
: iTest(KAlarmTestCodeTitle)
	{
	iAlarmServer = RASCliSession();
	iInitialHomeTime.HomeTime();
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::Close()
	{
	ASTstAlarmTest* tlsSelf = reinterpret_cast<ASTstAlarmTest*>(Dll::Tls());
	delete tlsSelf;
	Dll::SetTls(NULL);
	}


//*************************************************************************************
EXPORT_C ASTstAlarmTest& ASTstAlarmTest::Self()
	{
	ASTstAlarmTest* tlsSelf = reinterpret_cast<ASTstAlarmTest*>(Dll::Tls());
	if	(!tlsSelf)
		{
		tlsSelf = new ASTstAlarmTest; 
		ASSERT(tlsSelf != NULL);
		Dll::SetTls(tlsSelf);
		}
	return *tlsSelf;
	}


//
//
//


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::operator()(TInt aResult, TInt aLineNum, const TText* aFileName)
	{
	if (!aResult)
		resetHomeTimeL();
	iTest.operator()(aResult, aLineNum, aFileName);
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::operator()(TInt aResult, TInt aLineNum)
	{
		
	if (!aResult)
		resetHomeTimeL();
	iTest.operator()(aResult, aLineNum);
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::operator()(TInt aResult)
	{
	if (!aResult)
		resetHomeTimeL();
	iTest.operator()(aResult);
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::Title()
	{
	iTest.Title();
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::Start(const TDesC& aHeading)
	{
	iTest.Start(aHeading);
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::Next(const TDesC& aHeading)
	{
	iTest.Next(aHeading);
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::EndL()
	{
	iTest.End();
	resetHomeTimeL();
	}

//*************************************************************************************
EXPORT_C void ASTstAlarmTest::UpLevel()
	{
	iTest.End();
	}


//
//
//


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::TestClearStoreL()
//
//	Clear any settings that may have been stored
//  Method could leave when trying to get list of all alarm categories in use within the alarm server.
//
	{
	iTest(iAlarmServer.Handle() != KNullHandle);

	// Delete all alarms
	RArray<TAlarmCategory> categories;
	iAlarmServer.GetAvailableCategoryListL(categories); 
	TInt count = categories.Count();
	for(TInt i=0; i<count; i++)
		{
		const TAlarmCategory category = categories[i];
		iTest(iAlarmServer.AlarmDeleteAllByCategory(category, EFalse) == KErrNone);
		}
	categories.Close();

	// Restore alarm sound
	iAlarmServer.SetAlarmSoundState(EAlarmGlobalSoundStateOn);

	// Check there aren't any queued alarms (not a particularly good
	// test, but better than nothing).
	count = iAlarmServer.NumberOfAlarmsActiveInQueue();
	iTest(!count);
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::WaitForNotificationBufferToBeEmptied(TRequestStatus& aStatus, TAlarmId& aAlarmId)
	{
	if	(aStatus == KRequestPending)
		return;

	iAlarmServer.NotifyChange(aStatus, aAlarmId);
	User::After(KTimeToWait);
	if	(aStatus == KRequestPending)
		return;
	User::WaitForRequest(aStatus);

	// Because of event bufferring inside the Alarm Server
	// We need to wait until there aren't any more events
	// in the buffer, and then attempt to cancel the request.
	//
	// Trying to cancel a request when the server has
	// already completed the message (because something
	// was in the buffer) results in the server panicking
	// the client.
	FOREVER
		{
		iTest(aStatus >= EAlarmChangeEventState && aStatus <= EAlarmChangeEventLast);
		iAlarmServer.NotifyChange(aStatus, aAlarmId);
		//
		// Wait a second so that we can tell if this really is
		// waiting for an event, or just for the Alarm Server
		// process to be given some scheduling time by the
		// kernel
		User::After(KTimeToWait);
		
		// If its still pending, then we should attempt
		// to cancel the request. Otherwise, if its not
		// pending, then the Alarm Server probably just
		// completed the request based upon the contents
		// of the event buffer.
		if	(aStatus == KRequestPending)
			break;
		else
			{
			// Eat the request and try again
			User::WaitForRequest(aStatus);
			}
		}
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::WaitForEvent(TInt aEvent, TRequestStatus& aStatus, TAlarmId& aAlarmId)
	{
	FOREVER
		{
		iAlarmServer.NotifyChange(aStatus, aAlarmId);
		User::WaitForRequest(aStatus);
		if	(aStatus == aEvent)
			return;
		}
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::TestEventBuffer(TInt aExpected, TRequestStatus& aStatus, TAlarmId& aAlarmId)
//
//	Check there the notification of this thread. Because notifications
//	in the new Alarm Server are bufferred, then the first notification
//	may not be the one we are looking for. Therefore we continue to
//  request notifications until the buffer is exhausted, at which point,
//	if we haven't yet seent the notification we were looking for, we
//	panic.
//
	{
	if	(aStatus != aExpected)
		{
		// Need to exhaust the event buffer

		// If the buffer is empty (server side) then the current
		// request will still be outstanding, therefore the test
		// has failed.
		User::After(KTimeToWait);
		iTest(aStatus != KRequestPending);

		// Eat current request
		User::WaitForRequest(aStatus);

		// Now go through each item in the buffer, requesting them
		// one by one
		FOREVER
			{
			iAlarmServer.NotifyChange(aStatus, aAlarmId);
			User::After(KTimeToWait);
			iTest(aStatus != KRequestPending);
			User::WaitForRequest(aStatus);
			if	(aStatus == aExpected)
				break;
			}
		}

	// Request another notification
	if	(aStatus != KRequestPending)
		{
		// Eat all the rest
		WaitForNotificationBufferToBeEmptied(aStatus, aAlarmId);
		}
	}


//*************************************************************************************
EXPORT_C TInt ASTstAlarmTest::CountOrphanedAlarmsL()
//
// Return the number of orphaned alarms
// Method could leave while getting the list of unique identifiers of all the alarms in the alarm server.
//
	{
	TASShdAlarm alarm;
	//
	RArray<TAlarmId> ids;
	iAlarmServer.GetAlarmIdListL(ids);   // Method could leave while getting the list of unique identifiers of all the alarms in the alarm server.

	TInt ret = 0;
	const TInt count = ids.Count();
	for(TInt i=0; i<count; i++)
		{
		const TAlarmId id = ids[i];
		const TInt error = iAlarmServer.GetAlarmDetails(id, alarm);

		// Alarm might have been deleted by another thread? Unlikely,
		// but best to check
		if	(error == KErrNone && alarm.HasBecomeOrphaned())
			++ret;
		}
	ids.Close();
	//
	return ret;
	}


//*************************************************************************************
EXPORT_C void ASTstAlarmTest::TestStartServers(TAny*
#if defined (__WINS__)
													 instructionSet
#endif
																   )
	{

#ifdef __WINS__ 

const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);

	_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
	RProcess server;
    TInt ret;
	if ( instructionSet != NULL)
	   {   
		TBuf<256> cmdline;
		RArray<TInt>* temp = static_cast<RArray<TInt>*>(instructionSet);
	    TInt len = temp->Count();
		for(TInt i=0;i<len;i++)
		   {
			cmdline.Append((*temp)[i]);
		   }
        ret=server.Create(KConsoleAlarmAlertServerImg,cmdline,serverUid);
        iTest(ret == KErrNone);
	   }
    else 
		{
        ret=server.Create(KConsoleAlarmAlertServerImg,KNullDesC,serverUid);
        iTest(ret == KErrNone);
		}

	TRequestStatus stat;
	server.Rendezvous(stat);

	if (stat!=KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
	}
#endif // __WINS__ 

	// Start the alarm server
	const TInt r = iAlarmServer.Connect();
	if	(r != KErrNone)
		{
		_LIT(KStartupFailure, "Couldn't connect to AS");
		User::Panic(KStartupFailure, 0);
		}
	}


void ASTstAlarmTest::resetHomeTimeL()
{
	RPIMTestServer serv;
    User::LeaveIfError(serv.Connect()); 
    serv.SetHomeTime(iInitialHomeTime);
    serv.Close();
}

//
// ----> Global Methods (source)
//

//*************************************************************************************
