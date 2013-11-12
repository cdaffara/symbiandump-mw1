// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#include <e32std.h>

#include "appfwk_test_utils_server.h"
#include "appfwk_test_utils.h"

inline CShutdown::CShutdown()
	:CTimer(-1)
	{CActiveScheduler::Add(this);}

inline void CShutdown::ConstructL()
	{CTimer::ConstructL();}

inline void CShutdown::Start()
	{After(KMyShutdownDelay);}

inline CSmlTestUtilsServer::CSmlTestUtilsServer()
	:CServer2(0,ESharableSessions)
	{}

inline CSmlTestUtilsSession::CSmlTestUtilsSession()
	{}

inline CSmlTestUtilsServer& CSmlTestUtilsSession::Server()
	{return *static_cast<CSmlTestUtilsServer*>(const_cast<CServer2*>(CSession2::Server()));}


/**
  RMessage::Panic() also completes the message. This is:
	(a) important for efficient cleanup within the kernel
	(b) a problem if the message is completed a second time
*/
static void PanicClient(const RMessage2& aMessage,TTestPanic aPanic)
	{
	_LIT(KPanic,"TestServer");
 	aMessage.Panic(KPanic,aPanic);
	}


void CSmlTestUtilsSession::CreateL()
	{
	Server().AddSession();
	User::LeaveIfError(iFs.Connect());
	iFileMan = CFileMan::NewL(iFs);
	}


CSmlTestUtilsSession::~CSmlTestUtilsSession()
	{
	Server().DropSession();
	iFs.Close();
	delete iFileMan;
	}


void CSmlTestUtilsSession::ServiceL(const RMessage2& aMessage)
	{
	TInt result = KErrNone;

	switch (aMessage.Function())
		{
	case ECreateDir:
		result = DoCreateDirectoryL(aMessage);
		break;
	case ERenameDir:
		result = DoRenameDirectoryL(aMessage);
		break;
	case EDeleteDir:
		result = DoDeleteDirectoryL(aMessage);
		break;
	case ECreateFile:
		result = DoCreateFileL(aMessage);
		break;
	case EDeleteFile:
		result = DoDeleteFileL(aMessage);
		break;
	case EDeleteFileUsingWildcard:
		result = DoDeleteFileUsingWildcardL(aMessage);
		break;
	case ECopyFile:
		result = DoCopyFileL(aMessage);
		break;
	case EReplaceFile:
		result = DoReplaceFileL(aMessage);
		break;
	case EIsFilePresent:
		result = DoIsFilePresentL(aMessage);
		break;
	case ESetReadOnly:
		result = DoSetReadOnlyL(aMessage);
		break;
	case EGetAttributes:
		result = DoGetAttL( aMessage );
		break;
	case ESetAttributes:
		result = DoSetAttL( aMessage );
		break;		
	case ECopyDirectory:
		result = DoCopyDirectoryL(aMessage);
		break;
	case EChangeFilePermission:
		result = DoChangeFilePermissionL(aMessage);
		break;	
	default:
		PanicClient(aMessage,ETestPanicIllegalFunction);
		break;
		}
	aMessage.Complete(result);
	}
	

TInt CSmlTestUtilsSession::DoCreateDirectoryL(const RMessage2& aMessage)
	{
	TBuf<255> path;
	aMessage.ReadL(0, path);
	TInt ret = iFs.MkDirAll(path);
	return ret;
	}
	
	
TInt CSmlTestUtilsSession::DoRenameDirectoryL(const RMessage2& aMessage)
	{
	TBuf<100> srcpath;
	aMessage.ReadL(0, srcpath);
	TBuf<100> destpath;
	aMessage.ReadL(1, destpath);
	TInt ret = iFs.Rename(srcpath, destpath);
	return ret;	
	}
	
	
TInt CSmlTestUtilsSession::DoDeleteDirectoryL(const RMessage2& aMessage)
	{
	TBuf<255> path;
	aMessage.ReadL(0, path);
	TInt ret = iFs.RmDir(path);
	return ret;
	}
	
	
TInt CSmlTestUtilsSession::DoCreateFileL(const RMessage2& aMessage)
	{
	TBuf<100> path;
	aMessage.ReadL(0, path);
	RFile file;
	CleanupClosePushL(file);
	TInt ret = file.Create(iFs, path, EFileRead);
	CleanupStack::PopAndDestroy(&file);
	return ret;
	}
	
	
TInt CSmlTestUtilsSession::DoDeleteFileL(const RMessage2& aMessage)
	{
	TBuf<100> path;
	aMessage.ReadL(0, path);
	TInt ret = iFs.Delete(path);
	return ret;	
	}
	

TInt CSmlTestUtilsSession::DoDeleteFileUsingWildcardL(const RMessage2& aMessage)
	{
	TBuf<100> path;
	aMessage.ReadL(0, path);
	TInt ret = iFileMan->Delete(path);
	return ret;	
	}


TInt CSmlTestUtilsSession::DoCopyFileL(const RMessage2& aMessage)
	{
	TBuf<100> srcpath;
	aMessage.ReadL(0, srcpath);
	TBuf<100> destpath;
	aMessage.ReadL(1, destpath);
	TInt ret = iFileMan->Copy(srcpath,destpath);
	return ret;	
	}
	

