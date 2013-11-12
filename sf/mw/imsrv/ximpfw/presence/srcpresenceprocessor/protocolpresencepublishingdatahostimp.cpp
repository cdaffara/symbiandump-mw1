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
* Description:  Implementation for managing cached presence publish information.
 *
*/


#include <ximpobjectcollection.h>
#include <presenceinfo.h>
#include <ximpdatasubscriptionstate.h>
#include <ximpstatus.h>

#include "protocolpresencedatahostimp.h"
#include "protocolpresencepublishingdatahostimp.h"

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
// CProtocolPresencePublishingDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolPresencePublishingDataHostImp*
        CProtocolPresencePublishingDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolPresencePublishingDataHostImp* self =
                new( ELeave ) CProtocolPresencePublishingDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolPresencePublishingDataHostImp,
                                     MProtocolPresencePublishingDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolPresencePublishingDataHostImp,
                                           MProtocolPresencePublishingDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::~CProtocolPresencePublishingDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresencePublishingDataHostImp::~CProtocolPresencePublishingDataHostImp()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::CProtocolPresencePublishingDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresencePublishingDataHostImp::CProtocolPresencePublishingDataHostImp(
                                                                    MXIMPHost& aHost )
    : iHost( aHost )
    {
    }

// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolPresencePublishingDataHostImp::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::HandleSubscribedOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId
    CProtocolPresencePublishingDataHostImp::HandleSubscribedOwnPresenceL(
                                        MPresenceInfo* aPresenceInfo )
    {
    TRACE(_L("CProtocolPresencePublishingDataHostImp::HandleSubscribedOwnPresenceL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandleOwnPresence,
                                   aPresenceInfo );
    }


// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::OwnPresenceDataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
    CProtocolPresencePublishingDataHostImp::OwnPresenceDataSubscriptionState() const
    {
    TRACE(_L("CProtocolPresencePublishingDataHostImp::OwnPresenceDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().OwnPresenceDataSubscriptionState();
    }

// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::SetOwnPresenceDataSubscriptionStateL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresencePublishingDataHostImp::SetOwnPresenceDataSubscriptionStateL( 
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )
    {
    TRACE(_L("CProtocolPresencePublishingDataHostImp::SetOwnPresenceDataSubscriptionStateL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrSetOwnPresenceDataSubscriptionState,
                             aSubscriptionState, aStatus );
    }

// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::HandlePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId
    CProtocolPresencePublishingDataHostImp::HandlePresenceWatcherListL(
                                        MXIMPObjectCollection* aWatcherList )
    {
    TRACE(_L("CProtocolPresencePublishingDataHostImp::HandlePresenceWatcherListL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceWatcherList,
                             aWatcherList );
    }

// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::HandlePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
    CProtocolPresencePublishingDataHostImp::PresenceWatcherListDataSubscriptionState() const
    {
    TRACE(_L("CProtocolPresencePublishingDataHostImp::PresenceWatcherListDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().PresenceWatcherListDataSubscriptionState();
    }

// ---------------------------------------------------------------------------
// CProtocolPresencePublishingDataHostImp::HandlePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId 
    CProtocolPresencePublishingDataHostImp::SetPresenceWatcherListDataSubscriptionStateL(
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )
    {
    TRACE(_L("CProtocolPresencePublishingDataHostImp::SetPresenceWatcherListDataSubscriptionStateL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrSetPresenceWatcherListDataSubscriptionState,
                             aSubscriptionState, aStatus );    
    }

// End of file

