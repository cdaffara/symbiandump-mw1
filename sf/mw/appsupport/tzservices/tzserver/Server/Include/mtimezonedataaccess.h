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

#ifndef __MTIMEZONEDATAACCESS_H__
#define __MTIMEZONEDATAACCESS_H__

#include <e32base.h>
#include <tz.h>

// typical zoneid: 2526, "Europe/London" 
const TInt KMaxTimeZoneIdSize = 64;

/** 
Abstract class defining the interface to observing the TZ Data Provider

@internalComponent
*/
class MTzDataObserver
	{
public:
	virtual void NotifyTZDataStatusChangeL(RTz::TTzChanges aChange) = 0;
	};

#endif // __MTIMEZONEDATAACCESS_H__
