/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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




#include "server.h"
#include <e32property.h>
#include "t_oomdummyplugin_properties.h"

inline CShutdown::CShutdown()
	:CTimer(-1)
	{CActiveScheduler::Add(this);}
inline void CShutdown::ConstructL()
	{CTimer::ConstructL();}
inline void CShutdown::Start()
	{After(KMyShutdownDelay);}

inline CMyServer::CMyServer()
	:CServer2(CActive::EPriorityStandard, ESharableSessions)
	{}

inline CMySession::CMySession()
	{}
inline CMyServer& CMySession::Server()
	{return *static_cast<CMyServer*>(const_cast<CServer2*>(CSession2::Server()));}
inline TBool CMySession::ReceivePending() const
	{return !iReceiveMsg.IsNull();}


///////////////////////

void CMySession::CreateL()
//
// 2nd phase construct for sessions - called by the CServer framework
//
	{
	Server().AddSession();
	}

CMySession::~CMySession()
	{
	Server().DropSession();
	}

void CMySession::Send(const TDesC& aMessage)
//
// Deliver the message to the client, truncating if required
// If the write fails, panic the client, not the sender
//
	{
	if (ReceivePending())
		{
		TPtrC m(aMessage);
		if (iReceiveLen<aMessage.Length())
			m.Set(m.Left(iReceiveLen));
		TInt r=iReceiveMsg.Write(0,m);
		if (r==KErrNone)
			iReceiveMsg.Complete(KErrNone);
		else
			PanicClient(iReceiveMsg,EPanicBadDescriptor);
		}
	}

void CMySession::ServiceL(const RMessage2& aMessage)
//
// Handle a client request.
// Leaving is handled by CMyServer::ServiceError() which reports
// the error code to the client
//
	{
	switch (aMessage.Function())
		{
		case EAllocServReset:
			aMessage.Complete(Server().AllocManager().Reset());
			break;
		case EAllocServConfig:
			Server().AllocManager().ConfigureL(aMessage.Int0(),
					aMessage.Int1(), aMessage.Int2(), aMessage.Int3());
			aMessage.Complete(KErrNone);
			break;
		case EAllocServStart:
			aMessage.Complete(Server().AllocManager().StartAllocating());
			break;
		case EAllocServStop:
			aMessage.Complete(Server().AllocManager().StopAllocating());
			break;
		case EAllocServMemoryLow:
			aMessage.Complete(Server().AllocManager().MemoryLow(aMessage.Int0()));
			break;
		case EAllocServMemoryGood:
			aMessage.Complete(Server().AllocManager().MemoryGood(aMessage.Int0()));
			break;
		default:
			PanicClient(aMessage, EPanicIllegalFunction);
			break;
		}
	}

void CMySession::ServiceError(const RMessage2& aMessage,TInt aError)
//
// Handle an error from CMySession::ServiceL()
// A bad descriptor error implies a badly programmed client, so panic it;
// otherwise use the default handling (report the error to the client)
//
	{
	if (aError==KErrBadDescriptor)
		PanicClient(aMessage,EPanicBadDescriptor);
	CSession2::ServiceError(aMessage,aError);
	}

void CShutdown::RunL()
//
// Initiate server exit when the timer expires
//
	{
	CActiveScheduler::Stop();
	}

CServer2* CMyServer::NewLC()
	{
	CMyServer* self=new(ELeave) CMyServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CMyServer::ConstructL()
//
// 2nd phase construction - ensure the timer and server objects are running
//
	{
	StartL(KAllocServerName);
	iShutdown.ConstructL();
	// ensure that the server still exits even if the 1st client fails to connect
	iShutdown.Start();
	DefinePropertiesL();
	iAllocManager = CAllocManager::NewL();
	}

_LIT_SECURITY_POLICY_PASS(KAlwaysPass);

void CMyServer::DefinePropertiesL()
	{
	TInt err;
	for(TUint i=KUidOOMDummyPluginFirstImplementation;i<=KUidOOMDummyPluginLastImplementation;i++)
		{
		err = RProperty::Define(KUidOomPropertyCategory, i + KOOMDummyPluginLowMemoryCount, RProperty::EInt, KAlwaysPass, KAlwaysPass);
		if(err != KErrNone && err != KErrAlreadyExists) User::Leave(err);
		err = RProperty::Define(KUidOomPropertyCategory, i + KOOMDummyPluginGoodMemoryCount, RProperty::EInt, KAlwaysPass, KAlwaysPass);
		if(err != KErrNone && err != KErrAlreadyExists) User::Leave(err);
		err = RProperty::Define(KUidOomPropertyCategory, i + KOOMDummyPluginCurrentAllocationBytes, RProperty::EInt, KAlwaysPass, KAlwaysPass);
		if(err != KErrNone && err != KErrAlreadyExists) User::Leave(err);
		err = RProperty::Define(KUidOomPropertyCategory, i + KOOMDummyPluginBytesRequested, RProperty::EInt, KAlwaysPass, KAlwaysPass);
		if(err != KErrNone && err != KErrAlreadyExists) User::Leave(err);
		}
	}

CSession2* CMyServer::NewSessionL(const TVersion&,const RMessage2&) const
//
// Cretae a new client session. This should really check the version number.
//
	{
	return new(ELeave) CMySession();
	}

void CMyServer::AddSession()
//
// A new session is being created
// Cancel the shutdown timer if it was running
//
	{
	++iSessionCount;
	iShutdown.Cancel();
	}

void CMyServer::DropSession()
//
// A session is being destroyed
// Start the shutdown timer if it is the last session.
//
	{
	if (--iSessionCount==0)
		iShutdown.Start();
	}

void CMyServer::Send(const TDesC& aMessage)
//
// Pass on the signal to all clients
//
	{
	iSessionIter.SetToFirst();
	CSession2* s;
	while ((s=iSessionIter++)!=0)
		static_cast<CMySession*>(s)->Send(aMessage);
	}

CMyServer::~CMyServer()
	{
	delete iAllocManager;
	}

CAllocManager& CMyServer::AllocManager()
	{
	return *iAllocManager;
	}

void PanicClient(const RMessagePtr2& aMessage,TMyPanic aPanic)
//
// RMessage::Panic() also completes the message. This is:
// (a) important for efficient cleanup within the kernel
// (b) a problem if the message is completed a second time
//
	{
	_LIT(KPanic,"MyServer");
	aMessage.Panic(KPanic,aPanic);
	}

static void RunServerL()
//
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
//
	{
	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(RThread::RenameMe(KAllocServerName));
	//
	// create and install the active scheduler we need
	CActiveScheduler* s=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(s);
	CActiveScheduler::Install(s);
	//
	// create the server (leave it on the cleanup stack)
	CMyServer::NewLC();
	//
	// Initialisation complete, now signal the client
	RProcess::Rendezvous(KErrNone);
	//
	// Ready to run
	CActiveScheduler::Start();
	//
	// Cleanup the server and scheduler
	CleanupStack::PopAndDestroy(2);
	}

TInt E32Main()
//
// Server process entry-point
//
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup=CTrapCleanup::New();
	TInt r=KErrNoMemory;
	if (cleanup)
		{
		TRAP(r,RunServerL());
		delete cleanup;
		}
	//
	__UHEAP_MARKEND;
	return r;
	}
