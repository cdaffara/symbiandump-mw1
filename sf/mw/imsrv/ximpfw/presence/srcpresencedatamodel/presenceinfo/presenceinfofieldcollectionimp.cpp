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
* Description:  MPresenceInfoFieldValueText API object implementation.
 *
*/


#include "presenceinfofield.h"
#include "presenceinfofieldimp.h"
#include "presenceinfofieldcollectionimp.h"
#include "presenceapidataobjfactory.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"
#include "presencetypehelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFieldCollectionImp* CPresenceInfoFieldCollectionImp::NewLC()
    {
    CPresenceInfoFieldCollectionImp* self = new ( ELeave ) CPresenceInfoFieldCollectionImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::~CPresenceInfoFieldCollectionImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldCollectionImp::~CPresenceInfoFieldCollectionImp()
    {
    iFields.Close();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::CPresenceInfoFieldCollectionImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldCollectionImp::CPresenceInfoFieldCollectionImp()
    {
    // nothing
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldCollectionImp::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoFieldCollectionImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoFieldCollectionImp, MPresenceInfoFieldCollection )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoFieldCollectionImp, MPresenceInfoFieldCollection )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldCollectionImp::ExternalizeL( RWriteStream& aStream ) const
    {
    CPresenceApiDataObjFactory::ExternalizeL< CPresenceInfoFieldImp >
        ( aStream, iFields );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFieldCollectionImp::InternalizeL( RReadStream& aStream )
    {
    iFields.ResetAndDestroy();
    CPresenceApiDataObjFactory::InternalizeL< CPresenceInfoFieldImp >
        ( aStream, iFields );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFieldCollectionImp::EqualsContent( 
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoFieldCollectionImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFieldCollectionImp >::From( 
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;

    x = TXIMPEqualsContent< CPresenceInfoFieldImp >::Array( iFields, tmp->iFields );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::FieldCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldCollectionImp::FieldCount() const
    {
    return iFields.Count();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::FieldAt()
// ---------------------------------------------------------------------------
//
const MPresenceInfoField& CPresenceInfoFieldCollectionImp::FieldAt(
        TInt aIndex ) const
    {
    CPresenceInfoFieldImp* obj = iFields[ aIndex ];
    MPresenceInfoField* infoField = ( MPresenceInfoField* ) 
        obj->Base().GetInterface( MPresenceInfoField::KInterfaceId, 
                MXIMPBase::EPanicIfUnknown );
    return *infoField;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::LookupFieldByFieldType()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldCollectionImp::LookupFieldByFieldType(
            MPresenceInfoField*& aPresenceInfoField,
            const TDesC8& aMatch ) const
    {
    TInt start = FindIndexOfObj( aPresenceInfoField );
    start = start == KErrNotFound ? 0 : start;

    aPresenceInfoField = NULL;

    // now find the object with name
    for ( TInt i = start; i < iFields.Count(); i++ )
        {
        CPresenceInfoFieldImp* field = iFields[ i ];

        if ( KErrNotFound != field->FieldType().MatchF( aMatch ) )
            {
            // got a match with correct name
            aPresenceInfoField = &(*field);
            return;
            }
        }

    return;
    }
    
// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::DeleteFieldByFieldType()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldCollectionImp::DeleteFieldByFieldType(
            const TDesC8& aMatch )
    {
    TInt deleted(KErrNotFound);
    // now find the object with name
    for ( TInt i = 0; i < iFields.Count(); i++ )
        {
        CPresenceInfoFieldImp* field = iFields[ i ];

        if ( KErrNotFound != field->FieldType().MatchF( aMatch ) )
            {
            // got a match with correct name
            iFields.Remove( i );
            delete field;
            deleted = KErrNone;
            }
        }

    return deleted;
    }    

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::AddOrReplaceFieldL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldCollectionImp::AddOrReplaceFieldL(
            MPresenceInfoField* aPresenceInfoField )
    {
    User::LeaveIfError( aPresenceInfoField == NULL ? KErrArgument : KErrNone );

    TInt pos = FindIndexOfObj( aPresenceInfoField );
    CPresenceInfoFieldImp* obj = TXIMPGetImpClassOrPanic< CPresenceInfoFieldImp >::From( *aPresenceInfoField );

    if ( KErrNotFound == pos )
        {
        // not found, so just append
        iFields.AppendL( obj );
        }
    else
        {
        // found it, so replace the one at "pos"
        CPresenceInfoFieldImp* old = iFields[ pos ];
        iFields.Remove( pos );
        iFields.Insert( obj, pos );
        delete old;
        old = NULL;
        }
    }
    
    

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::FindIndexOfObj()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldCollectionImp::FindIndexOfObj( MPresenceInfoField*& aObject ) const
    {
    TInt start = KErrNotFound;

    // naive lookup, least recently used (from beginning to the end).
    // find starting position
    if ( aObject != NULL )
        {
        TBool found = EFalse;
        for ( TInt i = 0; ( i < iFields.Count()-1 ) && ( ! found ); i++ )
            {
            CPresenceInfoFieldImp* gotObj = TXIMPGetImpClassOrPanic< CPresenceInfoFieldImp >::From( *aObject );
            CPresenceInfoFieldImp* obj = iFields[ i ];
            if ( &(gotObj->Base()) == &(obj->Base()) )
                {
                found = ETrue;
                start = i;
                }
            }
        }

    return start;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldCollectionImp::RemoveFieldL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldCollectionImp::RemoveField( TInt aIndex )
    {
    CPresenceInfoFieldImp* field = iFields[ aIndex ];
    iFields.Remove( aIndex );
    delete field;
    }

// End of file
