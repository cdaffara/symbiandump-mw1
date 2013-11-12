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
* Description:  MXIMPStatus API object implementation.
*
*/

#ifndef CXIMPSTATUSIMP_H
#define CXIMPSTATUSIMP_H

#include "ximpapidataobjbase.h"
#include <ximpstatus.h>
#include <s32strm.h>


/**
 * MXIMPStatus API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPStatusImp ): public CXIMPApiDataObjBase,
                                     public MXIMPStatus
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPSTATUSIMP };

public:

    IMPORT_C static CXIMPStatusImp* NewLC();
    IMPORT_C static CXIMPStatusImp* NewL();
    virtual ~CXIMPStatusImp();


private:

    CXIMPStatusImp();
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

public: // From MXIMPStatus

    /**
     * Implementation of MXIMPStatus interface methods
     * @see MXIMPStatus
     */
    TInt ResultCode() const;
    TInt ProtocolResultCode() const;
    const TDesC16& ProtocolResultDescription() const;

    void SetResultCode( TInt aCode );
    void SetProtocolResultCode( TInt aCode );
    void SetProtocolResultDescriptionL( const TDesC16& aDescription );




public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

private: // data


    /**
     * Result code.
     * Own.
     */
    TInt iResultCode;


    /**
     * Protocol result code.
     * Own.
     */
    TInt iProtocolResultCode;


    /**
     * Protocol description.
     * Own.
     */
    RBuf iProtocolResultDescription;

    };


#endif // CXIMPSTATUSIMP_H


