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
* Description:  MXIMPProtocolRequestCompleteEvent API object implementation.
*
*/

#ifndef CXIMPPROTOCOLREQUESTCOMPLETEEVENTIMP_H
#define CXIMPPROTOCOLREQUESTCOMPLETEEVENTIMP_H

#include "ximpapieventbase.h"
#include <ximpprotocolrequestcompleteevent.h>


class RWriteStream;
class RReadStream;


/**
 * MXIMPProtocolRequestCompleteEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPProtocolRequestCompleteEventImp ):
    public CXIMPApiObjBase,
    public MXIMPProtocolRequestCompleteEvent
    {
    public:
        /** The class ID. */
        enum { KClassId = XIMPIMP_CLSID_CXIMPPROTOCOLREQUESTCOMPLETEEVENTIMP };

public:

    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CXIMPProtocolRequestCompleteEventImp* NewL();
    IMPORT_C static CXIMPProtocolRequestCompleteEventImp * NewFromStreamLC( RReadStream& aStream );
    virtual ~CXIMPProtocolRequestCompleteEventImp();

private:
    CXIMPProtocolRequestCompleteEventImp();
    void ConstructL();


public: // From MXIMPProtocolRequestCompleteEvent

    /**
     * Implementation of MXIMPProtocolRequestCompleteEvent interface methods
     * @see MXIMPProtocolRequestCompleteEvent
     */
    const TXIMPRequestId& RequestId() const;
    TInt ResultCode() const;


public:

    /**
     * Set request id
     * @param aRequestId New requestId.
     */
    IMPORT_C void SetRequestId( const TXIMPRequestId& aRequestId );


    /**
     * Set reseult code
     * @param aResult New result.
     */
    IMPORT_C void SetResultCode( TInt aResult );


    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;
    
    
    IMPORT_C void InternalizeL( RReadStream& aStream );



public: // From API base interfaces

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS
    



private: // data

    /**
     * Completed request.
     * Own.
     */
    TXIMPRequestId iReqId;


    /**
     * Request result.
     * Own.
     */
    TInt iResult;

    };


#endif // CXIMPPROTOCOLREQUESTCOMPLETEEVENTIMP_H
