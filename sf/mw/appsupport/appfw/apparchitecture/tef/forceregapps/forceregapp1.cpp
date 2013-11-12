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
#include "../t_forceregstep.h"
#include <e32property.h>


TInt E32Main()
    {
    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KForceRegTestPropertyKey, EOwnerThread));
    
    TInt status;
    forceRegStatus.Get(status);
    status |= KForceRegApp1Executed;
    forceRegStatus.Set(status);
    forceRegStatus.Close();
    
    return(KErrNone);
    }
