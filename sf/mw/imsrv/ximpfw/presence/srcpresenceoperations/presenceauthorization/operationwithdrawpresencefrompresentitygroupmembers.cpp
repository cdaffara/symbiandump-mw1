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
* Description:  Withdraw presence from presentity group members operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationwithdrawpresencefrompresentitygroupmembers.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include "presencedatacacheimp.h"
#include "presenceinfofilterimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "ximppanics.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::COperationWithdrawPresenceFromPresentityGroupMembers()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationWithdrawPresenceFromPresentityGroupMembers::COperationWithdrawPresenceFromPresentityGroupMembers()
    {
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::~COperationWithdrawPresenceFromPresentityGroupMembers()
// ---------------------------------------------------------------------------
//
COperationWithdrawPresenceFromPresentityGroupMembers::~COperationWithdrawPresenceFromPresentityGroupMembers()
    {
    delete iGroupId;

    if( iConfItem )
        {
        iConfItem->Close();
        }    
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentityGroupMembers::ConstructL( const TDesC8& aParamPck )
    {
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iGroupId, aParamPck );    
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentityGroupMembers::ProcessL()
    {
    TRACE(_L("COperationWithdrawPresenceFromPresentityGroupMembers::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CGroupMembersPresenceConfigurationItem& confItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().GroupMembersPresenceConfigurationItemLC( *iGroupId );
    CleanupStack::Pop(); // subItem
    iConfItem = &confItem;

    iConfigurationStatus = iConfItem->ConfigurationStatus( iContext );

    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    iContext->SetPresenceInfoFilterL( MXIMPPscContext::EGrantPresenceToGroupMemberFilter,
            NULL, iGroupId );

    switch( iConfigurationStatus )
        {
        case CGroupMembersPresenceConfigurationItem::ESubscribedForCtxOnly:
            {
            authorization.DoWithdrawPresenceGrantFromPresentityGroupMembersL( *iGroupId, iReqId );
            iConfItem->RemoveConfigurator( iContext );
            break;
            }

        case CGroupMembersPresenceConfigurationItem::ESubscribedForCtxAndOthers:
            {
            CPresenceInfoFilterImp* pifWithoutCtx =
                iConfItem->CollectConfigurationPifWithoutCtxL( iContext );
            CleanupStack::PushL( pifWithoutCtx );

            if ( iConfItem->ConfigurationPif().Contains( *pifWithoutCtx ) )
                {
                authorization.DoUpdatePresenceGrantPifForPresentityGroupMembersL( *iGroupId, *pifWithoutCtx, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }

            CleanupStack::PopAndDestroy( pifWithoutCtx );
            break;
            }

        case CGroupMembersPresenceConfigurationItem::ENotSubscribedAtAll:
            {
            // OPAA-73BCS8
            authorization.DoWithdrawPresenceGrantFromPresentityGroupMembersL( *iGroupId, iReqId );
            break;
            }
        case CGroupMembersPresenceConfigurationItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }    
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentityGroupMembers::RequestCompletedL()
    {
    TRACE(_L("COperationWithdrawPresenceFromPresentityGroupMembers::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iConfigurationStatus )
            {
            case CGroupMembersPresenceConfigurationItem::ESubscribedForCtxOnly:
            case CGroupMembersPresenceConfigurationItem::ESubscribedForCtxAndOthers:
                {
                iConfItem->RemoveConfigurator( iContext );
                break;
                }
            
            case CGroupMembersPresenceConfigurationItem::ENotSubscribedAtAll:
            case CGroupMembersPresenceConfigurationItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
            
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationWithdrawPresenceFromPresentityGroupMembers::Type() const
    {
    return NPresenceOps::EPrWithdrawPresenceFromPresentityGroupMembers;
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentityGroupMembers::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentityGroupMembers::SynthesiseEventL()
    {   
    }



// End of file
