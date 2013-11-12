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
// DST TZ Database Compiler 
// 
//

#ifndef __PERSISTED_ENTITY_WRAPPER_H
#define __PERSISTED_ENTITY_WRAPPER_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include "..\..\tz\Server\Include\tzpersisted.h"

//============================================================================
// Forward Declarations
//============================================================================
class CTZDocument;
//============================================================================
// CPersistedEntityWrapper class definition
// Abstract base wrapper class for all the entities.  
// Each specialised wrapper class will be responsible for the behaviour of a 
// persisted entity.
//============================================================================
class CPersistedEntityWrapper
	{
public:
	CPersistedEntityWrapper(CTZDocument& aDocument);
	virtual ~CPersistedEntityWrapper();

	virtual void ExternaliseL(std::ofstream& aFilestream)=0;

	// When a derived class is destroyed, call this on each referenced entity.
	virtual void DecrRefCount() { --iReferenceCount; }
	virtual void IncrRefCount() { ++iReferenceCount; }
	virtual bool IsUnreferenced() { return iReferenceCount==0; } 
	virtual TUint16 ReferenceCount() { return iReferenceCount; }

	int iOffset;				//Reference to the entity in the database

protected:
	CTZDocument& iDocument;
	
private:
	TUint16 iReferenceCount;		//Number of objects referencing this entity
								//If this remains 0 after assembly then 
								//this entity will not be persisted.
	};

//============================================================================
// SEntityCheck
// Used to remove unreferenced entities from the entity tables
//============================================================================
struct SEntityCheck
	{
	TBool operator()(CPersistedEntityWrapper* aEntity)
		{
		bool remove = aEntity->IsUnreferenced();
		if (remove)
			{
			delete aEntity;
			}
		return remove;
		}
	};

#endif //__PERSISTED_ENTITY_WRAPPER_H
//============================================================================
// End of file
//============================================================================
