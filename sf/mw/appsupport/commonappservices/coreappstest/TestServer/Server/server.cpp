// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "server.h"
#include <f32file.h>
#include <e32std.h>


namespace nsPIMTestServer
	{
	_LIT(KPanic,"PIMTestServer");	
	const TInt maxTry = 5;
	}


// ----------------------------------
// CTestSession Class implementation
// ==================================

void CTestSession::CreateL()
// 2nd phase construct for sessions - called by the CServer framework
	{
	iParamBuf = HBufC::NewL(KMaxFileName); 
	User::LeaveIfError(iTz.Connect());
	Server().AddSession();
	}


CTestSession::~CTestSession()
	{
	iTz.Close();
	delete iParamBuf;
	iParamBuf = 0;
	Server().DropSession();
	delete iNotifyChange;
	}

	
void CTestSession::ServiceL(const RMessage2& aMessage)
	{
	TInt result = KErrNone;
	
	TBool complete(ETrue);


 	switch (aMessage.Function())
		{
		case ETestIpcSetHomeTime:
			DoSetHomeTimeL(aMessage);
			break;
		case ETestIpcDeleteFile:
			DoDeleteFileL(aMessage);
			break;
		case ETestIpcKillProcess:
			DoKillProcessL(aMessage);
			break;
		case ETestIpcChangeLocale:
			DoChangeLocaleL(aMessage);
			break;
		case ETestIpcCheckForFile:
		    DoCheckForFileL(aMessage);
		    break;
		case ETestIpcCopyFile:
		    DoCopyFileL(aMessage);
		    break; 
		case ETestIpcCreateEmptyFile:
			DoCreateEmptyFileL(aMessage);
			break;
		case ETestIpcSetTimeZone:
			DoSetTimeZoneL(aMessage);
			break;
		case ETestIpcGetTimeZone:
			DoGetTimeZoneL(aMessage);
			break;
		case ETestIpcDeleteDir:
			DoDeleteDirL(aMessage);
			break;
		case ETestIpcGetFileSize:
			DoGetFileSizeL(aMessage);
			break;
		case ETestIpcNotifyChange:
			DoNotifyChangeL(aMessage);
			complete = EFalse;
			break;
		case ETestIpcNotifyChangeCancel:
			DoNotifyChangeCancel();
			break;
		case ETestIpcCloseTzSession:
			iTz.Close();
			break;
		case ETestIpcChangeFSAttributes:
			DoChangeFSAttributesL(aMessage);
			break;
		case ETestIpcDeleteFilesInDir:
			DoDeleteAllFilesInDirL(aMessage);
			break;
	    case ETestIpcCopyDirAndFiles:
	        DoCopyDirAndFilesL(aMessage);
	        break; 

		default:
			PanicClient(aMessage,EPanicIllegalFunction);
			break;
		}
 	if (complete)
  		{
  		aMessage.Complete(result);
  		}
	}



// Private Helper methods


void CTestSession::DoSetHomeTimeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> pckg;
	aMessage.ReadL(0, pckg);
	User::LeaveIfError(iTz.SetHomeTime(pckg()));
	}


void CTestSession::ReadMessageParamL(const RMessage2& aMessage, TInt aParamNo) const
	{
	TPtr16 strptr(iParamBuf->Des());
	aMessage.ReadL(aParamNo, strptr);
	}


// Private Power User methods

void CTestSession::DoDeleteFileL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);

	// Remove any read-only attribute from the file so that we can delete it.
	TInt err = GetRFs().SetAtt(*iParamBuf, KEntryAttNormal,	KEntryAttReadOnly);

	// ASync calls to the server can result in this delete method being 
	// called while a handle to the file still exists.
	TInt count = 0;
	while ((err == KErrInUse) && (count < nsPIMTestServer::maxTry))
		{
		User::After(500000);
		err = GetRFs().SetAtt(*iParamBuf, KEntryAttNormal,	KEntryAttReadOnly);
		++count;
		}
	
	err = GetRFs().Delete(iParamBuf->Des());
	
	// Ignore not-found error
	if (!(err==KErrNone || err==KErrNotFound || err==KErrPathNotFound)) 
		{
		User::Leave(err);
		}
	} 


