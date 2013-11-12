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
* Description:  Implementation of presence info field collection.
 *
*/


#ifndef CPRESENCEINFOFIELDCOLLECTIONIMP_H
#define CPRESENCEINFOFIELDCOLLECTIONIMP_H

#include <e32std.h>
#include <s32strm.h> // RWriteStream, RReadStream
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include "presencetypehelpers.h"
#include "presenceinfofieldcollection.h"

class MPresenceInfoField;
class CPresenceInfoFilterImp;

/**
 * Implementation of presence info field collection.
 *
 *
 *
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoFieldCollectionImp ): public CXIMPApiDataObjBase, 
    public MPresenceInfoFieldCollection
    {
public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOFIELDCOLLECTIONIMP };

public:

    IMPORT_C static CPresenceInfoFieldCollectionImp* NewLC();

    virtual ~CPresenceInfoFieldCollectionImp();

private:

    CPresenceInfoFieldCollectionImp();

    void ConstructL();

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_PRIV_METHODS

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS

public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


public: // from MPresenceInfoFieldCollection

    /**
     * @see MPresenceInfoFieldCollection
     */
    TInt FieldCount() const;

    /**
     * @see MPresenceInfoFieldCollection
     */
    const MPresenceInfoField& FieldAt(
            TInt aIndex ) const;

    /**
     * @see MPresenceInfoFieldCollection
     */
    void LookupFieldByFieldType(
            MPresenceInfoField*& aPresenceInfoField,
            const TDesC8& aMatch ) const;
            
    /**
     * @see MPresenceInfoFieldCollection
     */
    TInt DeleteFieldByFieldType(const TDesC8& aMatch );

    /**
     * @see MPresenceInfoFieldCollection
     */
    void AddOrReplaceFieldL(
            MPresenceInfoField* aPresenceInfoField );

public: // new methods

    /**
     * Remove from this field collection the field
     * at the given index.
     */
    void RemoveField( TInt aIndex );

private: // helpers

    /** 
     * Find index, starting at the given object.
     * @return KErrNotFound if not found
     * @return Otherwise returns the index of given object
     */
    TInt FindIndexOfObj( MPresenceInfoField*& aObject ) const;

private: // data

    /**
     * OWN: The fields stored in this collection
     */
    RPrInfoFieldArray iFields;
    };

#endif // CPRESENCEINFOFIELDCOLLECTIONIMP_H