TInt CSmlTestUtilsSession::DoReplaceFileL(const RMessage2& aMessage)
	{
	TBuf<100> srcpath;
	aMessage.ReadL(0, srcpath);
	TUint lFileMode;
	TPckgBuf<TUint> temp;
	aMessage.ReadL(1, temp);
	lFileMode=temp();
	RFile file;
	TInt ret = file.Replace(iFs,srcpath,lFileMode);
	return ret;	
	}
	

TInt CSmlTestUtilsSession::DoIsFilePresentL(const RMessage2& aMessage)
	{
	TFileName srcpath;
	aMessage.ReadL(0, srcpath);
	TEntry entry;
	TInt err = iFs.Entry(srcpath, entry);
	if (err == KErrNone)
		{
		aMessage.WriteL(1,TPckgBuf<TBool>(ETrue));
		}
	else
		{
		aMessage.WriteL(1,TPckgBuf<TBool>(EFalse));
		}
	return KErrNone;
	}


TInt CSmlTestUtilsSession::DoSetReadOnlyL(const RMessage2& aMessage)
	{
	TBuf<100> srcpath;
	aMessage.ReadL(0, srcpath);
	TUint attMask;
	TPckgBuf<TUint> temp;
	aMessage.ReadL(1, temp);
	attMask=temp();
	TInt ret = iFs.SetAtt(srcpath,attMask,KEntryAttReadOnly);
	return ret;	
	}
	
	
TInt CSmlTestUtilsSession::DoGetAttL( const RMessage2& aMessage )
	{
	TFileName nameBuf;
	TUint attributes = 0;
	aMessage.ReadL( 0, nameBuf );
	TInt err = iFs.Att( nameBuf, attributes );	
	if ( KErrNone == err )
		{
		aMessage.WriteL( 1, TPckgBuf<TInt>(attributes) );
		}
	return err;
	}


TInt CSmlTestUtilsSession::DoSetAttL( const RMessage2& aMessage )
	{
	
	TFileName nameBuf;
	aMessage.ReadL( 0, nameBuf );
	TUint setAttMask = *(TUint*)aMessage.Ptr1();
	TUint clearAttMask = *(TUint*)aMessage.Ptr2();
	TInt err = iFs.SetAtt( nameBuf, setAttMask, clearAttMask );
	return err;
	}
	
	
/**
  Handle an error from CMySession::ServiceL()
  A bad descriptor error implies a badly programmed client, so panic it;
  otherwise use the default handling (report the error to the client)
*/
void CSmlTestUtilsSession::ServiceError(const RMessage2& aMessage,TInt aError)
	{
	if (aError==KErrBadDescriptor)
		PanicClient(aMessage,ETestPanicBadDescriptor);
	CSession2::ServiceError(aMessage,aError);
	}


TInt CSmlTestUtilsSession::DoCopyDirectoryL(const RMessage2& aMessage)
	{
	TBuf<100> source;
	aMessage.ReadL(0, source);
	TBuf<100> target;
	aMessage.ReadL(1, target);
	TInt ret = iFileMan->Copy(source,target, CFileMan::ERecurse);
	return ret;		
	}


/**
  Initiate server exit when the timer expires
*/
void CShutdown::RunL()
	{ // this implementation of this method does not leave but further derived implementations may if they choose
	CActiveScheduler::Stop();
	}


CServer2* CSmlTestUtilsServer::NewLC()
	{
	CSmlTestUtilsServer* self=new(ELeave) CSmlTestUtilsServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


/**
  2nd phase construction - ensure the timer and server objects are running
*/
void CSmlTestUtilsServer::ConstructL()
	{
	StartL(KTestServerName);
	iShutdown.ConstructL();
	// ensure that the server still exits even if the 1st client fails to connect
	iShutdown.Start();
	}


/**
  Cretae a new client session. This should really check the version number.
*/
CSession2* CSmlTestUtilsServer::NewSessionL(const TVersion&,const RMessage2&) const
	{
	return new(ELeave) CSmlTestUtilsSession();
	}


/**
  A new session is being created. Cancel the shutdown timer if it was running
*/
void CSmlTestUtilsServer::AddSession()
	{
	++iSessionCount;
	iShutdown.Cancel();
	}


/**
  A session is being destroyed. Start the shutdown timer if it is the last session.
*/
void CSmlTestUtilsServer::DropSession()
	{
	if (--iSessionCount==0)
		{
		iShutdown.Start();
		}
	}
	

TInt CSmlTestUtilsSession::DoChangeFilePermissionL(const RMessage2& aMessage)
	{
	TInt ret(0);
	TRequestStatus status;
	TTime time((TInt64)0); 
	// lint warns of custodial pointer (fileman) not being freed,
	// which is done via the cleanup stack, hence disabled here
	//lint -esym(429,fileman)
	CFileMan* fileman = CFileMan::NewL (iFs);
	CleanupStack::PushL(fileman);
	TBuf<100> path;
	aMessage.ReadL(0, path);
	ret = fileman->Attribs(path,KEntryAttNormal,KEntryAttReadOnly, time,0,status);
	User::WaitForRequest(status); 
	ret = status.Int(); 
	CleanupStack::PopAndDestroy(); 
	return ret;	
	}

