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
* Description:  Implementation of presence info field text value object.
*
*/

#ifndef CPRESENCEINFOFIELDVALUETEXTIMP_H
#define CPRESENCEINFOFIELDVALUETEXTIMP_H

#include <e32std.h>
#include <s32strm.h> // RWriteStream, RReadStream
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include "presenceinfo.h"
#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include "presenceinfofieldvaluetext.h"


/**
 * Implementation of presence info field text value object.
 *
 *
 *
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoFieldValueTextImp ): public CXIMPApiDataObjBase, 
    public MPresenceInfoFieldValueText
    {
public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOFIELDVALUETEXTIMP };

public:

    /**
     * @param aMaxLength NPresenceInfo::KPrValueLengthUnlimited to not enforce
     * length limits
     */
    IMPORT_C static CPresenceInfoFieldValueTextImp* NewLC(
            TInt aMaxLength = NPresenceInfo::KValueLengthUnlimited );

    virtual ~CPresenceInfoFieldValueTextImp();

private:

    CPresenceInfoFieldValueTextImp( TInt aMaxLength );
    CPresenceInfoFieldValueTextImp();

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


public: // from MPresenceInfoFieldValueText

    /**
     * @see MPresenceInfoFieldValueText
     */
    TInt MaxLength() const;

    /**
     * @see MPresenceInfoFieldValueText
     */
    TPtrC TextValue() const;

    /**
     * @see MPresenceInfoFieldValueText
     */
    void SetTextValueL( const TDesC& aValue );

private: // data

    // owned. field value
    RBuf16 iFieldValue;

    // maximum length of field value
    TInt iMaxLength;
    };

#endif // CPRESENCEINFOFIELDVALUETEXTIMP_H



