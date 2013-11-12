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
* Description:  Grant presence to presentity operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationgrantpresencetopresentity.h"
#include "presenceinfofilterimp.h"
#include "ximpidentityimp.h"
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
// COperationGrantPresenceToPresentity::COperationGrantPresenceToPresentity()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationGrantPresenceToPresentity::COperationGrantPresenceToPresentity()
    {
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentity::~COperationGrantPresenceToPresentity()
// ---------------------------------------------------------------------------
//
COperationGrantPresenceToPresentity::~COperationGrantPresenceToPresentity()
    {
    delete iPif;
    delete iIdentity;
    delete iMergedPif;
    
    if( iConfItem )
        {
        iConfItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentity::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentity::ConstructL( const TDesC8& aParamPck )
    {
    // Read params. PIF and identity    
    RDesReadStream rs;
    rs.Open( aParamPck ); // CSI: 65 # Does not return a value.
    CleanupClosePushL( rs );

    // group id
    iPif = CPresenceInfoFilterImp::NewLC();
    CleanupStack::Pop( iPif );
    iPif->InternalizeL( rs );

    // identity
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    iIdentity->InternalizeL( rs );
    
    CleanupStack::PopAndDestroy(); // rs
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentity::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentity::ProcessL()
    {
    TRACE(_L("COperationGrantPresenceToPresentity::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CPresenceConfigurationItem& confItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresenceConfigurationItemLC( *iIdentity );
    CleanupStack::Pop(); // subItem
    iConfItem = &confItem;

    // Get the subscription status
    iConfigurationStatus = iConfItem->ConfigurationStatus( iContext );

    MProtocolPresenceAuthorization& authorization = 
                            iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    switch( iConfigurationStatus )
        {
        case CPresenceConfigurationItem::ENotConfiguredAtAll:
        case CPresenceConfigurationItem::EConfiguredForCtxOnly:
            {
            iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( *iPif );
            authorization.DoGrantPresenceForPresentityL( iConfItem->Identity(),
                                                      *iMergedPif,
                                                      iReqId );
            break;
            }
        case CPresenceConfigurationItem::EConfiguredForOtherCtxOnly:
            {
            // Aggregate subscription and context pifs
            // Check aggregate and if needed.. update subscription
            iMergedPif = DocumentUtils::InfoFilterUnionL( iConfItem->ConfigurationPif(), *iPif );
            if( iMergedPif->Contains( iConfItem->ConfigurationPif() ) )
                {
                authorization.DoUpdatePresenceGrantPifForPresentityL( iConfItem->Identity(),
                                                                  *iMergedPif,
                                                                  iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
        //case CPresenceConfigurationItem::EConfiguredForCtxOnly:
        case CPresenceConfigurationItem::EConfiguredForCtxAndOthers:
            {
            CPresenceInfoFilterImp* oldPif = 
                iContext->PresenceInfoFilter( MXIMPPscContext::EGrantPresenceToPresentityFilter, iIdentity );
            CPresenceInfoFilterImp& oldMergedPif = iConfItem->ConfigurationPif();
            TBool makeUnion( EFalse );
            if( !oldPif )
                {
                TRACE(_L("COperationGrantPresenceToPresentity::ProcessL() no old pif" ) );
                makeUnion = ETrue;
                }
            else if ( *oldPif != *iPif ) 
                {
                makeUnion = ETrue;
                }
            if( makeUnion )
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
                authorization.DoUpdatePresenceGrantPifForPresentityL( *iIdentity, *iMergedPif, iReqId );
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
// COperationGrantPresenceToPresentity::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentity::RequestCompletedL()
    {
    TRACE(_L("COperationGrantPresenceToPresentity::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        if( iConfigurationStatus == CPresenceConfigurationItem::ENotSubscribedAtAll )
            {
            MXIMPBase* object = NULL; 
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            if( object )
                {
                delete iIdentity;
                iIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );

                // Got altered identity. If it differs from original
                // we'll create a new subscription item and close the old.
                CPresenceConfigurationItem& altSubItem = 
                    iMyHost->PresenceDataAccess().PresenceDataCache().PresenceConfigurationItemLC( *iIdentity );

                iConfItem->Close();
                iConfItem = &altSubItem;

                CleanupStack::Pop(); // altSubItem                    
                }            
            }
            
        if( iConfigurationStatus == CPresenceConfigurationItem::ENotSubscribedAtAll ||
            iConfigurationStatus == CPresenceConfigurationItem::ESubscribedForOtherCtxOnly )
            {
            iConfItem->AddConfiguratorL( iContext );        
            }
            
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EGrantPresenceToPresentityFilter,
                iPif, iIdentity );
        iPif = NULL;
        iConfItem->SetConfigurationPif( iMergedPif );
        iMergedPif = NULL;

        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentity::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationGrantPresenceToPresentity::Type() const
    {
    return NPresenceOps::EPrGrantPresenceToPresentity;
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentity::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentity::SynthesiseEventL()
    {   
    }



// End of file
