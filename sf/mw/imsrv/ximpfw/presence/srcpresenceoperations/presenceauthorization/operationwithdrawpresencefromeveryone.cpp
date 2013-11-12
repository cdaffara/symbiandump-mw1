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
* Description:  Withdraw presence from everyone
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationwithdrawpresencefromeveryone.h"
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
// COperationWithdrawPresenceFromEveryone::COperationWithdrawPresenceFromEveryone()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationWithdrawPresenceFromEveryone::COperationWithdrawPresenceFromEveryone()
    {
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromEveryone::~COperationWithdrawPresenceFromEveryone()
// ---------------------------------------------------------------------------
//
COperationWithdrawPresenceFromEveryone::~COperationWithdrawPresenceFromEveryone()
    {
    if( iConfItem )
        {
        iConfItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromEveryone::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromEveryone::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromEveryone::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromEveryone::ProcessL()
    {
    TRACE(_L("COperationWithdrawPresenceFromEveryone::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CPresenceToEveryoneConfigurationItem& confItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresenceToEveryoneConfigurationItemLC();
    CleanupStack::Pop(); // subItem
    iConfItem = &confItem;

    iConfigurationStatus = iConfItem->ConfigurationStatus( iContext );

    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    iContext->SetPresenceInfoFilterL( MXIMPPscContext::EGrantPresenceToEveryoneFilter,
            NULL, NULL );

    switch( iConfigurationStatus )
        {
        case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxOnly:
            {
            authorization.DoWithdrawPresenceGrantFromEveryoneL( iReqId );
            iConfItem->RemoveConfigurator( iContext );
            break;
            }

        case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxAndOthers:
            {
            CPresenceInfoFilterImp* pifWithoutCtx =
                iConfItem->CollectConfigurationPifWithoutCtxL( iContext );
            CleanupStack::PushL( pifWithoutCtx );

            if ( iConfItem->ConfigurationPif().Contains( *pifWithoutCtx ) )
                {
                authorization.DoUpdatePresenceGrantPifForEveryoneL( *pifWithoutCtx, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }

            CleanupStack::PopAndDestroy( pifWithoutCtx );
            break;
            }

        case CPresenceToEveryoneConfigurationItem::ENotConfiguredAtAll:
            {
            // OPAA-73BCS8
            authorization.DoWithdrawPresenceGrantFromEveryoneL( iReqId );
            break;
            }
        case CPresenceToEveryoneConfigurationItem::EConfiguredForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }    
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromEveryone::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromEveryone::RequestCompletedL()
    {
    TRACE(_L("COperationWithdrawPresenceFromEveryone::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iConfigurationStatus )
            {
            case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxOnly:
            case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxAndOthers:
                {
                iConfItem->RemoveConfigurator( iContext );
                break;
                }
            
            case CPresenceToEveryoneConfigurationItem::ENotConfiguredAtAll:
            case CPresenceToEveryoneConfigurationItem::EConfiguredForOtherCtxOnly:
                {
                break;
                }
            
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromEveryone::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationWithdrawPresenceFromEveryone::Type() const
    {
    return NPresenceOps::EPrWithdrawPresenceFromEveryone;
    }

// ---------------------------------------------------------------------------
// COperationWithdrawPresenceFromEveryone::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationWithdrawPresenceFromEveryone::SynthesiseEventL()
    {   
    }



// End of file
