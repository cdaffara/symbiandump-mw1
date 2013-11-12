// amastartpaniccodes.h

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
// This header file defines all the possible panic categories and codes within the After Market Application Starter component.
// When adding panic codes to the After Market Application Starter component and related modules,
// identify the module concerned and create a category if necessary.
// The category should contain the name of the class where the panic is occuring.
// Try to reuse an existing panic code otherwise add the new panic code 
// to the end of the enum list.
// 
//

#if !defined (__AMASTARTPANICCODES_H__)
#define __AMASTARTPANICCODES_H__

#include <e32std.h>

/** 
Defines all the modules (categories) within After Market Application Starter which can throw panics
Strings must not be longer than 16 characters or they will be truncated by User::Panic()
@internalComponent
@released
*/
_LIT(KPanicDsc, "CDscStore");

/** Defines all possible panic codes within After Market Application Starter
@internalComponent
@released
*/
enum TAmaStartPanicCodes
	{
	EIdCannotChange 		= 25 	// To maintain the compability for sysstart for v9.5
	};

/** Global panic function for use within After Market Application Starter
@internalComponent
@released
*/
extern void PanicNow(const TDesC &aCategory, TInt aReason);

#endif // __AMASTARTPANICCODES_H__
