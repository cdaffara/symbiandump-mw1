/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Msearchkeyinfo API object implementation.
*
*/

#include "searchkeyinfoimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSupportedSearchKeyImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchKeyInfoImp* CSearchKeyInfoImp::NewLC()
    {
    CSearchKeyInfoImp* self = new( ELeave ) CSearchKeyInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchKeyInfoImp* CSearchKeyInfoImp::NewL()
    {
    CSearchKeyInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CSearchKeyInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchKeyInfoImp, MSearchKeyInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchKeyInfoImp, MSearchKeyInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::~CSearchKeyInfoImp()
// ---------------------------------------------------------------------------
//
CSearchKeyInfoImp::~CSearchKeyInfoImp()
    {
    iLabel.Close();
    }

// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::CSearchKeyInfoImp()
// ---------------------------------------------------------------------------
//
CSearchKeyInfoImp::CSearchKeyInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchKeyInfoImp::ConstructL()
    {
    iLabel.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CSearchKeyInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iKeyType );
    aStream.WriteInt32L( iKey );
    XIMPRBuf16Helper::ExternalizeL( iLabel, aStream );
    }

// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchKeyInfoImp::InternalizeL( RReadStream& aStream )
    {
    iKeyType = aStream.ReadInt32L(); 
    iKey = aStream.ReadInt32L(); 
    XIMPRBuf16Helper::InternalizeL( iLabel, aStream );
    }
  


// ---------------------------------------------------------------------------
// 
// CSearchKeyInfoImp::Key()
// ---------------------------------------------------------------------------
//
TSearchKey CSearchKeyInfoImp::Key() const
    {
    return (TSearchKey)iKey;
    }
    


// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::SetKey
// ---------------------------------------------------------------------------
//
void CSearchKeyInfoImp::SetKey(TSearchKey aKey) 
    
    {
    iKey = aKey;
    }
// ---------------------------------------------------------------------------
// 
// CSearchKeyInfoImp::Key()
// ---------------------------------------------------------------------------
//
TKeyType CSearchKeyInfoImp::Type() const
    {
    return (TKeyType)iKeyType;
    }
    


// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::SetKey
// ---------------------------------------------------------------------------
//
void CSearchKeyInfoImp::SetType(TKeyType  aType) 
    
    {
    iKeyType = aType;
    }

// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::Label()
// ---------------------------------------------------------------------------
//
const TDesC16& CSearchKeyInfoImp::Label() const
    {
    return iLabel;
    }
 
// ---------------------------------------------------------------------------.
// CSearchKeyInfoImp::SetLabelL()
// ---------------------------------------------------------------------------
//
void CSearchKeyInfoImp::SetLabelL(const TDesC16& aLabel) 
    
    {
    iLabel.Close();
    iLabel.Assign(  aLabel.AllocL() );
    }
// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CSearchKeyInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& /*aOtherInstance*/ ) const
    {
    return ETrue; 
    }
// ---------------------------------------------------------------------------
// CSearchKeyInfoImp::LinearOrder
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSearchKeyInfoImp::LinearOrder(
        const CSearchKeyInfoImp& /*aA*/,
       const CSearchKeyInfoImp& /*aB*/ )
    {
    // Just returns 1 so that the Keys are appended as they come from server.
    return 1;
    }
    

// End of file
