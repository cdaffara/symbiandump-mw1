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
* Description:  MXIMPObjectCollection implementation
 *
*/

#include "ximpobjectcollectionimp.h"


// ============================ HELPER CLASS =============================

// ---------------------------------------------------------------------------
// CXIMPCollectionElement::NewL
// ---------------------------------------------------------------------------
//
CXIMPObjectCollectionImp::CXIMPCollectionElement* 
CXIMPObjectCollectionImp::CXIMPCollectionElement::NewL(
        CXIMPApiObjBase* aObject, const TDesC8& aName )
    {
    CXIMPCollectionElement* self = new ( ELeave ) CXIMPCollectionElement();
    CleanupStack::PushL( self );
    self->ConstructL( aObject, aName );
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CXIMPCollectionElement::ConstructL
// ---------------------------------------------------------------------------
//
void CXIMPObjectCollectionImp::CXIMPCollectionElement::ConstructL(
        CXIMPApiObjBase* aObject, const TDesC8& aName )
    {
    iObject = aObject;

    if ( aName.Length() > 0 )
        {
        iName = aName.AllocL();
        }
    else
        {
        // at least an empty one always exists
        iName = HBufC8::NewL( 0 );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPCollectionElement::CXIMPCollectionElement
// ---------------------------------------------------------------------------
//
CXIMPObjectCollectionImp::CXIMPCollectionElement::CXIMPCollectionElement()
    {
    }

// ---------------------------------------------------------------------------
// CXIMPCollectionElement::~CXIMPCollectionElement
// ---------------------------------------------------------------------------
//
CXIMPObjectCollectionImp::CXIMPCollectionElement::~CXIMPCollectionElement()
    {
    delete iObject;
    delete iName;
    }

// ---------------------------------------------------------------------------
// CXIMPCollectionElement::GetObject
// ---------------------------------------------------------------------------
//
CXIMPApiObjBase* CXIMPObjectCollectionImp::CXIMPCollectionElement::GetObject()
    {
    CXIMPApiObjBase* obj = iObject;
    iObject = NULL;
    return obj;
    }

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPObjectCollectionImp* CXIMPObjectCollectionImp::NewL()
    {
    CXIMPObjectCollectionImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPObjectCollectionImp* CXIMPObjectCollectionImp::NewLC()
    {
    CXIMPObjectCollectionImp* self = new ( ELeave ) CXIMPObjectCollectionImp;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::~CXIMPObjectCollectionImp()
// ---------------------------------------------------------------------------
//
CXIMPObjectCollectionImp::~CXIMPObjectCollectionImp()
    {
    iCollection.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::CXIMPObjectCollectionImp()
// ---------------------------------------------------------------------------
//
CXIMPObjectCollectionImp::CXIMPObjectCollectionImp()
    {
    }


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPObjectCollectionImp,
                                     MXIMPObjectCollection )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPObjectCollectionImp,
                                           MXIMPObjectCollection )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::LookupByType()
// ---------------------------------------------------------------------------
//
void CXIMPObjectCollectionImp::LookupByType(
        const MXIMPBase*& aObject,
        TInt32 aInterfaceId ) const
    {
    TInt start = FindLRUIndex( aObject );
    
    aObject = NULL;
    
    // now find the object without name
    for ( TInt i = start; i < iCollection.Count(); i++ )
        {
        const CXIMPObjectCollectionImp::CXIMPCollectionElement* element = iCollection[ i ];

        if ( ! element->iName->Length() )
            {
            // no name, ok
            if ( aInterfaceId == element->iObject->Base().GetInterfaceId() )
                {
                // got a match!
                aObject = &(element->iObject->Base());
                return;
                }
            }
        }

    return;
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::LookupByTypeAndName()
// ---------------------------------------------------------------------------
//
void CXIMPObjectCollectionImp::LookupByTypeAndName(
                const MXIMPBase*& aObject,
                TInt32 aInterfaceId,
                const TDesC8& aMatch ) const
    {
    TInt start = FindLRUIndex( aObject );

    aObject = NULL;

    // now find the object with name
    for ( TInt i = start; i < iCollection.Count(); i++ )
        {
        const CXIMPObjectCollectionImp::CXIMPCollectionElement* element = iCollection[ i ];

        if ( ( KErrNotFound != element->iName->MatchF( aMatch ) ) 
                && ( aInterfaceId == element->iObject->Base().GetInterfaceId() ) )
            {
            // got a match with correct name and interface id
            aObject = &(element->iObject->Base());
            return;
            }
        }

    return;
    }


// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::AddObjectL
// ---------------------------------------------------------------------------
//
void CXIMPObjectCollectionImp::AddObjectL(
        MXIMPBase* aObject )
    {
    AddObjectWithNameL( aObject, KNullDesC8 );
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::AddObjectWithNameL
// ---------------------------------------------------------------------------
//
void CXIMPObjectCollectionImp::AddObjectWithNameL(
        MXIMPBase* aObject,
        const TDesC8& aName )
    {
    // NULL is not accepted
    User::LeaveIfError( aObject == NULL ? KErrArgument : KErrNone );
    
    // name must not contain wildcards
    User::LeaveIfError( ValidateName( aName ) );

    CXIMPApiObjBase* base = ( CXIMPApiObjBase* ) 
        aObject->GetInterface( XIMPIMP_CLSID_CXIMPAPIOBJBASE, EPanicIfUnknown ); 
    CXIMPObjectCollectionImp::CXIMPCollectionElement* element = 
        CXIMPObjectCollectionImp::CXIMPCollectionElement::NewL( base, aName );
    CleanupStack::PushL( element );
    iCollection.AppendL( element );
    CleanupStack::Pop( element ); 
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::GetByType
// ---------------------------------------------------------------------------
//
TBool CXIMPObjectCollectionImp::GetByType(
        MXIMPBase*& aObject,
        TInt32 aInterfaceId )
    {
    aObject = NULL;

    // now find the object without name
    for ( TInt i = 0; i < iCollection.Count(); i++ )
        {
        CXIMPObjectCollectionImp::CXIMPCollectionElement* element = iCollection[ i ];

        if ( ! element->iName->Length() )
            {
            // no name, ok
            if ( aInterfaceId == element->iObject->Base().GetInterfaceId() )
                {
                // got a match!
                CXIMPApiObjBase* object = element->GetObject(); 
                iCollection.Remove( i );
                delete element;
                aObject = &(object->Base());
                return ETrue;
                }
            }
        }

    return EFalse;
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::GetByTypeAndName
// ---------------------------------------------------------------------------
//
TBool CXIMPObjectCollectionImp::GetByTypeAndName(
        MXIMPBase*& aObject,
        TInt32 aInterfaceId,
        const TDesC8& aMatch )
    {
    aObject = NULL;

    // now find the object with name
    for ( TInt i = 0; i < iCollection.Count(); i++ )
        {
        CXIMPObjectCollectionImp::CXIMPCollectionElement* element = iCollection[ i ];

        if ( ( KErrNotFound != element->iName->MatchF( aMatch ) ) 
                && ( aInterfaceId == element->iObject->Base().GetInterfaceId() ) )
            {
            // got a match with correct name and interface id
            CXIMPApiObjBase* object = element->GetObject(); 
            iCollection.Remove( i );
            delete element;
            aObject = &(object->Base());
            return ETrue;
            }
        }

    return EFalse;
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::FindLRUIndex()
// ---------------------------------------------------------------------------
//
TInt CXIMPObjectCollectionImp::FindLRUIndex( const MXIMPBase*& aObject ) const
    {
    TInt start = 0;

    // naive lookup, least recently used (from beginning to the end).
    // find starting position
    if ( aObject != NULL )
        {
        TBool found = EFalse;
        for ( TInt i = 0; ( i < iCollection.Count()-1 ) && ( ! found ); i++ )
            {
            const CXIMPObjectCollectionImp::CXIMPCollectionElement* element = iCollection[ i ];
            if ( aObject == &(element->iObject->Base()) )
                {
                found = ETrue;
                start = i;
                }
            }
        }

    return start;
    }

// ---------------------------------------------------------------------------
// CXIMPObjectCollectionImp::ValidateName()
// ---------------------------------------------------------------------------
//
TInt CXIMPObjectCollectionImp::ValidateName( const TDesC8& aName ) const
    {
    const TUint8* pName = aName.Ptr();

    TInt pNameLen = aName.Length();
    for( ; pNameLen; pName++, pNameLen-- )
        {
        if( *pName > 0x7e || 
            *pName < 0x20 || 
            *pName == '*' || 
            *pName == '?' || 
            *pName == ':' )
            {
            return KErrBadName;                
            }
        }

	return KErrNone;
	}



// End of file
