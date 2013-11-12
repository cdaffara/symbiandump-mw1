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

#ifndef __ASSRVSERVER_H__
#define __ASSRVSERVER_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvServerWideData.h"

// Type definitions

// Constants

// Enumerations


//
// ----> CASSrvServer (header)
//
/**
 * The Alarm Server
 */
class CASSrvServer : public CPolicyServer
	{
	
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvServer*					NewLC();
	~CASSrvServer();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvServer();
	void									ConstructL();

//
public:										// FROM CServer2
//
	/**
	 * @see CServer2
	 */
	CSession2*						NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;

//
private:									// FROM CActive
//
	/**
	 * @see CActive
	 */
	TInt									RunError(TInt aError);

//
private:									// MEMBER DATA
//

	/**
	 * The global server-wide data for the whole Alarm Server
	 */
	CASSrvServerWideData*					iServerWideData;
	TCustomResult CustomSecurityCheckL(const RMessage2& aMsg, TInt& aAction, TSecurityInfo& aMissing);
	
	};

#endif
