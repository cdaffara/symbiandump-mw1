// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined __CTZLOCALIZATIONTEST_SERVER_H__)
#define __CTZLOCALIZATIONTEST_SERVER_H__

#include <test/testexecuteserverbase.h>
#include "TzLocalizer.h"

class CTzLocalizationTestServer : public CTestServer
	{
public:
	static CTzLocalizationTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	RFs& Fs(){return iFs;};
	~CTzLocalizationTestServer();

private:
	void ConstructL(const TDesC& aName);

private:
	RFs iFs;
	};

#endif //__CTZLOCALIZATIONTEST_SERVER_H__
