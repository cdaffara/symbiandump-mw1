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
* Description:  MXIMPFeatureInfo API object implementation.
*
*/

#include "ximpfeatureinfoimp.h"
#include "ximprbufhelpers.h"


const TInt KXIMPFeatureInfoGranularity = 5;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPFeatureInfoImp* CXIMPFeatureInfoImp::NewLC()
    {
    CXIMPFeatureInfoImp* self = new( ELeave ) CXIMPFeatureInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPFeatureInfoImp* CXIMPFeatureInfoImp::NewL()
    {
    CXIMPFeatureInfoImp* self = CXIMPFeatureInfoImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }



// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::~CXIMPFeatureInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPFeatureInfoImp::~CXIMPFeatureInfoImp()
    {
    if ( iFeatures )
        {
        iFeatures->Reset();
        }
    delete iFeatures;
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::CXIMPFeatureInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPFeatureInfoImp::CXIMPFeatureInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPFeatureInfoImp::ConstructL()
    {
    iFeatures = new ( ELeave ) CDesC8ArraySeg( KXIMPFeatureInfoGranularity );
    }

// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPFeatureInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPFeatureInfoImp, 
                                     MXIMPFeatureInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPFeatureInfoImp, 
                                           MXIMPFeatureInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::FeatureIds()
// ---------------------------------------------------------------------------
const MDesC8Array& CXIMPFeatureInfoImp::FeatureIds() const
    {
    return *iFeatures;
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::HasFeatureId()
// ---------------------------------------------------------------------------
TInt CXIMPFeatureInfoImp::HasFeatureId( const TDesC8& aFeatureId )
    {
    TInt index(0);
    return FindFeature( aFeatureId, index );
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::AddFeatureL()
// ---------------------------------------------------------------------------
EXPORT_C TInt CXIMPFeatureInfoImp::AddFeatureL( const TDesC8& aFeature )
    {
    // First check if the feature already exists
    TInt index( 0 );
    if ( FindFeature( aFeature, index ) == 0 )
        {
        return KErrAlreadyExists;
        }

    // Add to features
    iFeatures->InsertIsqL( aFeature );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::FindFeature()
// ---------------------------------------------------------------------------
TInt CXIMPFeatureInfoImp::FindFeature( const TDesC8& aFeature, TInt& aIndex )
    {
    if ( iFeatures->FindIsq( aFeature, aIndex ) != 0 )
        {
        return KErrNotFound;
        }

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPFeatureInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    const TInt featureCount = iFeatures->MdcaCount();
    aStream.WriteInt32L( featureCount );

    for ( TInt count = 0; count < featureCount; count++ )
        {
        TPtrC8 temp (iFeatures->MdcaPoint( count ) );
        aStream.WriteInt32L( temp.Length() );
        aStream.WriteL( temp );
        }
    }


// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPFeatureInfoImp::InternalizeL( RReadStream& aStream )
    {
    iFeatures->Reset();

    const TInt featureCount = aStream.ReadInt32L();

    RBuf8 temp;
    CleanupClosePushL( temp );

    for ( TInt count( 0 ); count < featureCount; count++ )
        {
        const TInt length = aStream.ReadInt32L();
        XIMPRBuf8Helper::GrowIfNeededL( temp, length );
        aStream.ReadL( temp, length );

        // Fine to simply append new entries, since we are internalizing
        // data which has been already validated and ordered
        iFeatures->AppendL( temp );
        }

    CleanupStack::PopAndDestroy(); // temp
    }

// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPFeatureInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CXIMPFeatureInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CXIMPFeatureInfoImp >::From( aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;

    x = iFeatures->MdcaCount() == tmp->iFeatures->MdcaCount();
    same &= x;

    if ( ! same )
        {
        // early-out if lengths mismatch to avoid costly array
        // comparison
        return EFalse;
        }

    // arrays must be in same order with same content.
    for ( TInt i = 0; i < iFeatures->Count(); i++ )
        {
        x = 0 == iFeatures->MdcaPoint( i ).Compare( tmp->iFeatures->MdcaPoint( i ) );
        same &= x;
        }

    return same;
    }

// End of file
