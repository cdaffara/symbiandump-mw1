/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MPresenceManagement
 *
*/

#include "ximpobjectfactoryimp.h"
#include "ximpstatusimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPObjectFactoryImp* CXIMPObjectFactoryImp::NewL()
    {
    CXIMPObjectFactoryImp* self = new( ELeave ) CXIMPObjectFactoryImp;
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPObjectFactoryImp, 
                                     MXIMPObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPObjectFactoryImp, 
                                           MXIMPObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::~CXIMPObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CXIMPObjectFactoryImp::~CXIMPObjectFactoryImp()
    {
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::CXIMPObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CXIMPObjectFactoryImp::CXIMPObjectFactoryImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::NewStatusLC()
// ---------------------------------------------------------------------------
//
MXIMPStatus* CXIMPObjectFactoryImp::NewStatusLC()
    {
    return CXIMPStatusImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::NewIdentityLC()
// ---------------------------------------------------------------------------
//
MXIMPIdentity* CXIMPObjectFactoryImp::NewIdentityLC()
    {
    return CXIMPIdentityImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::NewRestrictedObjectCollectionLC()
// ---------------------------------------------------------------------------
//
MXIMPRestrictedObjectCollection* CXIMPObjectFactoryImp::NewRestrictedObjectCollectionLC()
    {
    return CXIMPRestrictedObjectCollectionImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::NewRestrictedObjectCollectionLC()
// ---------------------------------------------------------------------------
//
MXIMPObjectCollection* CXIMPObjectFactoryImp::NewObjectCollectionLC()
    {
    return CXIMPObjectCollectionImp::NewLC();
    }
    
// ---------------------------------------------------------------------------
// CXIMPObjectFactoryImp::NewRestrictedObjectCollectionLC()
// ---------------------------------------------------------------------------
//
MXIMPDataSubscriptionState* CXIMPObjectFactoryImp::NewDataSubscriptionStateLC()
    {
    return CXIMPDataSubscriptionStateImp::NewLC();
    }


