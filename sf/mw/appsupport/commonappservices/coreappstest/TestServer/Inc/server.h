// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TestServer - server classes
// 
//

#include <e32base.h>
#include <f32file.h>

#include "clientserver.h"
#include "testtz.h"


// Forward Declarations
enum TTestPanic
	{
	EPanicBadDescriptor,
	EPanicIllegalFunction,
	EPanicAlreadyReceiving
	};







// CShutdown class
class CShutdown : public CTimer
	{ 
	
	enum {
			KMyShutdownDelay = 0x500000
		 };	// approx 5s

public:
	inline CShutdown();
	inline void ConstructL();
	inline void Start();

private:
	void RunL();

	};




// TestServer Class
class CTestServer : public CServer2
	{

public:
	static CServer2* NewLC();
	void AddSession ();
	void DropSession();
	void Send(const TDesC& aMessage);
	virtual ~CTestServer();
	inline RFs& getRFs();
	
private:
	CTestServer ();
	void ConstructL ();
    CSession2* NewSessionL (const TVersion& aVersion, const RMessage2& aMessage) const;

private:
	TInt iSessionCount;
	CShutdown iShutdown;
	RFs iFs; 
	};


class CNotifyChange : public CActive
	{
public:
	static CNotifyChange* NewL(RFs& aFs);
	void Start(TNotifyType type, const TDesC& path, const RMessage2& aMessagePtr);
	
private:
	void RunL();
	void DoCancel();
	CNotifyChange(RFs& aFs);
	
private:
	RFs& iFs;
	RMessage2 iMessagePtr;
	};


// TestSession Class
class CTestSession : public CSession2
	{

public:
	CTestSession();
	void CreateL();
	void Send(const TDesC& aMessage);

private:
	~CTestSession();
	inline CTestServer& Server();
	void ServiceL(const RMessage2& aMessage);
	void ServiceError(const RMessage2 &aMessage, TInt aError);
	void PanicClient   (const RMessage2& aMessage,TTestPanic TMyPanic) const;


	void DoSetHomeTimeL  (const RMessage2& aMessage);
	// General File Manipulation
	void DoDeleteFileL  (const RMessage2& aMessage);

	// Power User
	void DoKillProcessL (const RMessage2& aMessage);
	void DoChangeLocaleL(const RMessage2& aMessage);
	void DoCheckForFileL (const RMessage2& aMessage);
	void DoCopyFileL    (const RMessage2& aMessage);
	void DoCreateEmptyFileL(const RMessage2& aMessage);
	void DoSetTimeZoneL	(const RMessage2& aMessage);
	void DoGetTimeZoneL	(const RMessage2& aMessage);
	void DoDeleteDirL	(const RMessage2& aMessage);
	void DoGetFileSizeL	(const RMessage2& aMessage);
	void DoNotifyChangeL (const RMessage2& aMessage);
	void DoNotifyChangeCancel ();
	void DoChangeFSAttributesL(const RMessage2& aMessage);
	void DoDeleteAllFilesInDirL(const RMessage2& aMessage);
	void DoCopyFileL(const RMessage2& aMessage, TUint aTAction);
	void DoCopyDirAndFilesL(const RMessage2& aMessage);
	
	//Helper Functions
	void ReadMessageParamL(const RMessage2& aMessage, TInt aParamNo=0) const;
	RFs& GetRFs();

private:
	RMessage2 	   iReceive;
	mutable HBufC* iParamBuf;// large frequently used heap-buffer
	RTestTz			   iTz;
	
	CNotifyChange* iNotifyChange;

	};





// CShutDown inline methods
inline CShutdown::CShutdown():CTimer(CActive::EPriorityIdle)
	{
	CActiveScheduler::Add(this);

	}
	
inline void CShutdown::ConstructL()
	{
	CTimer::ConstructL();

	}
	
inline void CShutdown::Start()
	{
	After (KMyShutdownDelay);
	}


// CTestServer inline methods	
inline CTestServer::CTestServer():CServer2(0, ESharableSessions)
	{
	}
	
inline RFs& CTestServer::getRFs()
	{
	return iFs;
	}


	
// CTestSession inline methods		
inline CTestSession::CTestSession():iParamBuf(0)
	{}
	
inline CTestServer& CTestSession::Server()
	{
	return *static_cast<CTestServer*>(const_cast<CServer2*>(CSession2::Server()));
	}
	
inline RFs& CTestSession::GetRFs()
	{
	return Server().getRFs();
	}
