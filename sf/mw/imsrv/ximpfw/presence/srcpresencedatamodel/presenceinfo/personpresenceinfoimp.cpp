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
* Description:  Implementation of MPersonPresenceInfo object.
*
*/


#include "presenceinfofield.h"
#include "personpresenceinfoimp.h"
#include "presenceinfofieldcollectionimp.h"
#include "presenceinfofilterimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPersonPresenceInfoImp* CPersonPresenceInfoImp::NewLC()
    {
    CPersonPresenceInfoImp* self = new( ELeave ) CPersonPresenceInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPersonPresenceInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPersonPresenceInfoImp,
                                     MPersonPresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPersonPresenceInfoImp,
                                           MPersonPresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::~CPersonPresenceInfoImp()
// ---------------------------------------------------------------------------
//
CPersonPresenceInfoImp::~CPersonPresenceInfoImp()
    {
    delete iFields;
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::CPersonPresenceInfoImp()
// ---------------------------------------------------------------------------
//
CPersonPresenceInfoImp::CPersonPresenceInfoImp()
    {
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPersonPresenceInfoImp::ConstructL()
    {
    iFields = CPresenceInfoFieldCollectionImp::NewLC(); // CSI: 49 #
    CleanupStack::Pop( iFields );
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPersonPresenceInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iFields->ExternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPersonPresenceInfoImp::InternalizeL( RReadStream& aStream )
    {
    // we only have a parameterless ConstructL which creates the fields structure...
    CXIMPApiDataObjBase* tmp = CPresenceInfoFieldCollectionImp::NewFromStreamLC( aStream );
    CPresenceInfoFieldCollectionImp* fields = 
        TXIMPGetImpClassOrPanic< CPresenceInfoFieldCollectionImp >::From( tmp->Base() );
    delete iFields;
    iFields = fields;
    CleanupStack::Pop( tmp );
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPersonPresenceInfoImp::EqualsContent(
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPersonPresenceInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CPersonPresenceInfoImp >::From(
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = iFields->EqualsContent( *( tmp->iFields ) );
    same &= x;

    return same;
    }


// ---------------------------------------------------------------------------
// From MPersonPresenceInfo class.
// CPersonPresenceInfoImp::Fields()
// ---------------------------------------------------------------------------
//
const MPresenceInfoFieldCollection& CPersonPresenceInfoImp::Fields() const
    {
    return *iFields;
    }

// ---------------------------------------------------------------------------
// From MPersonPresenceInfo class.
// CPersonPresenceInfoImp::Fields()
// ---------------------------------------------------------------------------
//
MPresenceInfoFieldCollection& CPersonPresenceInfoImp::Fields()
    {
    return *iFields;
    }

// ---------------------------------------------------------------------------
// CPersonPresenceInfoImp::FilterWithL()
// ---------------------------------------------------------------------------
//
void CPersonPresenceInfoImp::FilterWithL( const CPresenceInfoFilterImp& aFilter )
    {
    TInt count = iFields->FieldCount();

    // must traverse backwards when removing
    for ( TInt i = count-1; i >= 0; i-- )
        {
        if ( ! aFilter.MatchPersonFilter( iFields->FieldAt( i ).FieldType() ) )
            {
            // no match, so remove the field
            // the filter tells us which ones to ACCEPT
            iFields->RemoveField( i );
            }
        }
    }

// End of file
