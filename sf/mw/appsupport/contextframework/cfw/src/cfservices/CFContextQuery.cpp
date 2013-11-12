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
* Description:  CCFContextQuery class implementation.
*
*/


#include "cfcontextquery.h"
#include "CFContextQueryImpl.h"
#include "cftrace.h"

// MEMBER FUNCTIONS

EXPORT_C CCFContextQuery* CCFContextQuery::NewL()
    {
    FUNC_LOG;
    
    return CCFContextQuery::NewL( KNullDesC, KNullDesC );
    }

EXPORT_C CCFContextQuery* CCFContextQuery::NewLC()
    {
    FUNC_LOG;
    
    return CCFContextQuery::NewLC( KNullDesC, KNullDesC );
    }

EXPORT_C CCFContextQuery* CCFContextQuery::NewL( const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    return CCFContextQueryImpl::NewL( aSource, aType );
    }

EXPORT_C CCFContextQuery* CCFContextQuery::NewLC( const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    return CCFContextQueryImpl::NewLC( aSource, aType );
    }
