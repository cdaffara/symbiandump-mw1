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

#ifndef __TZUSERDATACACHE_H__
#define __TZUSERDATACACHE_H__

#include <e32base.h>
#include "tzconfigagent.h"

class CTzUserDataCache : public CBase
	{
public:
	static CTzUserDataCache* NewL();
	~CTzUserDataCache();
	
public:
	void SetNames(CTzUserNames* aNames);
	void SetIdsL(const RArray<TUint32>& Ids);
	TInt SizeOfNames() const;
	TInt GetNameSize() const; 
	const CTzUserNames& GetNames() const;
	const TArray<TUint32> GetIds() const;

	TInt SizeOfIds() const; 
	void Reset();
	
private:
	CTzUserDataCache();		
	
private:
	CTzUserNames* iNames;
	RArray<TUint32> iIds;
	};
	
#endif
