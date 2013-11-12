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
* Description:  MXIMPContextState API object implementation.
*
*/

#include "ximpcontextstateimp.h"



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextStateImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextStateImp* CXIMPContextStateImp::NewLC()
    {
    CXIMPContextStateImp* self = new( ELeave ) CXIMPContextStateImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextStateImp* CXIMPContextStateImp::NewL()
    {
    CXIMPContextStateImp* self = CXIMPContextStateImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }



// ---------------------------------------------------------------------------
// CXIMPContextStateImp::~CXIMPContextStateImp()
// ---------------------------------------------------------------------------
//
CXIMPContextStateImp::~CXIMPContextStateImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::CXIMPContextStateImp()
// ---------------------------------------------------------------------------
//
CXIMPContextStateImp::CXIMPContextStateImp()
    {
    iContextState = MXIMPContextState::EInactive;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextStateImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPContextStateImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPContextStateImp,
                                     MXIMPContextState )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPContextStateImp,
                                           MXIMPContextState )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPContextStateImp::ContextState()
// ---------------------------------------------------------------------------
MXIMPContextState::TState CXIMPContextStateImp::ContextState() const
    {
    return iContextState;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPContextStateImp::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iContextState );
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextStateImp::InternalizeL( RReadStream& aStream )
    {
    iContextState = (MXIMPContextState::TState) aStream.ReadInt32L();
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::SetStateValue()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextStateImp::SetStateValue( 
    MXIMPContextState::TState aState )
    {
    iContextState = aState;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPContextStateImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CXIMPContextStateImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPContextStateImp >::From( 
                                                        aOtherInstance.Base() );

    TBool totallySame = ETrue;
    TBool x;
    x = iContextState == tmp->iContextState;
    totallySame &= x;

    return totallySame;
    }


// End of file
