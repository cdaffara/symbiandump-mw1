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
* Description:  XIMP Framework store for filters
*
*/


#include "presencefilterstore.h"
#include "ximpidentityimp.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"

// ======== LOCAL FUNCTIONS ========

// ============================ HELPER CLASS =============================

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::NewL
// ---------------------------------------------------------------------------
//
CPresenceFilterStore::CPresenceFilterStoreMapping* 
CPresenceFilterStore::CPresenceFilterStoreMapping::NewL()
    {
    CPresenceFilterStoreMapping* self = new ( ELeave ) 
        CPresenceFilterStoreMapping();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::ConstructL
// ---------------------------------------------------------------------------
//
void CPresenceFilterStore::CPresenceFilterStoreMapping::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::CPresenceFilterStoreMapping
// ---------------------------------------------------------------------------
//
CPresenceFilterStore::CPresenceFilterStoreMapping::CPresenceFilterStoreMapping() 
    {
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::~CPresenceFilterStoreMapping
// ---------------------------------------------------------------------------
//
CPresenceFilterStore::CPresenceFilterStoreMapping::~CPresenceFilterStoreMapping()
    {
    delete iIdentity;
    delete iFilter;
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::Identity
// ---------------------------------------------------------------------------
//
CXIMPIdentityImp& CPresenceFilterStore::CPresenceFilterStoreMapping::Identity()
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::Filter
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp& CPresenceFilterStore::CPresenceFilterStoreMapping::Filter()
    {
    return *iFilter;
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::SetPresenceInfoFilter
// ---------------------------------------------------------------------------
//
void CPresenceFilterStore::CPresenceFilterStoreMapping::SetPresenceInfoFilter( CPresenceInfoFilterImp* aFilter )
    {
    iFilter = aFilter;
    }
    
// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::SetIdentity
// ---------------------------------------------------------------------------
//
void CPresenceFilterStore::CPresenceFilterStoreMapping::SetIdentity( CXIMPIdentityImp* aIdentity )
    {
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::OrderOfMappings()
// ---------------------------------------------------------------------------
//
TInt CPresenceFilterStore::CPresenceFilterStoreMapping::OrderOfMappings( 
        const CPresenceFilterStoreMapping& aFirst, 
        const CPresenceFilterStoreMapping& aSecond )
    {
    return OrderOfMappingsByIdentity( aFirst.iIdentity, aSecond );
    }


// ---------------------------------------------------------------------------
// CPresenceFilterStoreMapping::OrderOfMappings()
// ---------------------------------------------------------------------------
//
TInt CPresenceFilterStore::CPresenceFilterStoreMapping::OrderOfMappingsByIdentity( 
        const CXIMPIdentityImp* aKey, 
        const CPresenceFilterStoreMapping& aSecond )
    {
    return aKey->Compare( *aSecond.iIdentity );
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPresenceFilterStore::CPresenceFilterStore()
// ---------------------------------------------------------------------------
//
CPresenceFilterStore::CPresenceFilterStore()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceFilterStore::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceFilterStore::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceFilterStore::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceFilterStore* CPresenceFilterStore::NewL()
    {
    CPresenceFilterStore* self = new( ELeave ) CPresenceFilterStore;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceFilterStore::~CPresenceFilterStore()
// ---------------------------------------------------------------------------
//
CPresenceFilterStore::~CPresenceFilterStore()
    {
    iMappings.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CPresenceFilterStore::AssociateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceFilterStore::AssociateL( 
            const CXIMPIdentityImp& aIdentity, 
            CPresenceInfoFilterImp* aFilter )
    {
    TLinearOrder< CPresenceFilterStore::CPresenceFilterStoreMapping > order( CPresenceFilterStore::CPresenceFilterStoreMapping::OrderOfMappings );

    if ( FilterForIdentity( aIdentity ) )
        {
        User::Leave( KErrAlreadyExists );
        }

    CXIMPIdentityImp* identity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aIdentity );
    CleanupStack::PushL( identity );
    CPresenceFilterStoreMapping* mapping = CPresenceFilterStoreMapping::NewL();
    mapping->SetIdentity( identity );
    CleanupStack::Pop( identity );
    CleanupStack::PushL( mapping );
    iMappings.InsertInOrderL( mapping, order );
    CleanupStack::Pop( mapping );
    mapping->SetPresenceInfoFilter( aFilter );
    }

// ---------------------------------------------------------------------------
// CPresenceFilterStore::DisassociateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceFilterStore::Disassociate( const CXIMPIdentityImp& aIdentity )
    {
    TInt index = iMappings.FindInOrder( aIdentity, CPresenceFilterStoreMapping::OrderOfMappingsByIdentity );
    if( index >= 0 )
        {
        CPresenceFilterStoreMapping* removed = iMappings[ index ];
        iMappings.Remove( index );
        delete removed;
        removed = NULL;
        }
    }
    
// ---------------------------------------------------------------------------
// CPresenceFilterStore::FilterForIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* CPresenceFilterStore::FilterForIdentity( 
        const CXIMPIdentityImp& aIdentity )
    {
    TInt index = iMappings.FindInOrder( aIdentity, CPresenceFilterStoreMapping::OrderOfMappingsByIdentity );

    if( index < 0 )
        {
        return NULL;
        }

    CPresenceFilterStoreMapping* entry = iMappings[ index ];
    return (entry->iFilter);
    }

// End of file
