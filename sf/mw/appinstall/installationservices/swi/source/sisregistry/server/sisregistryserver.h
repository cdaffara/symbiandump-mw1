/*
* Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
* CSisRegistryServer classe definition
*
*/


/**
 @file 
 @released
 @internalComponent
*/
#ifndef __SISREGISTRYSERVER_H__
#define __SISREGISTRYSERVER_H__

#include <e32base.h>
#include "sisregistryclientserver.h"
#include "sisregistryservershutdown.h"

namespace Swi
{

_LIT_SECURE_ID(KDaemonSecureId, 0x10202DCE);
_LIT_SECURE_ID(KSwisSecureId, 0x101F7295);	
_LIT_SECURE_ID(KSifServerSecureId, 0x10285BCB);	

const TInt KPolicyRanges= 13;
const TInt KPolicyElements = 7;

class CSisRegistrySession;

enum TSisRegistryPanic
	{
	EPanicBadDescriptor,
	EPanicIllegalFunction,
	EPanicAlreadyReceiving,
	EPanicBadOperationMode
	};

class CSisRegistryServer : public CPolicyServer
	{
public:
	static CSisRegistryServer* NewLC(CActive::TPriority = CActive::EPriorityStandard);
	void AddSession();
	void DropSession();
	
private:
	CSisRegistryServer(CActive::TPriority aPriority);
	void ConstructL();
	~CSisRegistryServer();
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;

private:
	TInt iSessionCount;
	CShutdown* iShutdown;

private:
	// Server Policies
  	static const TUint iRangeCount = KPolicyRanges;
  	static const TInt iRanges[KPolicyRanges];
  	static const TUint8 iElementsIndex[KPolicyRanges];
  	static const CPolicyServer::TPolicyElement iPolicyElements[KPolicyElements];
  	static const CPolicyServer::TPolicy iPolicy;
	};

} //namespace

#endif // __SISREGISTRYSERVER_H__
