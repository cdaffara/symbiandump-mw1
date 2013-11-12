// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASTSTALARMTESTSTD_H__
#define __ASTSTALARMTESTSTD_H__

// System includes
#include <e32test.h>

// User includes
#include <asshdalarm.h>
#include <asclisession.h>
#include <consolealarmalertservermain.h>

// Literal constants
_LIT(KAlarmTestCodeTitle, "AlarmServerTestCode");

// Constants
const TInt KTimeToWait = 1000000;

class ASTstAlarmTest
	{
public:
	/**
	 *
	 */
	IMPORT_C ASTstAlarmTest();

	/**
	 *
	 */
	IMPORT_C static void Close();

	/**
	 *
	 */
	IMPORT_C static ASTstAlarmTest& Self();

public:
	/**
	 *
	 */
	IMPORT_C void	operator()(TInt aResult, TInt aLineNum, const TText* aFileName);
	
	/**
	 *
	 */
	IMPORT_C void	operator()(TInt aResult, TInt aLineNum);

	/**
	 *
	 */
	IMPORT_C void	operator()(TInt aResult);

	/**
	 *
	 */
	IMPORT_C void	Title();

	/**
	 *
	 */
	IMPORT_C void	Start(const TDesC& aHeading);

	/**
	 *
	 */
	IMPORT_C void	Next(const TDesC& aHeading);

	/**
	 *
	 */
	IMPORT_C void	EndL();

	/**
	 *
	 */
	IMPORT_C void	UpLevel();

public:
	/**
	 *
	 */
	IMPORT_C void	TestClearStoreL();

	/**
	 *
	 */
	IMPORT_C void	WaitForNotificationBufferToBeEmptied(TRequestStatus& aStatus, TAlarmId& aAlarmId);

	/**
	 *
	 */
	IMPORT_C void	WaitForEvent(TInt aEvent, TRequestStatus& aStatus, TAlarmId& aAlarmId);

	/**
	 *
	 */
	IMPORT_C void	TestEventBuffer(TInt aExpected, TRequestStatus& aStatus, TAlarmId& aAlarmId);

	/**
	 *
	 */
	IMPORT_C TInt	CountOrphanedAlarmsL();

	/**
	 *
	 */
	IMPORT_C void	TestStartServers(TAny * instructionSet = NULL);

public:

	/**
	 *
	 */
	inline RTest& Test() { return iTest; }

	/**
	 *
	 */
	inline RASCliSession& Session() { return iAlarmServer; }

private:
	void resetHomeTimeL();

	/**
	 *
	 */
	RTest			iTest;

	/**
	 *
	 */
	RASCliSession	iAlarmServer;
	TTime			iInitialHomeTime;
	};

// Defines
#define TheAlarmTest ASTstAlarmTest::Self()
#define AlarmTest(_OUTCOME) ASTstAlarmTest::Self()(_OUTCOME,__LINE__)

#endif
