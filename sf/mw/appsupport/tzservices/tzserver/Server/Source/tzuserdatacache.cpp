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
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif
#include "tzuserdatacache.h"

/**
When the data of a user defined rules, names and IDs are sent from the server
to the client, the size of the data is requested first and then, the client can 
create a data buffer and send it to the server to fill the data.

Therefore, some client-server calls (e.g. CTzUserData::ReadRulesL(),
CTzUserData::ReadNamesL(), CTzUserData::GetTzIdsL() must be carried out in two
steps:

- Get the size of the data to allow client to create data buffer
- Get the data using this buffer

The class CTzUserDataCache is for:

- Find the data size
- Cache the data
- Retrieve cached data
*/	

CTzUserDataCache::CTzUserDataCache()
	 :iNames(NULL)
	{
	}

CTzUserDataCache* CTzUserDataCache::NewL()
	{
	CTzUserDataCache* self = new (ELeave) CTzUserDataCache();
	return self;
	}
	
CTzUserDataCache::~CTzUserDataCache()
	{
	Reset();
	iIds.Close();
	}

void CTzUserDataCache::SetNames(CTzUserNames* aNames)
  	{
 	delete iNames;
  	iNames = aNames;
  	}

void CTzUserDataCache::SetIdsL(const RArray<TUint32>& aIds)
	{
	iIds.Reset();
	for(TInt ii=0;ii<aIds.Count();++ii)
		{
		iIds.AppendL((aIds)[ii]);
		}
	}
	
TInt CTzUserDataCache::SizeOfNames() const 
	{
	__ASSERT_DEBUG(iNames, User::Invariant());
	if(iNames)
		{
		return iNames->SizeOfObject();
		}
	return 0;
	}

const CTzUserNames& CTzUserDataCache::GetNames() const
	{
	__ASSERT_DEBUG(iNames, User::Invariant());
	return *iNames;
	}
	
const TArray<TUint32> CTzUserDataCache::GetIds() const
	{
	return iIds.Array();
	}
	
TInt CTzUserDataCache::SizeOfIds() const
	{
	return  (iIds.Count()+1) * sizeof (TInt);	
	}
	
void CTzUserDataCache::Reset()
	{
	delete iNames;
	iNames = NULL;
	iIds.Reset();
	}

