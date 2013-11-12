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
* Description:  MXIMPProtocolInfo API object implementation.
*
*/

#include "ximpprotocolinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpfeatureinfoimp.h"
#include "ximprbufhelpers.h"



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPProtocolInfoImp* CXIMPProtocolInfoImp::NewLC()
    {
    CXIMPProtocolInfoImp* self = new( ELeave ) CXIMPProtocolInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPProtocolInfoImp* CXIMPProtocolInfoImp::NewLC(
                                        const TDesC16& aDisplayName,
                                        TUid aImplementationUid,
                                        const CXIMPFeatureInfoImp& aFeatures )
    {
    CXIMPProtocolInfoImp* self = new( ELeave ) CXIMPProtocolInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL( aDisplayName, aImplementationUid, aFeatures );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::~CXIMPProtocolInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPProtocolInfoImp::~CXIMPProtocolInfoImp()
    {
    iDisplayName.Close();
    delete iFeatures;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::CXIMPProtocolInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPProtocolInfoImp::CXIMPProtocolInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPProtocolInfoImp::ConstructL( const TDesC16& aDisplayName,
                                       TUid aImplementationUid,
                                       const CXIMPFeatureInfoImp& aFeatures )
    {
    iDisplayName.CreateL( aDisplayName );
    iImplementationUid = aImplementationUid;
    iFeatures = TXIMPObjectCloner< CXIMPFeatureInfoImp >::CloneL( aFeatures );
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPProtocolInfoImp::ConstructL()
    {
    // Initialize the RBufs to zero size. Later on realloc as needed.
    iDisplayName.CreateL( 0 );
    iFeatures = CXIMPFeatureInfoImp::NewL();
    }

// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPProtocolInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPProtocolInfoImp, 
                                     MXIMPProtocolInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPProtocolInfoImp, 
                                           MXIMPProtocolInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::DisplayName()
// ---------------------------------------------------------------------------
const TDesC16& CXIMPProtocolInfoImp::DisplayName() const
    {
    return iDisplayName;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::ImplementationUid()
// ---------------------------------------------------------------------------
TUid CXIMPProtocolInfoImp::ImplementationUid() const
    {
    return iImplementationUid;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::GetProtocolFeaturesLC()
// ---------------------------------------------------------------------------
MXIMPFeatureInfo* CXIMPProtocolInfoImp::GetProtocolFeaturesLC() const
    {
    return TXIMPObjectCloner< CXIMPFeatureInfoImp >::CloneLC( *iFeatures );
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPProtocolInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf16Helper::ExternalizeL( iDisplayName, aStream );
    aStream.WriteInt32L( iImplementationUid.iUid );

    iFeatures->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPProtocolInfoImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf16Helper::InternalizeL( iDisplayName, aStream );
    iImplementationUid.iUid = aStream.ReadInt32L();

    iFeatures->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// CXIMPProtocolInfoImp::EqualsContent
// ---------------------------------------------------------------------------
//
TBool CXIMPProtocolInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const 
    {
    const CXIMPProtocolInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPProtocolInfoImp >::From( 
                                                       aOtherInstance.Base() );


    TBool same = ETrue;
    TBool x;
    x = 0 == iDisplayName.Compare( tmp->iDisplayName );
    same &= x;

    x = iImplementationUid == tmp->iImplementationUid;
    same &= x;

    x = iFeatures->EqualsContent( *( tmp->iFeatures ) );
    same &= x;

    return same;
    }


// End of file
