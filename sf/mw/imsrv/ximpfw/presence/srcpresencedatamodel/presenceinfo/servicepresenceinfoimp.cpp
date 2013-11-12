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
* Description:  Implementation of MServicePresenceInfo object.
*
*/


#include "presenceinfofield.h"
#include "servicepresenceinfoimp.h"
#include "presenceinfofieldcollectionimp.h"
#include "ximprbufhelpers.h"
#include "presenceinfofilterimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CServicePresenceInfoImp* CServicePresenceInfoImp::NewLC()
    {
    CServicePresenceInfoImp* self = new( ELeave ) CServicePresenceInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CServicePresenceInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CServicePresenceInfoImp,
                                     MServicePresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CServicePresenceInfoImp,
                                           MServicePresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::~CServicePresenceInfoImp()
// ---------------------------------------------------------------------------
//
CServicePresenceInfoImp::~CServicePresenceInfoImp()
    {
    delete iFields;
    iServiceType.Close();
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::CServicePresenceInfoImp()
// ---------------------------------------------------------------------------
//
CServicePresenceInfoImp::CServicePresenceInfoImp()
    {
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CServicePresenceInfoImp::ConstructL()
    {
    iFields = CPresenceInfoFieldCollectionImp::NewLC();
    CleanupStack::Pop( iFields );

    iServiceType.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CServicePresenceInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf8Helper::ExternalizeL( iServiceType, aStream );
    iFields->ExternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CServicePresenceInfoImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf8Helper::InternalizeL( iServiceType, aStream );
    
    // we only have a parameterless ConstructL which creates the fields structure...
    CXIMPApiDataObjBase* tmp = CPresenceInfoFieldCollectionImp::NewFromStreamLC( aStream );
    CPresenceInfoFieldCollectionImp* fields = 
        TXIMPGetImpClassOrPanic< CPresenceInfoFieldCollectionImp >::From( tmp->Base() );
    delete iFields;
    iFields = fields;
    CleanupStack::Pop( tmp );
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CServicePresenceInfoImp::EqualsContent(
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CServicePresenceInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CServicePresenceInfoImp >::From(
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;

    x = iFields->EqualsContent( *( tmp->iFields ) );
    same &= x;

    return same;
    }


// ---------------------------------------------------------------------------
// From MServicePresenceInfo class.
// CServicePresenceInfoImp::ServiceType()
// ---------------------------------------------------------------------------
//
const TPtrC8 CServicePresenceInfoImp::ServiceType() const
    {
    return iServiceType;
    }

// ---------------------------------------------------------------------------
// From MServicePresenceInfo class.
// CServicePresenceInfoImp::SetServiceTypeL()
// ---------------------------------------------------------------------------
//
void CServicePresenceInfoImp::SetServiceTypeL( const TDesC8& aServiceType ) 
    {
    HBufC8* tmp = aServiceType.AllocL();
    iServiceType.Close();
    iServiceType.Assign( tmp );
    }


// ---------------------------------------------------------------------------
// From MServicePresenceInfo class.
// CServicePresenceInfoImp::Fields()
// ---------------------------------------------------------------------------
//
const MPresenceInfoFieldCollection& CServicePresenceInfoImp::Fields() const
    {
    return *iFields;
    }


// ---------------------------------------------------------------------------
// From MServicePresenceInfo class.
// CServicePresenceInfoImp::Fields()
// ---------------------------------------------------------------------------
//
MPresenceInfoFieldCollection& CServicePresenceInfoImp::Fields()
    {
    return *iFields;
    }

// ---------------------------------------------------------------------------
// CServicePresenceInfoImp::FilterWithL()
// ---------------------------------------------------------------------------
//
void CServicePresenceInfoImp::FilterWithL( const CPresenceInfoFilterImp& aFilter )
    {
    TInt count = iFields->FieldCount();

    // must traverse backwards when removing
    for ( TInt i = count-1; i >= 0; i-- )
        {
        if ( ! aFilter.MatchServiceFilterL( 
                    iServiceType, 
                    iFields->FieldAt( i ).FieldType() ) )
            {
            // no match, so remove the field
            // the filter tells us which ones to ACCEPT
            iFields->RemoveField( i );
            }
        }
    }


// End of file
