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
* Description:  Implementation for managing cached authorization information.
 *
*/


//#include <badesca.h>
//#include <e32cmn.h>

#include <ximpobjectcollection.h>
#include <presencegrantrequestinfo.h>
#include <ximpidentity.h>
#include <presenceblockinfo.h>
#include <ximpdatasubscriptionstate.h>
#include <ximpstatus.h>

#include "protocolpresencedatahostimp.h"
#include "protocolpresenceauthorizationdatahostimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpoperationbase.h"
#include "ximpglobals.h"
#include "ximpoperationfactory.h"
#include "ximphost.h"
#include "presencedatacacheimp.h"
#include "presenceoperationdefs.h"
#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::CProtocolPresenceAuthorizationDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresenceAuthorizationDataHostImp::CProtocolPresenceAuthorizationDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolPresenceAuthorizationDataHostImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolPresenceAuthorizationDataHostImp* CProtocolPresenceAuthorizationDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolPresenceAuthorizationDataHostImp* self = new( ELeave ) CProtocolPresenceAuthorizationDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolPresenceAuthorizationDataHostImp,
                                     MProtocolPresenceAuthorizationDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolPresenceAuthorizationDataHostImp,
                                           MProtocolPresenceAuthorizationDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::~CProtocolPresenceAuthorizationDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresenceAuthorizationDataHostImp::~CProtocolPresenceAuthorizationDataHostImp()
    {
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestListL(
            MXIMPObjectCollection* aGrantRequestList )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestListL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceGrantRequestList, aGrantRequestList );
    }
// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestReceivedL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestReceivedL(
            MPresenceGrantRequestInfo* aGrantRequest )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestReceivedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceGrantRequestReceived, aGrantRequest );
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestObsoletedL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestObsoletedL(
            MXIMPIdentity* aGrantRequestor )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::HandlePresenceGrantRequestObsoletedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceGrantRequestObsoleted, aGrantRequestor );
    }
    
// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::PresenceGrantRequestDataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
       CProtocolPresenceAuthorizationDataHostImp::PresenceGrantRequestDataSubscriptionState() const
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::PresenceGrantRequestDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().PresenceGrantRequestDataSubscriptionState();
    }
    
// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::SetPresenceGrantRequestDataSubscriptionStateL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId 
    CProtocolPresenceAuthorizationDataHostImp::SetPresenceGrantRequestDataSubscriptionStateL( 
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::SetPresenceGrantRequestDataSubscriptionStateL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrSetPresenceGrantRequestDataSubscriptionState,
                                   aSubscriptionState, aStatus );
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockListL(
            MXIMPObjectCollection* aBlockList )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockListL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceBlockList, aBlockList );
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockedL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockedL(
            MPresenceBlockInfo* aBlockInfo )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceBlocked, aBlockInfo );
    }
// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockCanceledL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockCanceledL(
            MXIMPIdentity* aBlockedIdentity )
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::HandlePresenceBlockCanceledL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresenceBlockCanceled, aBlockedIdentity );
    }
    
// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::PresenceBlockDataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
    CProtocolPresenceAuthorizationDataHostImp::PresenceBlockDataSubscriptionState() const
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::PresenceBlockDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().PresenceBlockDataSubscriptionState();
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceAuthorizationDataHostImp::SetPresenceBlockDataSubscriptionStateL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresenceAuthorizationDataHostImp::SetPresenceBlockDataSubscriptionStateL(
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )               
    {
    TRACE(_L("CProtocolPresenceAuthorizationDataHostImp::SetPresenceBlockDataSubscriptionStateL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrSetPresenceBlockDataSubscriptionState,
                                   aSubscriptionState, aStatus );
    }

// End of file
