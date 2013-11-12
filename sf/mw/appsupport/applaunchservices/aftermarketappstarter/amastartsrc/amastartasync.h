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

/**
 @file
 @internalTechnology
 @released
*/

#ifndef __AMASTARTASYNC_H__
#define __AMASTARTASYNC_H__

#include "ssmdebug.h"
#include <dscstore.h>
#include <dscitem.h>
#include <startsafe.h>
#include <ssm/ssmstartsafe.h>

NONSHARABLE_CLASS( CAmaStartAsync ) : public CActive
	{
public:
	static CAmaStartAsync* NewL();
	static CAmaStartAsync* NewLC();
	~CAmaStartAsync();

	void StartL(const TUid& aDscId, TRequestStatus& aStatus);

protected:
	//From CActive
	void RunL();
	TInt RunError( TInt aError );
	void DoCancel();

private:
	CAmaStartAsync();
	void ConstructL();
	void StartDscItem(const CDscItem& aDscItem);
	void MonitorProcessL(const CStartupProperties& aProperties, const RProcess& aProcess);
	void DoStartL();

private:
	RDscStore iDscStore;
	CSsmStartSafe* iSsmStartSafe;
	RSysMonSession iSysMon;
	TBool iRunning;
	TRequestStatus* iAmaStatus;
	RProcess iCurrentProcess; //A process to be created inside iSsmStartSafe->StartL();
	CDscItem* iCurrentDscItem;
	TInt iSSCancelIndex;
	};

#endif // __AMASTARTASYNC_H__
