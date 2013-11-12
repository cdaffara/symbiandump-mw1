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

#ifndef __TEALARMTESTSERVER_H__
#define __TEALARMTESTSERVER_H__

#include <test/testexecuteserverbase.h>

class CTEAlarmTestServer : public CTestServer
	{
public:
	static CTEAlarmTestServer* NewL(); // First phase constructor
	// Base class pure virtual
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
private:
	};

#endif // __TEALARMTESTSERVER_H__
