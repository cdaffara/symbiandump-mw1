/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Defines the test server of the Software Component Registry test harness
*
*/


/**
 @file 
 @internalComponent
 @test
*/

#ifndef TSCRSERVER_H
#define TSCRSERVER_H

#include <test/testexecuteserverbase.h>

_LIT(KScrTestServerName, "tscr");

class CScrTestServer : public CTestServer
/**
	Describes the test server for the SCR test harness.
 */
	{
public:
	static CScrTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	RFs& Fs();
	~CScrTestServer();
private:
	TBool IsPerformanceTestStep(const TDesC& aStepName, TPtrC& aStrippedName);
private:
	RFs iFs;
	};

inline RFs& CScrTestServer::Fs()
	{
	return iFs;
	}

#endif /* TSCRSERVER_H */
