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
* Description:  MPresenceInfoField API object implementation.
*
*/


#include "ximpbase.h"
#include "ximpapidataobjbase.h"
#include "presenceinfofield.h"
#include "presenceinfofieldimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"
#include "presenceapidataobjfactory.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFieldImp* CPresenceInfoFieldImp::NewLC(
    const TDesC8& aFieldName,
    CXIMPApiDataObjBase* aFieldValue )
    {
    CPresenceInfoFieldImp* self = new( ELeave ) CPresenceInfoFieldImp;
    CleanupStack::PushL( self );
    self->ConstructL( aFieldName, aFieldValue );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFieldImp* CPresenceInfoFieldImp::NewLC()
    {
    CPresenceInfoFieldImp* self = new( ELeave ) CPresenceInfoFieldImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoFieldImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoFieldImp, MPresenceInfoField )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoFieldImp, MPresenceInfoField )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::~CPresenceInfoFieldImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldImp::~CPresenceInfoFieldImp()
    {
    iFieldName.Close();
    delete iFieldValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::CPresenceInfoFieldImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldImp::CPresenceInfoFieldImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldImp::ConstructL( const TDesC8& aFieldName,
                                            CXIMPApiDataObjBase* aFieldValue )
    {
    // if you give NULL you shall suffer
    User::LeaveIfError( aFieldValue == NULL ? KErrArgument : KErrNone );

    iFieldValue = aFieldValue;
    iFieldName.CreateL( aFieldName );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldImp::ConstructL()
    {
    iFieldName.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf8Helper::ExternalizeL( iFieldName, aStream );
    CPresenceApiDataObjFactory::ExternalizeL( aStream, *iFieldValue );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFieldImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf8Helper::InternalizeL( iFieldName, aStream );

    CXIMPApiDataObjBase* tmp = CPresenceApiDataObjFactory::InternalizeLC( aStream );
    delete iFieldValue;
    iFieldValue = tmp;
    CleanupStack::Pop( tmp );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFieldImp::EqualsContent( 
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoFieldImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFieldImp >::From( 
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = 0 == iFieldName.Compare( tmp->iFieldName );
    same &= x;

    x = iFieldValue->EqualsContent( *( tmp->iFieldValue ) );
    same &= x;
    return same;
    }


// ---------------------------------------------------------------------------
// From MPresenceInfoField class.
// CPresenceInfoFieldImp::FieldType()
// ---------------------------------------------------------------------------
//
const TDesC8& CPresenceInfoFieldImp::FieldType() const
    {
    return iFieldName;
    }


// ---------------------------------------------------------------------------
// From MPresenceInfoField class.
// CPresenceInfoFieldImp::FieldValue()
// ---------------------------------------------------------------------------
//
const MXIMPBase& CPresenceInfoFieldImp::FieldValue() const
    {
    return iFieldValue->Base();
    }


// ---------------------------------------------------------------------------
// From MPresenceInfoField class.
// CPresenceInfoFieldImp::FieldValue()
// ---------------------------------------------------------------------------
//
MXIMPBase& CPresenceInfoFieldImp::FieldValue() 
    {
    return iFieldValue->Base();
    }

// ---------------------------------------------------------------------------
// From MPresenceInfoField class.
// CPresenceInfoFieldImp::SetFieldValue
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldImp::SetFieldValue( MXIMPBase* aFieldValue ) 
    {
    CXIMPApiDataObjBase* tmp =
        TXIMPGetImpClassOrPanic< CXIMPApiDataObjBase >::From( 
                *aFieldValue );

    delete iFieldValue;
    iFieldValue = tmp; 
    }

// ---------------------------------------------------------------------------
// From MPresenceInfoField class.
// CPresenceInfoFieldImp::SetFieldTypeL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldImp::SetFieldTypeL( const TDesC8& aFieldType )
    {
    HBufC8* newValue = aFieldType.AllocL();
    iFieldName.Close();
    iFieldName.Assign( newValue );
    }

// End of file
