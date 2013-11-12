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
* Description:  Implementation of presence info field binary value object.
*
*/

#ifndef CPRESENCEINFOFIELDVALUEBINARYIMP_H
#define CPRESENCEINFOFIELDVALUEBINARYIMP_H

#include <e32std.h>
#include <s32strm.h> // RWriteStream, RReadStream
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include "presenceinfo.h"
#include "ximpapidataobjbase.h"
#include "presenceinfofieldvaluebinary.h"
#include "presenceapiobjbase.h"

/**
 * Implementation of presence info field binary value object.
 *
 *
 *
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoFieldValueBinaryImp ): public CXIMPApiDataObjBase, 
    public MPresenceInfoFieldValueBinary
    {
public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOFIELDVALUEBINARYIMP };

public:

    /**
     * @param aMaxLength NPresenceInfo::KPrValueLengthUnlimited to not enforce
     * length limits
     */
    IMPORT_C static CPresenceInfoFieldValueBinaryImp* NewLC(
            TInt aMaxLength = NPresenceInfo::KValueLengthUnlimited );

    virtual ~CPresenceInfoFieldValueBinaryImp();

private:

    CPresenceInfoFieldValueBinaryImp( TInt aMaxLength );
    CPresenceInfoFieldValueBinaryImp();

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


public: // from MPresenceInfoFieldValueBinary

    /**
     * @see MPresenceInfoFieldValueBinary
     */
    TInt MaxLength() const;

    /**
     * @see MPresenceInfoFieldValueBinary
     */
    TPtrC8 BinaryValue() const;

    /**
     * @see MPresenceInfoFieldValueBinary
     */
    const TPtrC8 MimeType() const;

    /**
     * @see MPresenceInfoFieldValueBinary
     */
    void SetBinaryValueL(
            const TDesC8& aValue );

    /**
     * @see MPresenceInfoFieldValueBinary
     */
    void SetMimeTypeL(
            const TDesC8& aMimeType );

private: // data

    // owned. field value
    RBuf8 iValue;

    // owned. mime type
    RBuf8 iMimeType;

    // maximum length of field value
    TInt iMaxLength;
    };

#endif // CPRESENCEINFOFIELDVALUEBINARYIMP_H



