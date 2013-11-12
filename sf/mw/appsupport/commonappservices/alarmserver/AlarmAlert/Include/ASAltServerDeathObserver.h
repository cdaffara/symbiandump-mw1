// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASALTSERVERDEATHOBSERVER_H__
#define __ASALTSERVERDEATHOBSERVER_H__

// System includes
#include <e32base.h>

// User includes

// Type definitions

// Constants

// Enumerations

// Classes referenced
class RASAltClientSession;


//
// ----> CASAltServerDeathObserver (header)
//

class CASAltServerDeathObserver : public CActive
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASAltServerDeathObserver*		NewL(RASAltClientSession& aSession);
	~CASAltServerDeathObserver();

//
private:									// INTERNAL CONSTRUCT
//
	CASAltServerDeathObserver(RASAltClientSession& aSession);
	void									ConstructL();

//
private:									// FROM CActive
//

	void									RunL();

	void									DoCancel();

//
private:									// INTERNAL
//

	inline RASAltClientSession&				Session() { return iSession; }

//
private:									// MEMBER DATA
//

	RASAltClientSession&					iSession;
	};

#endif
