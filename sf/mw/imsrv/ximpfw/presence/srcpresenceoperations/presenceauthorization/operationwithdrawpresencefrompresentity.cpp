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
* Description:  Withdraw presence from presentity
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationwithdrawpresencefrompresentity.h"
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
// COperationWithdrawPresenceFromPresentity::COperationWithdrawPresenceFromPresentity()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationWithdrawPresenceFromPresentity::COperationWithdrawPresenceFromPresentity()
    {
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentity::~COperationWithdrawPresenceFromPresentity()
// ---------------------------------------------------------------------------
//
COperationWithdrawPresenceFromPresentity::~COperationWithdrawPresenceFromPresentity()
    {
    delete iIdentity;
    
    if( iConfItem )
        {
        iConfItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentity::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentity::ConstructL( const TDesC8& aParamPck )
    {
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iIdentity, aParamPck );    
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentity::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentity::ProcessL()
    {
    TRACE(_L("COperationWithdrawPresenceFromPresentity::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CPresenceConfigurationItem& confItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresenceConfigurationItemLC( *iIdentity );
    CleanupStack::Pop(); // subItem
    iConfItem = &confItem;

    iConfigurationStatus = iConfItem->ConfigurationStatus( iContext );

    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    iContext->SetPresenceInfoFilterL( MXIMPPscContext::EGrantPresenceToPresentityFilter,
            NULL, iIdentity );

    switch( iConfigurationStatus )
        {
        case CPresenceConfigurationItem::EConfiguredForCtxOnly:
            {
            authorization.DoWithdrawPresenceGrantFromPresentityL( *iIdentity, iReqId );
            iConfItem->RemoveConfigurator( iContext );
            break;
            }

        case CPresenceConfigurationItem::EConfiguredForCtxAndOthers:
            {
            CPresenceInfoFilterImp* pifWithoutCtx =
                iConfItem->CollectConfigurationPifWithoutCtxL( iContext );
            CleanupStack::PushL( pifWithoutCtx );

            if ( iConfItem->ConfigurationPif().Contains( *pifWithoutCtx ) )
                {
                authorization.DoUpdatePresenceGrantPifForPresentityL( *iIdentity, *pifWithoutCtx, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }

            CleanupStack::PopAndDestroy( pifWithoutCtx );
            break;
            }

        case CPresenceConfigurationItem::ENotConfiguredAtAll:
            {
            // OPAA-73BCS8
            authorization.DoWithdrawPresenceGrantFromPresentityL( *iIdentity, iReqId );
            break;
            }
        case CPresenceConfigurationItem::EConfiguredForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );            
        }    
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentity::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentity::RequestCompletedL()
    {
    TRACE(_L("COperationWithdrawPresenceFromPresentity::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iConfigurationStatus )
            {
            case CPresenceConfigurationItem::EConfiguredForCtxOnly:
            case CPresenceConfigurationItem::EConfiguredForCtxAndOthers:
                {
                iConfItem->RemoveConfigurator( iContext );
                break;
                }
                
            case CPresenceConfigurationItem::ENotConfiguredAtAll:
            case CPresenceConfigurationItem::EConfiguredForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentity::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationWithdrawPresenceFromPresentity::Type() const
    {
    return NPresenceOps::EPrWithdrawPresenceFromPresentity;
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromPresentity::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromPresentity::SynthesiseEventL()
    {   
    }



// End of file
