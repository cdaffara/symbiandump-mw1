/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TEST_BACKUP_RESTORE_NOTIFICATION__SERVER__SUITE_H__
#define __TEST_BACKUP_RESTORE_NOTIFICATION__SERVER__SUITE_H__

#include <test/testexecuteserverbase.h>

_LIT(KServerName,"TestBackupRestoreNotificationServerSuite");

class CTestBackupRestoreNotificationServerSuite : public CTestServer
	{
public:
	static CTestBackupRestoreNotificationServerSuite* NewL();
	// Base class pure virtual override
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

#endif
