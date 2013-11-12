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
* Description:  Implementation of MDevicePresenceInfo object.
*
*/


#include "presenceinfofield.h"
#include "devicepresenceinfoimp.h"
#include "presenceinfofieldcollectionimp.h"
#include "ximprbufhelpers.h"
#include "presenceinfofilterimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CDevicePresenceInfoImp* CDevicePresenceInfoImp::NewLC()
    {
    CDevicePresenceInfoImp* self = new( ELeave ) CDevicePresenceInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CDevicePresenceInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CDevicePresenceInfoImp,
                                     MDevicePresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CDevicePresenceInfoImp,
                                           MDevicePresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::~CDevicePresenceInfoImp()
// ---------------------------------------------------------------------------
//
CDevicePresenceInfoImp::~CDevicePresenceInfoImp()
    {
    delete iFields;
    iDeviceName.Close();
    }


// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::CDevicePresenceInfoImp()
// ---------------------------------------------------------------------------
//
CDevicePresenceInfoImp::CDevicePresenceInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CDevicePresenceInfoImp::ConstructL()
    {
    iFields = CPresenceInfoFieldCollectionImp::NewLC();
    CleanupStack::Pop( iFields );
    
    iDeviceName.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CDevicePresenceInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf8Helper::ExternalizeL( iDeviceName, aStream );
    iFields->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CDevicePresenceInfoImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf8Helper::InternalizeL( iDeviceName, aStream );

    // we only have a parameterless ConstructL which creates the fields structure...
    CXIMPApiDataObjBase* tmp = CPresenceInfoFieldCollectionImp::NewFromStreamLC( aStream );
    CPresenceInfoFieldCollectionImp* fields = 
        TXIMPGetImpClassOrPanic< CPresenceInfoFieldCollectionImp >::From( tmp->Base() );
    delete iFields;
    iFields = fields;
    CleanupStack::Pop( tmp );
    }


// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CDevicePresenceInfoImp::EqualsContent(
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CDevicePresenceInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CDevicePresenceInfoImp >::From(
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = iFields->EqualsContent( *( tmp->iFields ) );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// From MDevicePresenceInfo class.
// CDevicePresenceInfoImp::DeviceName()
// ---------------------------------------------------------------------------
//
const TPtrC8 CDevicePresenceInfoImp::DeviceName() const
    {
    return iDeviceName;
    }

// ---------------------------------------------------------------------------
// From MDevicePresenceInfo class.
// CDevicePresenceInfoImp::SetDeviceNameL()
// ---------------------------------------------------------------------------
//
void CDevicePresenceInfoImp::SetDeviceNameL( const TDesC8& aDeviceName ) 
    {
    HBufC8* tmp = aDeviceName.AllocL();
    iDeviceName.Close();
    iDeviceName.Assign( tmp );
    }


// ---------------------------------------------------------------------------
// From MDevicePresenceInfo class.
// CDevicePresenceInfoImp::Fields()
// ---------------------------------------------------------------------------
//
const MPresenceInfoFieldCollection& CDevicePresenceInfoImp::Fields() const
    {
    return *iFields;
    }


// ---------------------------------------------------------------------------
// From MDevicePresenceInfo class.
// CDevicePresenceInfoImp::Fields()
// ---------------------------------------------------------------------------
//
MPresenceInfoFieldCollection& CDevicePresenceInfoImp::Fields()
    {
    return *iFields;
    }

// ---------------------------------------------------------------------------
// CDevicePresenceInfoImp::FilterWithL()
// ---------------------------------------------------------------------------
//
void CDevicePresenceInfoImp::FilterWithL( const CPresenceInfoFilterImp& aFilter )
    {
    TInt count = iFields->FieldCount();

    // must traverse backwards when removing
    for ( TInt i = count-1; i >= 0; i-- )
        {
        if ( ! aFilter.MatchDeviceFilterL( 
                    iDeviceName, 
                    iFields->FieldAt( i ).FieldType() ) )
            {
            // no match, so remove the field
            // the filter tells us which ones to ACCEPT
            iFields->RemoveField( i );
            }
        }
    }

// End of file