void CTestSession::DoKillProcessL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);
	TFindProcess finder(*iParamBuf);
		
	RProcess process;
	CleanupClosePushL(process);
	
	TFileName tempName;
	TFileName lastFoundName;
	TInt err(KErrNone);
	
	do
		{
		lastFoundName.Copy(tempName);
		err = finder.Next(tempName);
		}
		while (err != KErrNotFound);
	
	User::LeaveIfError(process.Open(lastFoundName));
	
	TRequestStatus status;
	process.Logon(status);
	process.Kill(KErrNone); // No reason to kill
	User::WaitForRequest(status);
		
	CleanupStack::PopAndDestroy(&process);
	}



void CTestSession::DoChangeLocaleL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL 
	TExtendedLocale myExtendedLocale;
	_LIT(KEnglishLanguage, "elocl_lan.001") ;
	_LIT(KUKRegion, "elocl_reg.826") ;
	_LIT(KStandardCollation, "elocl_col.001") ;
	User::LeaveIfError(myExtendedLocale.LoadLocale(KEnglishLanguage(), KUKRegion(), KStandardCollation()));
	User::LeaveIfError(myExtendedLocale.LoadLocaleAspect(*iParamBuf));
	User::LeaveIfError(myExtendedLocale.SaveSystemSettings());
#else
	// The following line ensures UserSvr::ChangeLocale (which should 
	// really only happen at boot up) succeeds for the purposes of the test.
	UserSvr::ChangeLocale(KNullDesC);
	User::LeaveIfError(UserSvr::ChangeLocale(*iParamBuf));
#endif
	}



void CTestSession::DoCheckForFileL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);
	TUint attribs = KEntryAttNormal;		
	// if we leave with KErrExits this will be passed back to the client
	User::LeaveIfError(GetRFs().Att(*iParamBuf, attribs));
	}

void CTestSession::DoCopyFileL(const RMessage2& aMessage, TUint aTAction)
    {
    ReadMessageParamL(aMessage);
    HBufC* fileSourceBuf = iParamBuf->Des().AllocLC();

    ReadMessageParamL(aMessage, 1);
    
    CFileMan* fileMan = CFileMan::NewL(GetRFs());
    CleanupStack::PushL(fileMan);
    
    // only want to create a directory path. So we need to strip
    // any filenames if these where passed to us.
    TParse parser;
    parser.Set(*iParamBuf, NULL, NULL);
    TInt err = GetRFs().MkDirAll(parser.DriveAndPath());
    
    if (err  == KErrNone || err == KErrAlreadyExists)
        {
        // if readonly is not required, remove from here...
        err = fileMan->Copy(*fileSourceBuf, *iParamBuf, aTAction);
        if (err  == KErrNone || err == KErrAlreadyExists)
            {
            User::LeaveIfError(GetRFs().SetAtt(*iParamBuf, KEntryAttNormal, KEntryAttReadOnly));
            }
        else
            {
            // copy failed
            User::Leave(err);
            }
        }
    else
        {
        // mkdir failed
        User::Leave(err);       
        }
    // ...to here
    CleanupStack::PopAndDestroy(2); // fileSourceBuf, fileMan
    }


void CTestSession::DoCopyFileL(const RMessage2& aMessage)
	{
	DoCopyFileL(aMessage, CFileMan::EOverWrite );
	}

void CTestSession::DoCopyDirAndFilesL(const RMessage2& aMessage)
    {
    DoCopyFileL(aMessage, CFileMan::EOverWrite|CFileMan::ERecurse);
    }

void CTestSession::DoCreateEmptyFileL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);
	RFile file;
	CleanupClosePushL(file);
	// try to create the filder for the file
	TInt err = GetRFs().MkDirAll(iParamBuf->Des());
		
	if (err  == KErrNone || err == KErrAlreadyExists)
		{
		User::LeaveIfError(file.Replace(GetRFs(), iParamBuf->Des(), EFileWrite));
		}
	else
		{
		// mkdir failed
		User::Leave(err);		
		}

	CleanupStack::PopAndDestroy();
	}

