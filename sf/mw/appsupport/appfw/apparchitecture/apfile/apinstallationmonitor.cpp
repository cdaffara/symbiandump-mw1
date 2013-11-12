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
// apinstallationmonitor.cpp
//

#include "apinstallationmonitor.h"

// This active object has lower priority than CApaAppArcServer but higher than file scanning periodic timer.
const TInt KAppInstallationPriority=CActive::EPriorityStandard-10; 

//
// CApaAppInstallationMonitor class
//
EXPORT_C CApaAppInstallationMonitor* CApaAppInstallationMonitor::NewL(CApaAppArcServer* aApaServer)
	{
	CApaAppInstallationMonitor* self = new(ELeave) CApaAppInstallationMonitor(aApaServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CApaAppInstallationMonitor::CApaAppInstallationMonitor(CApaAppArcServer* aApaServer) :
		CActive(KAppInstallationPriority),
		iApaServer(aApaServer),
		iWaitingForStartInstallation(ETrue)
	{
	CActiveScheduler::Add(this);
	}

void CApaAppInstallationMonitor::ConstructL()
	{
	User::LeaveIfError(iSwisProperty.Attach(KUidSystemCategory, Swi::KUidSoftwareInstallKey));
	}

EXPORT_C CApaAppInstallationMonitor::~CApaAppInstallationMonitor()
	{
	Cancel();
	iSwisProperty.Close();
	}

EXPORT_C void CApaAppInstallationMonitor::Start()
	{
	DoStart();
	}

TBool CApaAppInstallationMonitor::InstallationInProgress() const
	{
	return (((iSwisState & Swi::KSwisOperationMask) & Swi::ESwisUninstall)||((iSwisState & Swi::KSwisOperationMask) & Swi::ESwisInstall))
			&& ((iSwisState & Swi::KSwisOperationStatusMask) == Swi::ESwisStatusNone);
	}
	
void CApaAppInstallationMonitor::RunL()
	{
	if(iStatus.Int() == KErrNone)
		{
		DoStart();
		
		if (iWaitingForStartInstallation)
			{
			if (InstallationInProgress())
				{
				iWaitingForStartInstallation = EFalse;
				iApaServer->HandleInstallationStartEvent();
				}
			}
		
		else 
			{ // waiting for end Installation
			if (!InstallationInProgress())
				{
				iWaitingForStartInstallation = ETrue;
				TRAP_IGNORE(iApaServer->HandleInstallationEndEventL()); // let user try again if OOM
				}
			}
		}
	}
	
void CApaAppInstallationMonitor::DoStart()
	{
	iSwisProperty.Subscribe(iStatus);
	TInt err = iSwisProperty.Get(KUidSystemCategory, Swi::KUidSoftwareInstallKey, iSwisState);
	if(err == KErrNone)
		{
		SetActive();
		}
	else
		{
		iSwisProperty.Cancel();
		}
	}

void CApaAppInstallationMonitor::DoCancel()
	{
	iSwisProperty.Cancel();
	}
	

// End of file
