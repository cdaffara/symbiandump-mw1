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
// Provides the implementation of the PanicNow function for global
// use throughout the After Market Application Starter component and all related modules.
// 
//

/**
 @internalComponent
*/

#include "amastartpanic.h"

//
// Global panic function for use within After Market Application Starter modules
// (defined in amastartpanic.h)
void PanicNow(const TDesC &aCategory, TInt aReason)
  	{
  	User::Panic(aCategory, aReason);
  	}
