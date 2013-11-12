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

#ifndef CXIMPRESTRICTEDOBJECTCOLLECTIONIMP_H
#define CXIMPRESTRICTEDOBJECTCOLLECTIONIMP_H

#include <e32std.h>
#include <ximprestrictedobjectcollection.h>
#include "ximpapiobjbase.h"


/**
 *
 * @since S60 v3.2
 */
class CXIMPRestrictedObjectCollectionImp : public CXIMPApiObjBase,
                                           public MXIMPRestrictedObjectCollection
    {
public:

    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPRESTRICTEDOBJECTCOLLECTIONIMP };
    
public:
    
    /*
     * Construction of instance
     * @param aObjectOwning. ETrue objects are owned by instance.
     *                       EFalse objects are not owned by instance.
     *                       Default = ETrue
     */
    IMPORT_C static CXIMPRestrictedObjectCollectionImp* NewLC( TBool aObjectOwning = ETrue );

    /*
     * Construction of instance
     * @param aObjectOwning. ETrue objects are owned by instance.
     *                       EFalse objects are not owned by instance.
     *                       Default = ETrue
     */
    IMPORT_C static CXIMPRestrictedObjectCollectionImp* NewL( TBool aObjectOwning = ETrue );
    

public:

    /**
     * Destruction
     */
    ~CXIMPRestrictedObjectCollectionImp();


private:

    /**
     * Construction
     * @param aObjectOwning. ETrue objects are owned by instance.
     *                       EFalse objects are not owned by instance.
     */
    CXIMPRestrictedObjectCollectionImp( TBool aObjectOwning );


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: // From MXIMPRestrictedObjectCollection

    void LookupByType( const MXIMPBase*& aObject, TInt32 aInterfaceId ) const;
    void AddOrReplaceTypeL( MXIMPBase* aObject );
    void GetByType( MXIMPBase*& aObject, TInt32 aInterfaceId );



public: // New methods

    /**
     * Set object ownership
     * @param aObjectOwning. ETrue objects are owned by instance.
     *                       EFalse objects are not owned by instance.
     *                       Default = ETrue
     */
    IMPORT_C void SetObjectOwnership( TBool aObjectOwning = ETrue );

private: // Helpers
    static TInt OrderIfIdAndApiObject( const TInt32* aIfId, 
                                       const CXIMPApiObjBase& aRhs );

    static TInt OrderApiObjects( const CXIMPApiObjBase& aLhs, 
                                 const CXIMPApiObjBase& aRhs );



private: // data

    /**
     * Owning of objects. ETrue, Owns, EFalse does not own.
     */
    TBool iOwnObjects;

    /**
     * OWN: Check iOwnObjects
     */
    RXIMPObjOwningPtrArray< CXIMPApiObjBase > iApiObjects;
    
    };


#endif // CXIMPRESTRICTEDOBJECTCOLLECTIONIMP_H




