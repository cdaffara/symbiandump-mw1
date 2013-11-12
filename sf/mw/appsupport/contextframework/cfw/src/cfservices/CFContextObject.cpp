/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextObject class description.
*
*/


#include "cfcontextobject.h"
#include "CFContextObjectImpl.h"
#include "cftrace.h"

// MEMBER FUNCTIONS

EXPORT_C CCFContextObject* CCFContextObject::NewL()
    {
    FUNC_LOG;
    
    return CCFContextObjectImpl::NewL();
    }
    
EXPORT_C CCFContextObject* CCFContextObject::NewLC()
    {
    FUNC_LOG;
    
    return CCFContextObjectImpl::NewLC();
    }

EXPORT_C CCFContextObject* CCFContextObject::NewL( const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    return CCFContextObjectImpl::NewL( aSource, aType, aValue );
    }
    
EXPORT_C CCFContextObject* CCFContextObject::NewLC( const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue)
    {
    FUNC_LOG;
    
    return CCFContextObjectImpl::NewLC( aSource, aType, aValue );
    }
