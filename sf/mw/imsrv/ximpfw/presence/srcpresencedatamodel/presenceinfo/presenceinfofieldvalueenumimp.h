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
* Description:  Implementation of presence info field enum value object.
*
*/

#ifndef CPRESENCEINFOFIELDVALUEENUMIMP_H
#define CPRESENCEINFOFIELDVALUEENUMIMP_H

#include <e32std.h>
#include <s32strm.h> // RWriteStream, RReadStream
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include "presenceinfofieldvalueenum.h"


/**
 * Implementation of presence info field enum value object.
 *
 *
 *
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoFieldValueEnumImp ): public CXIMPApiDataObjBase, 
    public MPresenceInfoFieldValueEnum
    {
public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOFIELDVALUEENUMIMP };

public:

    /**
     * @param aMaxLength KXIMPFieldValueLengthUnlimited to not enforce
     * length limits
     */
    IMPORT_C static CPresenceInfoFieldValueEnumImp* NewLC();

    virtual ~CPresenceInfoFieldValueEnumImp();

private:

    CPresenceInfoFieldValueEnumImp();

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


public: // from MPresenceInfoFieldValueEnum

    /**
     * @see MPresenceInfoFieldValueEnum
     */
    TInt MinValue() const;
    TInt MaxValue() const;
    TInt Value() const;
    void SetValueL( TInt aValue );
    void SetLimits( TInt aMin, TInt aMax );

private: // data

    // maximum allowed value
    TInt iMaxValue;

    // minimum allowed value
    TInt iMinValue;

    // actual value
    TInt iValue;
    };

#endif // CPRESENCEINFOFIELDVALUEENUMIMP_H



