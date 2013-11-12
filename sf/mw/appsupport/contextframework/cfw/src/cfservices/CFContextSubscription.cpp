/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  CCFContextSubscription class implementation.
*
*/


#include "cfcontextsubscription.h"
#include "CFContextSubscriptionImpl.h"


EXPORT_C CCFContextSubscription* CCFContextSubscription::NewL()
    {
    return CCFContextSubscriptionImpl::NewL();
    }

EXPORT_C CCFContextSubscription* CCFContextSubscription::NewLC()
    {
    return CCFContextSubscriptionImpl::NewLC();
    }
