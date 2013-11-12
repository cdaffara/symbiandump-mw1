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
* Description:  Implementation for managing cached subscribed presence information.
 *
*/


#include <ximpobjectcollection.h>
#include <ximpidentity.h>
#include <presenceinfo.h>
#include <ximpdatasubscriptionstate.h>
#include <ximpstatus.h>

#include "protocolpresencedatahostimp.h"
#include "protocolpresencewatchingdatahostimp.h"

#include "ximpoperationbase.h"
#include "ximpoperationfactory.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpglobals.h"
#include "ximphost.h"
#include "presencedatacacheimp.h"
#include "presenceoperationdefs.h"
#include "ximptrace.h"

// ======== LOCAL FUNCTIONS ========


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolPresenceWatchingDataHostImp*
        CProtocolPresenceWatchingDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolPresenceWatchingDataHostImp* self =
                new( ELeave ) CProtocolPresenceWatchingDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolPresenceWatchingDataHostImp,
                                     MProtocolPresenceWatchingDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolPresenceWatchingDataHostImp,
                                           MProtocolPresenceWatchingDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::~CProtocolPresenceWatchingDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresenceWatchingDataHostImp::~CProtocolPresenceWatchingDataHostImp()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::CProtocolPresenceWatchingDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresenceWatchingDataHostImp::CProtocolPresenceWatchingDataHostImp(
                                                                    MXIMPHost& aHost )
    : iHost( aHost )
    {
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolPresenceWatchingDataHostImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::HandleSubscribedPresentityPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId
    CProtocolPresenceWatchingDataHostImp::HandleSubscribedPresentityPresenceL(
                MXIMPIdentity* aPresentityId,
                MPresenceInfo* aPresenceInfo )
    {
    TRACE(_L("CProtocolPresenceWatchingDataHostImp::HandleSubscribedPresentityPresenceL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityPresence, 
                                   aPresentityId, aPresenceInfo );
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::PresentityPresenceDataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
    CProtocolPresenceWatchingDataHostImp::PresentityPresenceDataSubscriptionState( 
            const MXIMPIdentity& aPresentityId ) const
    {
    TRACE(_L("CProtocolPresenceWatchingDataHostImp::PresentityPresenceDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().PresentityPresenceDataSubscriptionState( aPresentityId );
    }
// ---------------------------------------------------------------------------
// CProtocolPresenceWatchingDataHostImp::SetPresentityPresenceDataSubscriptionStateL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceWatchingDataHostImp::SetPresentityPresenceDataSubscriptionStateL( 
            MXIMPIdentity* aPresentityId, 
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )
    {
    TRACE(_L("CProtocolPresenceWatchingDataHostImp::SetPresentityPresenceDataSubscriptionStateL") );
    CXIMPRestrictedObjectCollectionImp* collection = CXIMPRestrictedObjectCollectionImp::NewLC( ETrue );
    collection->AddOrReplaceTypeL( aPresentityId );
    collection->AddOrReplaceTypeL( aSubscriptionState );
    collection->AddOrReplaceTypeL( aStatus );    
    TXIMPRequestId reqId = 
        iHost.AddNewOperationL( NPresenceOps::EPrSetPresentityPresenceDataSubscriptionState,
                                collection );
    CleanupStack::Pop( collection );
    return reqId;    
    }




// End of file

