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

#ifndef __ASSRVSTARTUP_H__
#define __ASSRVSTARTUP_H__

// System includes
#include <e32std.h>

//
// ----> ASSrvStartup (header)
//
class ASSrvStartup
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//

	/**
	 * Start the server, returning an error if one occurs
	 */
	static TInt								RunServer();

	/**
	 * Start the server, leaving if there's an error
	 */
	static void								RunServerL();

	};


#endif
