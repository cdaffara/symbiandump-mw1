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

#include "ctzswiplugintester.h"
#include "cswiappinstall.h"
#include "testserver.h"
#include <tzupdate.h>

CCTzSwiPluginTester::CCTzSwiPluginTester() :
	CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

EXPORT_C CCTzSwiPluginTester* CCTzSwiPluginTester::NewLC()
	{
	CCTzSwiPluginTester* self = new ( ELeave ) CCTzSwiPluginTester();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CCTzSwiPluginTester* CCTzSwiPluginTester::NewL()
	{
	CCTzSwiPluginTester* self = CCTzSwiPluginTester::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CCTzSwiPluginTester::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	//Add the log file, this enables us to figure out when the sis install is complete
	iFs.MkDirAll(KSwiTzLogDir());
	iFs.Delete(KSwiTzLogFile());
	User::LeaveIfError(iPropertyDb.Attach(NTzUpdate::KPropertyCategory, NTzUpdate::ETzRulesChange));
	User::LeaveIfError(iPropertyLoc.Attach(NTzUpdate::KPropertyCategory, NTzUpdate::ETzNamesChange));
	}

EXPORT_C CCTzSwiPluginTester::~CCTzSwiPluginTester()
	{
	Cancel();
	iFs.Close();
	iPropertyDb.Close();
	iPropertyLoc.Close();
	}

void CCTzSwiPluginTester::DoCancel()
	{
	switch (iNotifyType)
		{
		case EPropertyRules:
			iPropertyDb.Cancel();
			break;
		case EPropertyNames:
			iPropertyLoc.Cancel();
			break;
		case EFileLog:
			iFs.NotifyChangeCancel(*iReqStatus);
			break;
		}
	}

void CCTzSwiPluginTester::RunL()
	{
//	Cancel(); //Check that this does not cause any problems!!
	User::RequestComplete(iReqStatus, KErrNone);
	CActiveScheduler::Stop();
	}

TInt CCTzSwiPluginTester::RunError(TInt aError)
	{
	Cancel();
	User::RequestComplete(iReqStatus, aError);
	CActiveScheduler::Stop();
	return aError;
	}

EXPORT_C void CCTzSwiPluginTester::InstallSIS(const TDesC& aSisFile, TUint aNotifyType, TRequestStatus& aReqStatus)
	{
	iNotifyType = static_cast<CCTzSwiPluginTester::TNotifyType> (aNotifyType);
	switch (iNotifyType)
		{
		case EPropertyRules:
			{
			if (!IsActive())
				{
				aReqStatus = KRequestPending;
				iReqStatus = &aReqStatus;
				iPropertyDb.Subscribe(iStatus);
				SetActive();
				}
			}
			break;
		case EPropertyNames:
			{
			if (!IsActive())
				{
				aReqStatus = KRequestPending;
				iReqStatus = &aReqStatus;
				iPropertyLoc.Subscribe(iStatus);
				SetActive();
				}
			}
			break;
		case EFileLog:
			{
			if (!IsActive())
				{
				aReqStatus = KRequestPending;
				iReqStatus = &aReqStatus;
				iFs.NotifyChange(ENotifyAll, iStatus, KSwiTzLogDir());
				SetActive();
				}
			}
			break;
		}
	TRAPD(err, CSwiAppInstall::InstallL(aSisFile));
	if (err != KErrNone)
		{
		Cancel();
		}
	}

EXPORT_C void CCTzSwiPluginTester::UninstallSIS(const TDesC& aApplicationUID, const TDesC& aPackageName, const TDesC& aVendorId, TUint aNotifyType, TRequestStatus& aReqStatus)
	{
	iNotifyType = static_cast<CCTzSwiPluginTester::TNotifyType> (aNotifyType);
	switch (iNotifyType)
		{
		case EPropertyRules:
			{
			if (!IsActive())
				{
				aReqStatus = KRequestPending;
				iReqStatus = &aReqStatus;
				iPropertyDb.Subscribe(iStatus);
				SetActive();
				}
			}
			break;
		case EPropertyNames:
			{
			if (!IsActive())
				{
				aReqStatus = KRequestPending;
				iReqStatus = &aReqStatus;
				iPropertyLoc.Subscribe(iStatus);
				SetActive();
				}
			}
			break;
		case EFileLog:
			{
			if (!IsActive())
				{
				aReqStatus = KRequestPending;
				iReqStatus = &aReqStatus;
				iFs.NotifyChange(ENotifyAll, iStatus, KSwiTzLogDir());
				SetActive();
				}
			}
			break;
		}
	TRAPD(err, CSwiAppInstall::UninstallL(aApplicationUID, aPackageName, aVendorId));
	if (err != KErrNone)
		{
		Cancel();
		}
	}

EXPORT_C TInt CCTzSwiPluginTester::UninstallSIS(const TDesC& aApplicationUID, const TDesC& aPackageName, const TDesC& aVendorId)
	{
	TRAPD(err, CSwiAppInstall::UninstallL(aApplicationUID, aPackageName, aVendorId));
	return err;
	}

EXPORT_C TInt CCTzSwiPluginTester::InstallSIS(const TDesC& aSisFile)
	{
	TRAPD(err,CSwiAppInstall::InstallL(aSisFile));
	return err;
	}