void CTestSession::DoSetTimeZoneL(const RMessage2& aMessage)
	{
	TBuf8<256> tzBuffer;
	aMessage.ReadL(0, tzBuffer);
	CTestTzId* tzId = CTestTzId::NewL(tzBuffer);
	CleanupStack::PushL(tzId);
	iTz.SetTimeZoneL(*tzId);
	CleanupStack::PopAndDestroy(tzId);
	}

void CTestSession::DoGetTimeZoneL(const RMessage2& aMessage)
	{
	CTestTzId* currentId = iTz.GetTimeZoneIdL();
	CleanupStack::PushL(currentId);
	
	aMessage.WriteL(0, currentId->TimeZoneNameID());
	
	CleanupStack::PopAndDestroy(currentId);
	}

void CTestSession::DoDeleteDirL(const RMessage2& aMessage)
	{
	TInt count = 0;
	TInt err;
	ReadMessageParamL(aMessage);
	TPtr dirName=iParamBuf->Des();
	FOREVER
		{
		err = GetRFs().RmDir(dirName);
		if ((err != KErrInUse) || (count == nsPIMTestServer::maxTry))
			break;
		++count;
		User::After(500000);		//0.5secs
		}
	// Ignore not-found error
	if (!(err==KErrNone || err==KErrNotFound || err==KErrPathNotFound))
		User::Leave(err);
	}

void CTestSession::DoDeleteAllFilesInDirL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);

	TPtr dirName=iParamBuf->Des();
		
	CDir* dirList;
	TInt err = GetRFs().GetDir(dirName, KEntryAttNormal , ESortNone, dirList);
	TInt numOfFiles = dirList->Count();
	
	if(numOfFiles > 0)
		{
		//	temp storage for traversing file names
		HBufC* buffer = HBufC::NewLC(KMaxFileName);
		TPtr fileName = buffer->Des();

		for(TInt i=0; i < numOfFiles; ++i)
			{
			fileName.Copy(dirName);
			fileName.Append((*dirList)[i].iName);
			
			// remove read-only attributes to files
			TInt count = 0;
			TInt err = GetRFs().SetAtt(fileName, KEntryAttNormal,	KEntryAttReadOnly);	

			// ASync calls to the server can result in this delete method being 
			// called while a handle to the file still exists.
			while ((err == KErrInUse) && (count < nsPIMTestServer::maxTry))
				{
				User::After(500000);
				err = GetRFs().SetAtt(fileName, KEntryAttNormal, KEntryAttReadOnly);
				++count;
				}
			err = GetRFs().Delete(fileName);
			
			User::LeaveIfError(err);
			}
		}
	CleanupStack::PopAndDestroy(); //buffer
	}

void CTestSession::DoGetFileSizeL(const RMessage2& aMessage)
	{
	ReadMessageParamL(aMessage);
	TPtr fileName=iParamBuf->Des();
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.Open(GetRFs(), *iParamBuf, EFileShareAny|EFileRead));
	TInt size = 0;
	User::LeaveIfError(file.Size(size));

	TPckg<TInt> theSize(size);
	aMessage.WriteL(1, theSize);
	CleanupStack::PopAndDestroy(&file);
	}

void CTestSession::DoNotifyChangeL(const RMessage2& aMessage)
	{
	if (!iNotifyChange)
		{
		iNotifyChange = CNotifyChange::NewL(Server().getRFs());
		}
	
	TFileName path;
	TNotifyType type = static_cast<TNotifyType>(aMessage.Int0());
	aMessage.ReadL(1, path);
	iNotifyChange->Start(type, path, aMessage);
	}

void CTestSession::DoNotifyChangeCancel()
	{
	if (iNotifyChange)
		{
		iNotifyChange->Cancel();
		}
	}

