// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent - Internal Symbian test code 
*/

#include <e32base.h>
#include "../t_nonnativetest.h"
#include <e32property.h>
#include <apacmdln.h>

const TUid KPropertyCategory = {0x101F289C};
const TUint KNonNativeTestPropertyKey = 2;

TInt E32Main()
    {
    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KNonNativeTestPropertyKey, EOwnerThread));
    
    forceRegStatus.Set(1);
    forceRegStatus.Close();
    return(KErrNone);
    }
