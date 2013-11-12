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

#ifndef __AMASTART_H__	
#define __AMASTART_H__

#include "amastartasync.h"
#include <sysmonclisess.h>
#include <dscstore.h>

class CStartSafe;
class CDscItem;

//Used to start After Market Applications (AMA).
class CAmaStart : public CBase
	{
public:
	IMPORT_C static CAmaStart* NewL();
	IMPORT_C void StartL(const TUid aDscId);
	IMPORT_C void Start(const TUid aDscId, TRequestStatus& aRequestStatus);
	IMPORT_C void CancelStart();
	IMPORT_C ~CAmaStart();

private:
	CAmaStart();
	void ConstructL();
	void StartDscItemL(const CDscItem& aDscItem);
	void MonitorProcessL(const CStartupProperties& aProperties, const RProcess& aProcess);

private:
	RDscStore iDscStore;
	CStartSafe* iStartSafe;
	TRequestStatus* iClientStatus; // Not owning
	CAmaStartAsync* iAmaStarter;
	RSysMonSession iSysMon;
	};
#endif // __AMASTART_H__
