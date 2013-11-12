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
* Description:  Object collection implementation.
 *
*/

#ifndef XIMPOBJECTCOLLECTIONIMP_H
#define XIMPOBJECTCOLLECTIONIMP_H

#include "ximpapiobjbase.h"
#include <e32std.h>
#include <ximpobjectcollection.h> 


/**
 * Object collection implementation.
 *
 * @see MXIMPObjectCollection
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class CXIMPObjectCollectionImp : public CXIMPApiObjBase,
    public MXIMPObjectCollection
    {

private: // helper class

    class CXIMPCollectionElement : public CBase
        {
        public:

            static CXIMPCollectionElement* NewL( CXIMPApiObjBase* aObject, const TDesC8& aName );
            ~CXIMPCollectionElement();

            /**
             * Get the stored object. Ownership transferred
             * to the caller.
             * @param The stored object
             */
            CXIMPApiObjBase* GetObject();

        private:

            CXIMPCollectionElement();
            void ConstructL( CXIMPApiObjBase* aObject, const TDesC8& aName );

        public:
            // owns: the object
            CXIMPApiObjBase* iObject;

            // owns: the name (always valid, but can be empty)
            HBufC8* iName;
        };

public:

    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPOBJECTCOLLECTIONIMP };

public:

    IMPORT_C static CXIMPObjectCollectionImp* NewLC();
    IMPORT_C static CXIMPObjectCollectionImp* NewL();

public:

    /**
     * Destruction
     */
    ~CXIMPObjectCollectionImp();


private:

    CXIMPObjectCollectionImp();


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: // from MXIMPObjectCollection

    // object lookup

    /**
     * @see MXIMPObjectCollection
     */
    void LookupByType(
            const MXIMPBase*& aObject,
            TInt32 aInterfaceId ) const; 

    /**
     * @see MXIMPObjectCollection
     */
    void LookupByTypeAndName(
                const MXIMPBase*& aObject,
                TInt32 aInterfaceId,
                const TDesC8& aMatch ) const;

    // set management

    /**
     * @see MXIMPObjectCollection
     */
    void AddObjectL(
                MXIMPBase* aObject );

    /**
     * @see MXIMPObjectCollection
     */
    void AddObjectWithNameL(
                MXIMPBase* aObject,
                const TDesC8& aName );


    /**
     * @see MXIMPObjectCollection
     */
    TBool GetByType(
                MXIMPBase*& aObject,
                TInt32 aInterfaceId );

    /**
     * @see MXIMPObjectCollection
     */
    TBool GetByTypeAndName(
                MXIMPBase*& aObject,
                TInt32 aInterfaceId,
                const TDesC8& aMatch );

private: // helpers

    /**
     * Find the index of the least recently used
     * object. Since the internal array is LIFO,
     * we walk the array forwards to find the index of 
     * least recently used object.
     * @param aObject The object from which to start
     * @return The index
     */
    TInt FindLRUIndex( const MXIMPBase*& aObject ) const;

    /**
     * Validate a given name. Checks that wildcard characters do not exist.
     * @return KErrNone if the name is OK, KErrBadName otherwise
     */
    TInt ValidateName( const TDesC8& aName ) const;

private: // data

    /**
     * OWN: Objects
     */
    RXIMPObjOwningPtrArray< CXIMPCollectionElement > iCollection;

    };


#endif // XIMPOBJECTCOLLECTIONIMP_H

