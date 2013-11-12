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
* Description:  Grant presence to presentity group members operation
 *
*/

 
#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationgrantpresencetopresentitygroupmembers.h"
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
// COperationGrantPresenceToPresentityGroupMembers::COperationGrantPresenceToPresentityGroupMembers()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationGrantPresenceToPresentityGroupMembers::COperationGrantPresenceToPresentityGroupMembers()
    {
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentityGroupMembers::~COperationGrantPresenceToPresentityGroupMembers()
// ---------------------------------------------------------------------------
//
COperationGrantPresenceToPresentityGroupMembers::~COperationGrantPresenceToPresentityGroupMembers()
    {
    delete iPif;
    delete iGroupId;
    delete iMergedPif;
    
    if( iConfItem )
        {
        iConfItem->Close();
        }    
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentityGroupMembers::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentityGroupMembers::ConstructL( const TDesC8& aParamPck )
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
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    iGroupId->InternalizeL( rs );
    
    CleanupStack::PopAndDestroy(); // rs    
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentityGroupMembers::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentityGroupMembers::ProcessL()
    {
    TRACE(_L("COperationGrantPresenceToPresentityGroupMembers::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CGroupMembersPresenceConfigurationItem& confItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().GroupMembersPresenceConfigurationItemLC( *iGroupId );
    CleanupStack::Pop(); // subItem
    iConfItem = &confItem;

    iConfigurationStatus = iConfItem->ConfigurationStatus( iContext );

    MProtocolPresenceAuthorization& authorization = 
                        iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    switch( iConfigurationStatus )
        {
        case CGroupMembersPresenceConfigurationItem::ENotConfiguredAtAll:
            {
            iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( *iPif );
            authorization.DoGrantPresenceForPresentityGroupMembersL(  *iGroupId,
                                                                   *iMergedPif,
                                                                   iReqId );
            break;
            }
        case CGroupMembersPresenceConfigurationItem::EConfiguredForOtherCtxOnly:
            {
            // Aggregate subscription and context pifs
            // Check aggregate and if needed.. update subscription
            iMergedPif = DocumentUtils::InfoFilterUnionL( iConfItem->ConfigurationPif(), *iPif );
            if( iMergedPif->Contains( iConfItem->ConfigurationPif() ) )
                {
                authorization.DoUpdatePresenceGrantPifForPresentityGroupMembersL( *iGroupId,
                                                                                  *iMergedPif,
                                                                                  iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
        case CGroupMembersPresenceConfigurationItem::EConfiguredForCtxOnly:
        case CGroupMembersPresenceConfigurationItem::EConfiguredForCtxAndOthers:
            {
            CPresenceInfoFilterImp* oldPif = 
                iContext->PresenceInfoFilter( MXIMPPscContext::EGrantPresenceToGroupMemberFilter, iGroupId );
            CPresenceInfoFilterImp& oldMergedPif = iConfItem->ConfigurationPif();
            TBool makeUnion( EFalse );
            if( !oldPif )
                {
                TRACE(_L("COperationGrantPresenceToPresentityGroupMembers::ProcessL() no old pif" ) );
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
                authorization.DoUpdatePresenceGrantPifForPresentityGroupMembersL( *iGroupId, 
                                                                                  *iMergedPif, 
                                                                                  iReqId );
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
// COperationGrantPresenceToPresentityGroupMembers::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentityGroupMembers::RequestCompletedL()
    {
    TRACE(_L("COperationGrantPresenceToPresentityGroupMembers::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        
        if( iConfigurationStatus == CGroupMembersPresenceConfigurationItem::ENotSubscribedAtAll )
            {
            MXIMPBase* object = NULL; 
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            if( object )
                {
                delete iGroupId;
                iGroupId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );

                // Got altered identity. If it differs from original
                // we'll create a new subscription item and close the old.
                CGroupMembersPresenceConfigurationItem& altSubItem = 
                    iMyHost->PresenceDataAccess().PresenceDataCache().GroupMembersPresenceConfigurationItemLC( *iGroupId );

                iConfItem->Close();
                iConfItem = &altSubItem;

                CleanupStack::Pop(); // altSubItem                    
                }            
            }
            
        if( iConfigurationStatus == CGroupMembersPresenceConfigurationItem::ENotSubscribedAtAll ||
            iConfigurationStatus == CGroupMembersPresenceConfigurationItem::ESubscribedForOtherCtxOnly )
            {
            iConfItem->AddConfiguratorL( iContext );        
            }
            
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EGrantPresenceToGroupMemberFilter,
                iPif, iGroupId );
        iPif = NULL;
        iConfItem->SetConfigurationPif( iMergedPif );
        iMergedPif = NULL;

        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentityGroupMembers::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationGrantPresenceToPresentityGroupMembers::Type() const
    {
    return NPresenceOps::EPrGrantPresenceToPresentityGroupMembers;
    }

// ---------------------------------------------------------------------------
// COperationGrantPresenceToPresentityGroupMembers::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationGrantPresenceToPresentityGroupMembers::SynthesiseEventL()
    {   
    }



// End of file
