// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __DATAPROVIDER_H__
#define __DATAPROVIDER_H__

#include <tzdefines.h> // ttztimereference
#include "tzswiobserver.h"

const TInt KDataProviderType = 0;

class CReadOnlyTzDb;
class CTzRules;
class RFs;
class CTzUserDataDb;

class CTzDataProvider : public CBase
	{
public:
	static CTzDataProvider* NewL(RFs& aFs, CTzSwiObserver& aSwiObserver, 
		CTzUserDataDb& aUserDataTzDb, MTzDataObserver& aTzDataObserver);
	~CTzDataProvider();

public: 
	CTzId* GetDefaultTimeZoneIdL() const;
	void GetTzRulesL(CTzRules& aTzRules, CTzId& aTzId) const;
	void ReleaseTzRules();
	void GetOffsetsForTimeZoneIdsL(CBufFlat& aTimeZoneIdArray) const;
	TBool IsIdInDbL(TUint aTzId);
	
	void BackupBeginningL();
	void BackupCompletedL();
	void RestoreBeginningL();
	void RestoreCompletedL();

private:
	CTzDataProvider(RFs& aFs, CTzUserDataDb& aUserDataTzDb, MTzDataObserver& aTzDataObserver);
	void ConstructL(CTzSwiObserver& aSwiObserver);

private:
	CReadOnlyTzDb* iReadOnlyTzDb;
	RFs& iFs;
	MTzDataObserver& iTzDataObserver;
	CTzUserDataDb& iUserDataTzDb;
	};	

#endif // __DATAPROVIDER_H__
