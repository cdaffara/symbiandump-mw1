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
* Description:  MXIMPRestrictedObjectCollection implementation
*
*/

#include "ximprestrictedobjectcollectionimp.h"

// ============================ MEMBER FUNCTIONS =============================


// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPRestrictedObjectCollectionImp*
    CXIMPRestrictedObjectCollectionImp::NewL( TBool aObjectOwning /*= ETrue*/ )
    {
    CXIMPRestrictedObjectCollectionImp* self = NewLC( aObjectOwning );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPRestrictedObjectCollectionImp*
    CXIMPRestrictedObjectCollectionImp::NewLC( TBool aObjectOwning /*= ETrue*/ )
    {
    CXIMPRestrictedObjectCollectionImp* self =
        new (ELeave) CXIMPRestrictedObjectCollectionImp( aObjectOwning );

    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::~CXIMPRestrictedObjectCollectionImp()
// ---------------------------------------------------------------------------
//
CXIMPRestrictedObjectCollectionImp::~CXIMPRestrictedObjectCollectionImp()
    {
    if( iOwnObjects )
        {
        iApiObjects.ResetAndDestroy();
        }
    else
        {
        iApiObjects.Reset();
        }    
    }


// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::CXIMPRestrictedObjectCollectionImp()
// ---------------------------------------------------------------------------
//
CXIMPRestrictedObjectCollectionImp::CXIMPRestrictedObjectCollectionImp( TBool aObjectOwning )
: iOwnObjects( aObjectOwning )
    {
    }


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPRestrictedObjectCollectionImp,
                                     MXIMPRestrictedObjectCollection )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPRestrictedObjectCollectionImp,
                                           MXIMPRestrictedObjectCollection )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::LookupByType()
// ---------------------------------------------------------------------------
//
void CXIMPRestrictedObjectCollectionImp::LookupByType(
    const MXIMPBase*& aObject,
    TInt32 aInterfaceId ) const
    {
    aObject = NULL;

    TInt index = iApiObjects.FindInOrder( aInterfaceId,
                                          OrderIfIdAndApiObject );
    if( index != KErrNotFound )
        {
        const CXIMPApiObjBase* theObject = iApiObjects[ index ];
        aObject = &theObject->Base();
        }
    }



// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::AddOrReplaceTypeL()
// ---------------------------------------------------------------------------
//
void CXIMPRestrictedObjectCollectionImp::AddOrReplaceTypeL(
    MXIMPBase* aObject )
    {
    CXIMPApiObjBase* theObject = TXIMPGetImpClassOrPanic< CXIMPApiObjBase >::From( *aObject );
                                                         

    TInt index = KErrNotFound;
    TInt entryExists = iApiObjects.FindInOrder( theObject,
                                                index,
                                                OrderApiObjects );

    if( entryExists == KErrNotFound )
        {
        iApiObjects.InsertL( theObject, index );
        }
    else
        {
        CXIMPApiObjBase* oldObject = iApiObjects[ index ];
        if( iOwnObjects )
            {
            delete oldObject;
            }        
        iApiObjects[ index ] = theObject;
        }
    }



// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::GetByType()
// ---------------------------------------------------------------------------
//
void CXIMPRestrictedObjectCollectionImp::GetByType(
    MXIMPBase*& aObject,
    TInt32 aInterfaceId )
    {
    aObject = NULL;

    TInt index = iApiObjects.FindInOrder( aInterfaceId,
                                          OrderIfIdAndApiObject );
    if( index != KErrNotFound )
        {
        CXIMPApiObjBase* theObject = iApiObjects[ index ];
        aObject = &theObject->Base();
        iApiObjects.Remove( index );
        }
    }



// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::OrderIfIdAndApiObject()
// ---------------------------------------------------------------------------
//
TInt CXIMPRestrictedObjectCollectionImp::OrderIfIdAndApiObject(
    const TInt32* aIfId,
    const CXIMPApiObjBase& aRhs )
    {
    return (*aIfId) - aRhs.Base().GetInterfaceId();
    }


// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::OrderApiObjects()
// ---------------------------------------------------------------------------
//
TInt CXIMPRestrictedObjectCollectionImp::OrderApiObjects(
    const CXIMPApiObjBase& aLhs,
    const CXIMPApiObjBase& aRhs )
    {
    const TInt32 interfaceId = aLhs.Base().GetInterfaceId();
    return OrderIfIdAndApiObject( &interfaceId, aRhs );
    }


// ---------------------------------------------------------------------------
// CXIMPRestrictedObjectCollectionImp::SetObjectOwnership()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPRestrictedObjectCollectionImp::SetObjectOwnership( TBool aObjectOwning /*= ETrue*/ )
    {
    iOwnObjects = aObjectOwning;
    }
// End of file
