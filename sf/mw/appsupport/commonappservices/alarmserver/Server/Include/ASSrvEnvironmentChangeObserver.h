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

#ifndef __ASSRVENVIRONMENTCHANGEOBSERVER_H__
#define __ASSRVENVIRONMENTCHANGEOBSERVER_H__

// System includes
#include <e32base.h>

//
// ----> MASSrvEnvironmentChangeObserver (header)
//
/**
 * The abstract interface which must be implemented by
 * objects wishing to be informed when an environment
 * change event occurs.
 */
class MASSrvEnvironmentChangeObserver
	{
//
public:										// FROM MASSrvEnvironmentChangeObserver
//

	/**
	 * Handle an environment change event. The changes are a
	 * series of flags.
	 */
	virtual void							MEnvChangeHandleEvent(TInt aChanges, TUint aWorkdays, TBool aWorkdaysChanged) = 0;
	};

#endif