void CTestSession::DoChangeFSAttributesL(const RMessage2& aMessage)
	{
	const TUint setAttMask = aMessage.Int0();
	const TUint clearAttMask = aMessage.Int1();
	ReadMessageParamL(aMessage, 2);
	User::LeaveIfError(GetRFs().SetAtt(*iParamBuf, setAttMask, clearAttMask));
	}

void CTestSession::PanicClient(const RMessage2& aMessage,TTestPanic aPanic)const
// RMessage::Panic() also completes the message. This is:
// (a) important for efficient cleanup within the kernel
// (b) a problem if the message is completed a second time
	{
	aMessage.Panic(nsPIMTestServer::KPanic,aPanic);
	}

// ServiceL left so we end up here...
void CTestSession::ServiceError(const RMessage2 &aMessage, TInt aError)
	{
	if(aError == KErrBadDescriptor)
		PanicClient(aMessage, EPanicBadDescriptor);
	else
		CSession2::ServiceError(aMessage, aError); // will complete the message with aError
	}

// ----------------------------------
// CNotifyChange Class implementation
// ==================================
CNotifyChange* CNotifyChange::NewL(RFs& aFs)
	{
	CNotifyChange* self = new(ELeave) CNotifyChange(aFs);
	return self;
	}

CNotifyChange::CNotifyChange(RFs& aFs)
:CActive(EPriorityStandard), iFs(aFs)
	{
	CActiveScheduler::Add(this);
	}

void CNotifyChange::Start(TNotifyType type, const TDesC& path, const RMessage2& aMessagePtr)
	{
	Cancel();

	iMessagePtr = aMessagePtr;
	iFs.NotifyChange(type, iStatus, path);
	SetActive();
	}
void CNotifyChange::RunL()
	{
	// Complete the asyncronous server call
	// with the result of file server NotifyChange call 
	iMessagePtr.Complete(iStatus.Int());
	}

void CNotifyChange::DoCancel()
	{
	iFs.NotifyChangeCancel();
	
	// Complete the asyncronous server call
	// with KErrCancel
	iMessagePtr.Complete(KErrCancel);
	}


// ----------------------------------
// CTestServer Class implementation
// ==================================

// Standard Constructors are declared inline.

CServer2* CTestServer::NewLC()
	{
	CTestServer* self = new(ELeave) CTestServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CTestServer::ConstructL()
// 2nd phase construction - ensure the timer and server objects are running
	{
	
	User::LeaveIfError(iFs.Connect());
	
	StartL(KTestServerName);
	iShutdown.ConstructL();
	// ensure that the server still exits even if the 1st client fails to connect
	iShutdown.Start();
	}

CTestServer::~CTestServer()
	{
	iFs.Close();		
	}
	




CSession2* CTestServer::NewSessionL(const TVersion&,const RMessage2&) const
// Create a new client session. This should really check the version number.
	{
	return new (ELeave) CTestSession();
	}


void CTestServer::AddSession()
// A new session is being created
// Cancel the shutdown timer if it was running
	{
	++iSessionCount;
	iShutdown.Cancel();
	}

void CTestServer::DropSession()
// A session is being destroyed
// Start the shutdown timer if it is the last session.
	{
	if (--iSessionCount==0)
		iShutdown.Start();
	}

void CShutdown::RunL()
// Initiate server exit when the timer expires
	{
	CActiveScheduler::Stop();
	}





/**
Perform all server initialisation, in particular creation of the
scheduler and server and then run the scheduler
*/

static void RunServerL()
	{
	
	// naming the server thread after the server helps to debug panics
  	User::LeaveIfError(RThread::RenameMe(KTestServerName));
	// create and install the active scheduler we need
	CActiveScheduler* s = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(s);
	CActiveScheduler::Install(s);
	CTestServer::NewLC(); 
	
	RProcess::Rendezvous(KErrNone);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(2); 	// Cleanup the server and scheduler

	}




TInt E32Main()
// Server process entry-point
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New();
	TInt r=KErrNoMemory;
	if (cleanup)
		{
		TRAP(r,RunServerL());
		delete cleanup;
		}

	__UHEAP_MARKEND;
	return r;
	}


