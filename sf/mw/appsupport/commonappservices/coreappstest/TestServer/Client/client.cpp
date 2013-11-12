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
// Test Server - client interface implementation
// 
//

#include <e32std.h>
#include <e32math.h>
#include "clientserver.h"
#include "testserver.h"


const TUid KServerUid3={0x101F8225};

static TInt StartServer()
	{
	const TUidType serverUid(KNullUid,KNullUid,KServerUid3);


	RProcess server;
	TInt r=server.Create(KTestServerImg, KNullDesC, serverUid);

	if (r!=KErrNone)
		return r;
	TRequestStatus stat;

	server.Rendezvous(stat);
	
	if (stat != KRequestPending)
		server.Kill(0);		// abort startup
	else
		server.Resume();	// logon OK - start the server	
	User::WaitForRequest(stat);	//wait for start or death

	r=(server.ExitType()==EExitPanic) ? KErrGeneral : stat.Int();
	server.Close();
	return r;
	}

EXPORT_C TInt RPIMTestServer::Connect()
	{
	TInt retry=2;
	for (;;)
		{
		TInt r=CreateSession(KTestServerName,TVersion(0,0,0),2);
		if (r!=KErrNotFound && r!=KErrServerTerminated)
			return r;
		if (--retry==0)
			return r;
		r=StartServer();
		if (r!=KErrNone && r!=KErrAlreadyExists)
			return r;
		User::After(1000000); //Let server load up (in EKA2)

		}
	}

/* 
	@param aTime is in Local Time format 
*/
EXPORT_C TInt RPIMTestServer::SetHomeTime(const TTime& aTime) 
	{
	TPckg<TTime> pckg(aTime);
	return SendReceive(ETestIpcSetHomeTime,TIpcArgs(&pckg));
	}

EXPORT_C void RPIMTestServer::DeleteFileL(const TDesC& aFileName)
	{
	TIpcArgs args(&aFileName);
	User::LeaveIfError(SendReceive(ETestIpcDeleteFile,args));
	}

EXPORT_C void RPIMTestServer::KillProcessL(const TDesC& aProcessName)
	{
	TIpcArgs args(&aProcessName);
	User::LeaveIfError(SendReceive(ETestIpcKillProcess,args));
	}

EXPORT_C void RPIMTestServer::ChangeLocaleNameL(const TDesC& aLocaleDllName)
	{
	TIpcArgs args(&aLocaleDllName);
	User::LeaveIfError(SendReceive(ETestIpcChangeLocale,args));
	}

EXPORT_C TInt RPIMTestServer::CheckForFile(const TDesC& aFileName)
    {
	TIpcArgs args(&aFileName); 
	return SendReceive(ETestIpcCheckForFile,args);  
    }
    
EXPORT_C void RPIMTestServer::CopyFileL(const TDesC& aSource, const TDesC& aDest)
    {
	TIpcArgs args(&aSource, &aDest); 
	User::LeaveIfError(SendReceive(ETestIpcCopyFile,args));  
    }
    
EXPORT_C void RPIMTestServer::CreateEmptyFileL(const TDesC& aFileName)
    {
	TIpcArgs args(&aFileName);
	User::LeaveIfError(SendReceive(ETestIpcCreateEmptyFile,args));  
    }
    
EXPORT_C void RPIMTestServer::SetTimeZoneL(const TDesC8& aTimeZoneName)
    {
	TIpcArgs args(&aTimeZoneName);
	User::LeaveIfError(SendReceive(ETestIpcSetTimeZone,args));  
    }
    
EXPORT_C void RPIMTestServer::GetTimeZoneL(TDes8& aTimeZoneName)
    {
	TIpcArgs args(&aTimeZoneName);
	User::LeaveIfError(SendReceive(ETestIpcGetTimeZone,args));  
    }
    
EXPORT_C void RPIMTestServer::DeleteDirL(const TDesC& aFileName)
	{
	TIpcArgs args(&aFileName);
	User::LeaveIfError(SendReceive(ETestIpcDeleteDir,args));
	}

EXPORT_C TInt RPIMTestServer::GetFileSizeL(const TDesC& aFileName)
	{
	TPckg<TInt> size(0);
	TIpcArgs args(&aFileName, &size);
	User::LeaveIfError(SendReceive(ETestIpcGetFileSize,args));
	return size();
	}
EXPORT_C void RPIMTestServer::FileServerNotifyChange(TNotifyType aType, TRequestStatus &aStat, const TDesC &aPathName)
	{
	TIpcArgs args(aType, &aPathName);
	SendReceive(ETestIpcNotifyChange,args,aStat);
	}

EXPORT_C void RPIMTestServer::FileServerNotifyChangeCancel()
	{
	SendReceive(ETestIpcNotifyChangeCancel);
	}
 
EXPORT_C void RPIMTestServer::CloseTzSession()
	{
	SendReceive(ETestIpcCloseTzSession);
	}

EXPORT_C void RPIMTestServer::ChangeFSAttributesL(const TDesC& aPath, TUint aSetAttMask, TUint aClearAttMask)
	{
	TIpcArgs args(aSetAttMask, aClearAttMask, &aPath);
	User::LeaveIfError(SendReceive(ETestIpcChangeFSAttributes, args));
	}

EXPORT_C void RPIMTestServer::DeleteFilesInDirL(const TDesC& aDirName)
	{
	TIpcArgs args(&aDirName);
	User::LeaveIfError(SendReceive(ETestIpcDeleteFilesInDir,args));
	}

EXPORT_C void RPIMTestServer::CopyDirAndFilesL(const TDesC& aSource, const TDesC& aDest)
    {
    TIpcArgs args(&aSource, &aDest); 
     User::LeaveIfError(SendReceive(ETestIpcCopyDirAndFiles,args));  
     }


