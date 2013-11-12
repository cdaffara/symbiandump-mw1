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
* Description:  Presence Service Connection group list subscription implementation.
*
*/


#include "presentitypresencesubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "presentitypresenceeventimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include <presencecachewriter.h>
#include <presencebuddyinfo.h>

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::CPresentityPresenceSubscriptionItem()
// ---------------------------------------------------------------------------
//
CPresentityPresenceSubscriptionItem::CPresentityPresenceSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceSubscriptionItem::ConstructL( const CXIMPIdentityImp& aIdentity )
    {
    BaseConstructL();
    
    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aIdentity );
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::NewLC()
// ---------------------------------------------------------------------------
//
CPresentityPresenceSubscriptionItem*
    CPresentityPresenceSubscriptionItem::NewLC( MXIMPItemParentBase& aParent, const CXIMPIdentityImp& aIdentity )
    {
    CPresentityPresenceSubscriptionItem* self =
        new( ELeave ) CPresentityPresenceSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL( aIdentity );
    return self;
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::~CPresentityPresenceSubscriptionItem()
// ---------------------------------------------------------------------------
//
CPresentityPresenceSubscriptionItem::~CPresentityPresenceSubscriptionItem()
    {
    // Remove it also from the external cache
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    if(presCache)
        {
            MPresenceCacheWriter* extCache = presCache->ExternalCache();
            if(extCache && iIdentity)
                TRAP_IGNORE( extCache->DeletePresenceL( *iIdentity ) );
            presCache->RemoveMe( this );
        }
    delete iIdentity;
    delete iPresenceInfo;
    delete iPif;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::SubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp&
            CPresentityPresenceSubscriptionItem::SubscriptionPif()
    {
    TRACE(_L("CPresentityPresenceSubscriptionItem::SubscriptionPif" ) );
    // assert removed
    return *iPif;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::SetSubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityPresenceSubscriptionItem::SetSubscriptionPif(
                                                CPresenceInfoFilterImp* aPif )
    {
    TRACE(_L("CPresentityPresenceSubscriptionItem::SetSubscriptionPif" ) );
    if ( !aPif )
        {
        TRACE(_L("CPresentityPresenceSubscriptionItem::SetSubscriptionPif aPif NULL" ) );
        }
    delete iPif;
    iPif = aPif;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp*
    CPresentityPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL(
                                                        MXIMPPscContext* aContext )
    {
    TRACE(_L("CPresentityPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL" ) );
    RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;
    CleanupClosePushL( arrayOfPifs );

    TInt count = iContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iContexts[ a ] != aContext )
            {
            CPresenceInfoFilterImp* filter = iContexts[ a ]->PresenceInfoFilter( 
                        MXIMPPscContext::EPresentityPresenceFilter, iIdentity );
            if ( filter )
                {
                arrayOfPifs.AppendL( filter );
                }
            }
        }
    CPresenceInfoFilterImp* pifWithoutCtx = DocumentUtils::InfoFilterUnionL( arrayOfPifs );
    CleanupStack::PopAndDestroy(); // arrayOfPifs
    return pifWithoutCtx;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::SetPresenceInfoL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityPresenceSubscriptionItem::SetPresenceInfoL(
                                            CPresenceInfoImp* aPresenceInfo )
    {
    TRACE(_L("CPresentityPresenceSubscriptionItem::SetPresenceInfoL" ) );
    delete iPresenceInfo;
    iPresenceInfo = aPresenceInfo;
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    
    // Now let's update the external presence cache
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    MPresenceCacheWriter* extCache = presCache->ExternalCache();
    MPresenceBuddyInfo* buddyPresence = extCache->NewBuddyPresenceInfoLC();
    
    // we need to clonce the parameters since the ownership is transfered
    CXIMPIdentityImp* identity = TXIMPObjectCloner<CXIMPIdentityImp>::CloneL(*iIdentity);
    buddyPresence->SetBuddyId( identity );
    
    CPresenceInfoImp* presenceInfo = TXIMPObjectCloner<CPresenceInfoImp>::CloneL(*iPresenceInfo);
    buddyPresence->SetPresenceInfo( presenceInfo );

    // No harm to do this, because the internal cache is still working
    // we just lose the external update
    TRAP_IGNORE( extCache->WritePresenceL( buddyPresence ) );

    CleanupStack::PopAndDestroy( 1 ); // buddyPresence
    
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresentityPresenceSubscriptionItem::Order(
                        const CPresentityPresenceSubscriptionItem& aA,
                        const CPresentityPresenceSubscriptionItem& aB )
    {
    return IdentityOrder( aA.iIdentity, aB );
    }
// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresentityPresenceSubscriptionItem::IdentityOrder(
                        const CXIMPIdentityImp* aA,
                        const CPresentityPresenceSubscriptionItem& aB )
    {
    return aA->Compare( *aB.iIdentity );
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::Identity()
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPIdentity& CPresentityPresenceSubscriptionItem::Identity() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::GroupIdentity()
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPIdentity* CPresentityPresenceSubscriptionItem::GroupIdentity() const
    {
    return iGroupIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::SetIdentity()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityPresenceSubscriptionItem::SetIdentity(
                                            CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::SetGroupIdentity()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityPresenceSubscriptionItem::SetGroupIdentity(
                                            CXIMPIdentityImp* aGroup )
    {
    delete iGroupIdentity;
    iGroupIdentity = aGroup;
    }

// ---------------------------------------------------------------------------
// From class MXIMPSubscriptionItem.
// CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL(
                                                    MXIMPPscContext* aContext,
                                                    TBool aForceEvent )
    {
    TRACE_1( _L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext ); // << status

    CPresentityPresenceEventImp* newEvent = NULL;
    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable )
        {
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 2" ) );
        __ASSERT_ALWAYS( iPresenceInfo, User::Panic( NXIMPPrivPanic::KCategory,
                                            NXIMPPrivPanic::EPresentityPresencePresenceInfoDoesNotExist ) );
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 3" ) );
		// ERIK TODO: Logic is the following:
		// 1 - Find all the groups where the iIdentity belongs to
		// 2 - Collect the filters from those groups
		// 3 - collect the filter from the individual subscription item also own
		// 4 - make a union of the filters
        
        CPresenceInfoFilterImp* groupPif = NULL;
        if ( iGroupIdentity ) 
            { 
            groupPif = aContext->PresenceInfoFilter( MXIMPPscContext::EPresentityGroupMemberPresenceFilter, iGroupIdentity );
            if ( !iPif )
                {
                TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL ownPif found" ) );
                iPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneLC( *groupPif );
                CleanupStack::Pop();
                }
            }
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 4_0" ) );
        CPresenceInfoFilterImp* ownPif = aContext->PresenceInfoFilter( MXIMPPscContext::EOwnPresenceFilter, iIdentity );
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 4_1" ) );
        CPresenceInfoFilterImp* presentityPif = aContext->PresenceInfoFilter( MXIMPPscContext::EPresentityPresenceFilter, iIdentity );
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 4_2" ) );
    	RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;    // << arrayOfPifs
    	CleanupClosePushL( arrayOfPifs );
    
        if ( groupPif )
            {
            TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL groupPif found" ) );
            arrayOfPifs.AppendL( groupPif );
            if ( !iPif )
                {
                TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL iPif created" ) );
                SetSubscriptionPif( groupPif );
                //iPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneLC( groupPif );
                //CleanupStack::Pop();
                }
            }
        if ( ownPif )
            {
            TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL ownPif found" ) );
            arrayOfPifs.AppendL( ownPif );
            }
        if ( presentityPif )
            {
            TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL presentityPif found" ) );
            arrayOfPifs.AppendL( presentityPif );
            }
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 4_3" ) );
		CPresenceInfoFilterImp* unionPif = NULL;
    	unionPif = DocumentUtils::InfoFilterUnionL( arrayOfPifs );    
    	if ( unionPif ) 
    	    {
    	    TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL unionPif found" ) );
    	    }
    	CleanupStack::PopAndDestroy(); // >>> arrayOfPifs
    	CleanupStack::PushL( unionPif );    // << unionPif

        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 5" ) );
        CPresenceInfoImp* filteredPresenceInfo = 
                DocumentUtils::CollectFilteredPresenceInfoLC(
                    *unionPif, 
                    *iPresenceInfo );   // << filteredPresenceInfo
        
        CleanupStack::Pop( filteredPresenceInfo ); // >> filteredPresenceInfo
        CleanupStack::PopAndDestroy( unionPif );    // >>> unionPif
        CleanupStack::PushL( filteredPresenceInfo );    // << filteredPresenceInfo
        TRACE(_L("CPresentityPresenceSubscriptionItem::SynthesiseSubscriptionEventToL 6" ) );
        newEvent = CPresentityPresenceEventImp::NewL( status ); // CSI: 35 # Event is put to cleanupstack in next branch.
        newEvent->SetPresenceInfo( filteredPresenceInfo );
        CleanupStack::Pop( filteredPresenceInfo ); // >> filteredPresenceInfo
        CleanupStack::Pop( status );    // >> status
        newEvent->SetIdentityL( *iIdentity );
        }
    else if( aForceEvent )
        {
        newEvent = CPresentityPresenceEventImp::NewL( status ); // CSI: 35 # Event is put to cleanupstack in next branch.
        CleanupStack::Pop( status );
        newEvent->SetIdentityL( *iIdentity );
        }

    if( newEvent )
        {
        CleanupStack::PushL( newEvent );
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
        presCache->AddEventL( *newEvent, aContext );
        }
    CleanupStack::PopAndDestroy(); //status || newEvent depending on branch
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceSubscriptionItem::CleanExpired()
    {

    }

// End of file
