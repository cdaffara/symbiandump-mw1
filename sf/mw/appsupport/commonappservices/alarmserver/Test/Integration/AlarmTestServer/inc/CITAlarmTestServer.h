/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __CITALARMTESTSERVER_H__
#define __CITALARMTESTSERVER_H__

#include <test/testexecuteserverbase.h>

class CCITAlarmTestServer : public CTestServer
	{
public:
	static CCITAlarmTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
    inline RFs& Fs();

private:
	RFs iFs;
	};
	

inline RFs& CCITAlarmTestServer::Fs()
    {
    return iFs;
    }

#endif
