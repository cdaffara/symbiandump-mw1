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
* Description:  Grant presence to everyone
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationgrantpresencetoeveryone.h"
#include "presenceinfofilterimp.h"
#include "presencedatacacheimp.h"
#include "ximpobjecthelpers.h"
#include "documentutils.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "ximppanics.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::COperationGrantPresenceToEveryone()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationGrantPresenceToEveryone::COperationGrantPresenceToEveryone()
    {
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::~COperationGrantPresenceToEveryone()
// ---------------------------------------------------------------------------
//
COperationGrantPresenceToEveryone::~COperationGrantPresenceToEveryone()
    {
    delete iPif;
    delete iMergedPif;
    
    if( iConfItem )
        {
        iConfItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToEveryone::ConstructL( const TDesC8& aParamPck )
    {
    // group id
    iPif = CPresenceInfoFilterImp::NewLC();
    CleanupStack::Pop( iPif );
    TXIMPObjectPacker< CPresenceInfoFilterImp >::UnPackL( *iPif, aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToEveryone::ProcessL()
    {
    TRACE(_L("COperationGrantPresenceToEveryone::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CPresenceToEveryoneConfigurationItem& confItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresenceToEveryoneConfigurationItemLC();
    CleanupStack::Pop(); // subItem
    iConfItem = &confItem;

    // Get the subscription status
    iConfigurationStatus = iConfItem->ConfigurationStatus( iContext );

    MProtocolPresenceAuthorization& authorization = 
                            iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    switch( iConfigurationStatus )
        {
        case CPresenceToEveryoneConfigurationItem::ENotConfiguredAtAll:
            {
            iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( *iPif );
            authorization.DoGrantPresenceForEveryoneL( *iMergedPif, iReqId );
            break;
            }
        case CPresenceToEveryoneConfigurationItem::EConfiguredForOtherCtxOnly:
            {
            // Aggregate subscription and context pifs
            // Check aggregate and if needed.. update subscription
            iMergedPif = DocumentUtils::InfoFilterUnionL( iConfItem->ConfigurationPif(), *iPif );
            if( iMergedPif->Contains( iConfItem->ConfigurationPif() ) )
                {
                authorization.DoUpdatePresenceGrantPifForEveryoneL( *iMergedPif, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }

        case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxOnly:
        case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxAndOthers:
            {
            CPresenceInfoFilterImp* oldPif = 
                    iContext->PresenceInfoFilter( MXIMPPscContext::EGrantPresenceToEveryoneFilter );
            CPresenceInfoFilterImp& oldMergedPif = iConfItem->ConfigurationPif();
            if( *oldPif != *iPif )
                {
                iDoNotForce = ETrue;
                CPresenceInfoFilterImp* withoutPif = 
                            iConfItem->CollectConfigurationPifWithoutCtxL( iContext );
                iMergedPif = DocumentUtils::InfoFilterUnionL( *withoutPif, *iPif );
                }
            else
                {
                iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( oldMergedPif );
                }
                
            if( iMergedPif->Contains( oldMergedPif ) )
                {
                iDoNotForce = ETrue;
                authorization.DoUpdatePresenceGrantPifForEveryoneL( *iMergedPif, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToEveryone::RequestCompletedL()
    {
    TRACE(_L("COperationGrantPresenceToEveryone::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iConfigurationStatus )
            {
            case CPresenceToEveryoneConfigurationItem::ENotConfiguredAtAll:
            case CPresenceToEveryoneConfigurationItem::EConfiguredForOtherCtxOnly:
                {
                iContext->SetPresenceInfoFilterL( MXIMPPscContext::EGrantPresenceToEveryoneFilter, iPif );
                iPif = NULL;
                iConfItem->SetConfigurationPif( iMergedPif );
                iMergedPif = NULL;
                iConfItem->AddConfiguratorL( iContext );

                break;
                }
            case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxOnly:
            case CPresenceToEveryoneConfigurationItem::EConfiguredForCtxAndOthers:
                {
                break;
                }
            
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }

        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationGrantPresenceToEveryone::Type() const
    {
    return NPresenceOps::EPrGrantPresenceToEveryone;
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToEveryone::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToEveryone::SynthesiseEventL()
    {   
    }



// End of file
