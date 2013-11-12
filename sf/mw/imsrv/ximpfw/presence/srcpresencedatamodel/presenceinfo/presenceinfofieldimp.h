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
* Description:  Implementation of presence info field object.
*
*/

#ifndef CPRESENCEINFOFIELDIMP_H
#define CPRESENCEINFOFIELDIMP_H

#include <e32std.h>
#include <s32strm.h> // RWriteStream, RReadStream
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include "presenceinfofield.h"

/**
 * Implementation of presence info field object.
 *
 *
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoFieldImp ): public CXIMPApiDataObjBase, 
                                                public MPresenceInfoField
    {
    public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOFIELDIMP };

public:

    IMPORT_C static CPresenceInfoFieldImp* NewLC( const TDesC8& aFieldName,
                                                      CXIMPApiDataObjBase* aFieldValue );
    
    IMPORT_C static CPresenceInfoFieldImp* NewLC();

    virtual ~CPresenceInfoFieldImp();

private:

    CPresenceInfoFieldImp();

    void ConstructL( const TDesC8& aFieldName,
                     CXIMPApiDataObjBase* aFieldValue );

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


public: // from MPresenceInfoField

    /**
     * @see MPresenceInfoField
     */
    const TDesC8& FieldType() const;
    const MXIMPBase& FieldValue() const;
    MXIMPBase& FieldValue();
    void SetFieldValue( MXIMPBase* aFieldValue );
    void SetFieldTypeL( const TDesC8& aFieldType );

private: // data

    // owned. name of the field
    RBuf8 iFieldName;

    // owned. the actual field.
    CXIMPApiDataObjBase* iFieldValue;
    };
    

#endif // CPRESENCEINFOFIELDIMP_H



