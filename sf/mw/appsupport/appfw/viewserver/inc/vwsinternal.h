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

#ifndef __viewsrv_internal_H__
#define __viewsrv_internal_H__

#include <e32base.h>
#include <gdi.h>
#include <vwsdef.h>

// Forward declarations.
class MVwsAppStarter;
class TVwsViewIdAndMessage;
class TVwsViewEvent; 

//
// Panic.
//
/**
@internalComponent
*/
enum TVwsPanic
	{
	EVwsCreateScheduler=1,
	EVwsThreadRename
	};

/**
@internalComponent
*/
GLREF_C void Panic(TVwsPanic aPanic);

/**
Server thread start.

@internalComponent
*/
GLDEF_C TInt ViewServerThreadStart(TAny* aPtr);

/**
@internalComponent
*/
struct SVwsCommandLine
	{
	MVwsAppStarter* iAppStarter;
	};

//
// Typedefs.
//
/**
@internalComponent
@released
*/
typedef TPckgBuf<TVwsViewIdAndMessage> TVwsViewIdAndMessageBuf;

/**
@internalComponent
@released
*/
typedef TPckgBuf<TVwsViewEvent> TVwsViewEventBuf;

#endif

