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
* Description:  CCFClient class implementation.
*
*/


// INCLUDE FILES

#include "cfclient.h"
#include "CFClientImpl.h"
#include "cftrace.h"

EXPORT_C CCFClient* CCFClient::NewL(
    MCFListener& aListener )
    {
    return CCFClientImpl::NewL( aListener );
    }

EXPORT_C CCFClient* CCFClient::NewLC(
    MCFListener& aListener )
    {
    return CCFClientImpl::NewLC( aListener );
    }
