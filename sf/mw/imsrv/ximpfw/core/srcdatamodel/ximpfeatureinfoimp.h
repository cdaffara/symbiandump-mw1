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
* Description:  MXIMPFeatureInfo API object implementation.
*
*/

#ifndef CXIMPFEATUREINFOIMP_H
#define CXIMPFEATUREINFOIMP_H


#include "ximpapidataobjbase.h"
#include <ximpfeatureinfo.h>
#include <s32strm.h> // RWriteStream, RReadStream
#include <badesca.h> // CDesC8ArraySeg


/**
 * MXIMPFeatureInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPFeatureInfoImp ): public CXIMPApiDataObjBase,
                                          public MXIMPFeatureInfo
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPFEATUREINFOIMP };


public:

    IMPORT_C static CXIMPFeatureInfoImp* NewLC();
    IMPORT_C static CXIMPFeatureInfoImp* NewL();
    virtual ~CXIMPFeatureInfoImp();


private:

    CXIMPFeatureInfoImp();
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

public: // From MXIMPFeatureInfo

    /**
     * Implementation of MXIMPFeatureInfo interface methods
     * @see MXIMPFeatureInfo
     */
    const MDesC8Array& FeatureIds() const;
    TInt HasFeatureId( const TDesC8& aFeatureId );


public: // New methods

    /**
     * Adds a feature.
     *
     * @param aFeature Feature to be added.
     * @return KErrNone if the feature was added successfully,
     *         KErrAlreadyExists if the feature already exists,
     */
    IMPORT_C TInt AddFeatureL( const TDesC8& aFeature );

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


private: // helpers

    /**
     * Find a feature from features.
     *
     * @param [in] aFeature That is searched.
     * @param [out] aIndex If found the index position of the found feature,
     *                     if not found the place where it should be inserted.
     * @return KErrNone if the feature exists,
     *         KErrNotFound if the feature isn't found,
     */
    TInt FindFeature( const TDesC8& aFeature, TInt& aIndex );


private: // data

    /**
     * Supported features
     * Own.
     */
    CDesC8ArraySeg* iFeatures;

    };


#endif // CXIMPFEATUREINFOIMP_H
