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
//

#include <e32base.h>
#include <e32cons.h>
#include <hal.h>

// Epoc Includes
#include <swi/launcher.h>

#include "cswiappinstall.h"
#include "cswitestsecurityinstuihandler.h"
#include <swi/sisregistrypackage.h>
#include <swi/sisregistrysession.h>
#include <swi/sisregistryentry.h>
#include <mm/mmcleanup.h>


CSwiAppInstall::CSwiAppInstall()
	{
	}

void CSwiAppInstall::InstallL(const TDesC& aFileName)
	{
	TFileName fileName(aFileName);
	fileName.PtrZ();
	Swi::CInstallPrefs* installPrefs = Swi::CInstallPrefs::NewL();
	CleanupStack::PushL(installPrefs);
	CSwiTestSecurityInstUIHandler* uiHandler = CSwiTestSecurityInstUIHandler::NewL();
	CleanupStack::PushL(uiHandler);
	User::LeaveIfError(Swi::Launcher::Install(*uiHandler,fileName,*installPrefs));
	CleanupStack::PopAndDestroy(uiHandler);
	CleanupStack::PopAndDestroy(installPrefs);
	}

void CSwiAppInstall::UninstallL(const TDesC& aUid)
	{
	TLex conv(aUid);
	TUint32 id;
	User::LeaveIfError(conv.Val(id,EHex));
	TUid uid = TUid::Uid(id);
	CSwiTestSecurityInstUIHandler* uiHandler = CSwiTestSecurityInstUIHandler::NewL();
	CleanupStack::PushL(uiHandler);
	User::LeaveIfError(Swi::Launcher::Uninstall(*uiHandler, uid));
	CleanupStack::PopAndDestroy(uiHandler);
	}

void CSwiAppInstall::UninstallL(const TDesC& aUid, const TDesC& aPackageName, const TDesC& aVendorName)
	{
	TLex conv(aUid);
	TUint32 id;
	User::LeaveIfError(conv.Val(id,EHex));

	TUid uid = TUid::Uid(id);
	CSwiTestSecurityInstUIHandler* uiHandler = CSwiTestSecurityInstUIHandler::NewL();
	CleanupStack::PushL(uiHandler);

	Swi::CSisRegistryPackage* uninstallPackage =  Swi::CSisRegistryPackage::NewL(uid, aPackageName, aVendorName);
	CleanupStack::PushL(uninstallPackage);

	Swi::RSisRegistrySession registrySession;
	User::LeaveIfError(registrySession.Connect());
	CleanupClosePushL(registrySession);

	Swi::RSisRegistryEntry registryEntry;

	User::LeaveIfError(registryEntry.Open(registrySession, uid));
	CleanupClosePushL(registryEntry);

	Swi::CSisRegistryPackage* package=registryEntry.PackageL();
	CleanupStack::PushL(package);

	if (*package == *uninstallPackage)
		{
		User::LeaveIfError(Swi::Launcher::Uninstall(*uiHandler, *package));
		}
	else
		{
		// check augmenations
		RPointerArray<Swi::CSisRegistryPackage> augmentationPackages;
		CleanupResetAndDestroy<RPointerArray<Swi::CSisRegistryPackage> >::PushL(augmentationPackages);

		registryEntry.AugmentationsL(augmentationPackages);
		for (TInt i=0; i < augmentationPackages.Count(); ++i)
			{
			if (*augmentationPackages[i] == *uninstallPackage)
				{
				User::LeaveIfError(Swi::Launcher::Uninstall(*uiHandler, *augmentationPackages[i]));
				break;
				}
			}
		CleanupStack::PopAndDestroy(&augmentationPackages);
		augmentationPackages.Close();
		}

	CleanupStack::PopAndDestroy(3, &registrySession);
	CleanupStack::PopAndDestroy(uninstallPackage);
	CleanupStack::PopAndDestroy(uiHandler);
	}
