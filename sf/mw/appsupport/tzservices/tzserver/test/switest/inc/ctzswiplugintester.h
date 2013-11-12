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

#ifndef CTZSWIPLUGINTESTER_H
#define CTZSWIPLUGINTESTER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <f32file.h>
#include <e32property.h>

_LIT(KSwiTzLogDir, "c:\\logs\\Tz\\");
_LIT(KSwiTzLogFile, "c:\\logs\\Tz\\TzSwiObserver.log");

class CCTzSwiPluginTester : public CActive
	{
public:
	IMPORT_C ~CCTzSwiPluginTester();
	IMPORT_C static CCTzSwiPluginTester* NewL();
	IMPORT_C static CCTzSwiPluginTester* NewLC();
	IMPORT_C TInt InstallSIS(const TDesC& aSisFile);
	IMPORT_C void InstallSIS(const TDesC& aSisFile, TUint aNotifyType, TRequestStatus& aReqStatus);
	IMPORT_C TInt UninstallSIS(const TDesC& aApplicationUID, const TDesC& aPackageName, const TDesC& aVendorId);
	IMPORT_C void UninstallSIS(const TDesC& aApplicationUID, const TDesC& aPackageName, const TDesC& aVendorId, TUint aNotifyType, TRequestStatus& aReqStatus);
	
	enum TNotifyType
		{
		EPropertyRules = 0,
		EPropertyNames,
		EFileLog
		};
private:
	CCTzSwiPluginTester();
	void ConstructL();
private:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
private:
	RProperty iPropertyDb;
	RProperty iPropertyLoc;
	RFs iFs;
	TRequestStatus* iReqStatus;
	TNotifyType iNotifyType;
	}; // class CCTzSwiPluginTester

#endif // CTZSWIPLUGINTESTER_H
