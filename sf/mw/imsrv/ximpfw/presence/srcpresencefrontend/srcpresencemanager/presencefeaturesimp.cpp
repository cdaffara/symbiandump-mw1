/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MXIMPContext
 *
*/


#include <ximpcontext.h>
#include "presencefeaturesimp.h"
#include "presenceobjectfactoryimp.h"
#include "presencepublishingimp.h"
#include "presentitygroupsimp.h"
#include "presencewatchingimp.h"
#include "presenceauthorizationimp.h"
#include "ximpcontextclientinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpcontextinternal.h"



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceFeaturesImp* CPresenceFeaturesImp::NewL(MXIMPContext* aContext)
    {
    CPresenceFeaturesImp* self = new( ELeave ) CPresenceFeaturesImp() ;
    CleanupStack::PushL( self );
    self->ConstructL(aContext);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//

XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceFeaturesImp, 
                                     MPresenceFeatures )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceFeaturesImp, 
                                           MPresenceFeatures )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::~CPresenceFeaturesImp()
// ---------------------------------------------------------------------------
//
CPresenceFeaturesImp::~CPresenceFeaturesImp()
    {
    delete iPresGroup;
    delete iPresWatch;
    delete iPresAuth;
    delete iPresPub;
    delete iPresObjFactory;
    iCtxInternal = NULL;
    }


// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::CPresenceFeaturesImp( CXIMPContextClientProxy& aCtxProxy )
// ---------------------------------------------------------------------------
//
CPresenceFeaturesImp::CPresenceFeaturesImp()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::SetCtxL(MXIMPContext* aContext)
// ---------------------------------------------------------------------------
//
void CPresenceFeaturesImp::SetCtxL(MXIMPContext* aContext)
    {
    if(!aContext)
        User::Leave(KErrArgument);
    iCtxInternal = TXIMPGetInterface<MXIMPContextInternal>::From(*aContext,EReturnNullIfUnknown);
                       
    }

// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::ConstructL(MXIMPContext* aContext)
// ---------------------------------------------------------------------------
//
void CPresenceFeaturesImp::ConstructL(MXIMPContext* aContext)
    {
    SetCtxL(aContext);

    iPresObjFactory = CPresenceObjectFactoryImp::NewL();
    iPresPub = CPresencePublishingImp::NewL( *iCtxInternal );
    iPresWatch = CPresenceWatchingImp::NewL( *iCtxInternal );
    iPresGroup = CPresentityGroupsImp::NewL( *iCtxInternal );
    iPresAuth = CPresenceAuthorizationImp::NewL(*iCtxInternal );
    }


// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::PresenceObjectFactory()
// ---------------------------------------------------------------------------
//
MPresenceObjectFactory& CPresenceFeaturesImp::PresenceObjectFactory() const
    {
    return *iPresObjFactory;
    }


// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::PresencePublishing()
// ---------------------------------------------------------------------------
//
MPresencePublishing& CPresenceFeaturesImp::PresencePublishing() const
    {
    return *iPresPub;
    }


// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::PresentityGroups()
// ---------------------------------------------------------------------------
//
MPresentityGroups& CPresenceFeaturesImp::PresentityGroups() const
    {
    return *iPresGroup;
    }

// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::PresenceWatching()
// ---------------------------------------------------------------------------
//
MPresenceWatching& CPresenceFeaturesImp::PresenceWatching() const
    {
    return *iPresWatch;
    }

// ---------------------------------------------------------------------------
// CPresenceFeaturesImp::PresenceWatching()
// ---------------------------------------------------------------------------
//
MPresenceAuthorization& CPresenceFeaturesImp::PresenceAuthorization() const
    {
    return *iPresAuth;
    }



