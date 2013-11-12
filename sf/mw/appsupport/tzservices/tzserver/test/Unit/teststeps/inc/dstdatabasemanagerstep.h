/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __DSTDATABASEMANAGER_H__
#define __DSTDATABASEMANAGER_H__

#include "DstUnitStep.h"
#include "mtimezonedataaccess.h"
#include "tzswiobserver.h"
#include "ReadOnlyTzDb.h"

// String for the server create test step code
_LIT(KDstDatabaseManagerStep, "DstDatabaseManager");

class CDstDatabaseManagerStep : public CDstUnitStep, public MTzDataObserver
	{
public:
	CDstDatabaseManagerStep(CDstUnitServer &aServer);
	~CDstDatabaseManagerStep();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	TBool DoDefaultZoneCheckL();
	TBool DoTimeAlignmentChecksL();
	TBool DoBackupRegistrationFileCheckL();

	TInt GetStartYear() const;

private:
	// from MTzDataObserver
	void NotifyTZDataStatusChangeL(RTz::TTzChanges /*aChange*/) { /* ignore - do nothing */}

private:
	CReadOnlyTzDb* iReadOnlyTzDb;
	RFs iFs;
	TInt iStartAddress;
	CTzSwiObserver* iTzSwiObserver;
	};

#endif // __DSTDATABASEMANAGER_H__
