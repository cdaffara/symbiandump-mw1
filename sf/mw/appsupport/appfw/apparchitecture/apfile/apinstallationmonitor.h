// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// apinstallationmonitor.h
// 

#ifndef __APINSTALLATIONMONITOR_H__
#define __APINSTALLATIONMONITOR_H__

#include <e32base.h>
#include <e32property.h>
#include <swi/swispubsubdefs.h>
#include "../apserv/apsserv.h"


class CApaAppArcServer;

NONSHARABLE_CLASS(CApaAppInstallationMonitor) : public CActive
/** 
CApaAppInstallationMonitor

A low priority active object which monitors software
install for installations/uninstallations of applications.

@internalComponent
*/
	{
public:
	IMPORT_C static CApaAppInstallationMonitor* NewL(CApaAppArcServer*);
	IMPORT_C ~CApaAppInstallationMonitor();
	IMPORT_C void Start();
private:
	CApaAppInstallationMonitor(CApaAppArcServer*);
	void ConstructL();
	void RunL();
	void RunError();
	void DoStart();
	void DoCancel();
	TBool InstallationInProgress() const;
private:
	CApaAppArcServer* iApaServer;
	RProperty iSwisProperty;
	TInt iSwisState;
	TBool iWaitingForStartInstallation;
	};
	
#endif
