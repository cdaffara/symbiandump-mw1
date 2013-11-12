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




#include <e32base.h>
#include "clientserver.h"
#include "callocmanager.h"

enum TMyPanic
	{
	EPanicBadDescriptor,
	EPanicIllegalFunction,
	EPanicAlreadyReceiving
	};

void PanicClient(const RMessagePtr2& aMessage,TMyPanic TMyPanic);

class CShutdown : public CTimer
	{
	enum {KMyShutdownDelay=0x200000};	// approx 2s
public:
	inline CShutdown();
	inline void ConstructL();
	inline void Start();
private:
	void RunL();
	};

class CMyServer : public CServer2
	{
public:
	static CServer2* NewLC();
	void AddSession();
	void DropSession();
	void Send(const TDesC& aMessage);
	CAllocManager& AllocManager();
	~CMyServer();
private:
	CMyServer();
	void ConstructL();
	void DefinePropertiesL();
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
private:
	TInt iSessionCount;
	CShutdown iShutdown;
	CAllocManager *iAllocManager;
	};

class CMySession : public CSession2
	{
public:
	CMySession();
	void CreateL();
	void Send(const TDesC& aMessage);
private:
	~CMySession();
	inline CMyServer& Server();
	void ServiceL(const RMessage2& aMessage);
	void ServiceError(const RMessage2& aMessage,TInt aError);
	inline TBool ReceivePending() const;
private:
	RMessagePtr2 iReceiveMsg;
	TInt iReceiveLen;
	};

