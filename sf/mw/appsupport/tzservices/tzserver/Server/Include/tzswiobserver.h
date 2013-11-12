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

#ifndef __TZSWIOBSERVER_H__
#define __TZSWIOBSERVER_H__

#include <e32base.h>
#include "mtimezonedataaccess.h"

/**
Handles the events received from the CTzSwiObserverPlugin class, analyzes them
and notifies the observers.

@internalComponent
*/
class CTzSwiObserver : public CBase
	{
public:
	static CTzSwiObserver* NewL();
	~CTzSwiObserver();
	
private:
	CTzSwiObserver();
	
public:
	void AddObserverL(MTzDataObserver* aObserver);
	void RemoveObserver(MTzDataObserver* aObserver);
	void BeginL();
	void FileChangedL(RTz::TSWIObserverFilterIndex aType);
	void EndL();
	
private:
	TBool iReadOnlyTzDbChanged;
	TBool iResourcesChanged;
	// We don't own the observers
	RPointerArray<MTzDataObserver> iObservers;
	};

#endif
