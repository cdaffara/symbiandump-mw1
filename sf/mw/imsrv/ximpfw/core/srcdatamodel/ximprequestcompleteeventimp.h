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
* Description:  MXIMPRequestCompleteEvent API object implementation.
*
*/

#ifndef CXIMPREQUESTCOMPLETEEVENTIMP_H
#define CXIMPREQUESTCOMPLETEEVENTIMP_H

#include "ximpapieventbase.h"
#include "ximptypehelpers.h"
#include <ximprequestcompleteevent.h>

class RWriteStream;
class RReadStream;
class CXIMPStatusImp;
class MXIMPStatus;
class CXIMPApiDataObjBase;


/**
 * MXIMPRequestCompleteEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPRequestCompleteEventImp ): 
    public CXIMPApiEventBase,
    public MXIMPRequestCompleteEvent
    {
    public:
        /** The class ID. */
        enum { KClassId = XIMPIMP_CLSID_CXIMPREQUESTCOMPLETEEVENTIMP };

public:

    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CXIMPRequestCompleteEventImp* NewL();
    IMPORT_C static CXIMPRequestCompleteEventImp* NewLC(
            TXIMPRequestId& aRequestId, TInt aDefaultErrorValue = KErrNone );


    /**
     * Instantiation method for event automation.
     * Method signature must be exactly this to work
     * with event delivery automation system.
     *
     * Event implementation must be registered to
     * XIMPEventCodec KXIMPEventConstructorTable.
     */
    static CXIMPApiEventBase* NewFromStreamLC( RReadStream& aStream );


    virtual ~CXIMPRequestCompleteEventImp();

private:
    CXIMPRequestCompleteEventImp( TXIMPRequestId& aRequestId );
    CXIMPRequestCompleteEventImp();
    void ConstructL( TInt aDefaultErrorValue );
    void ConstructL( RReadStream& aStream );
    
public: // From MXIMPRequestCompleteEvent

    /**
     * Implementation of MXIMPRequestCompleteEvent interface methods
     * @see MXIMPRequestCompleteEvent
     */
    const TXIMPRequestId& RequestId() const;
    const MXIMPStatus& CompletionResult() const;    
    const MXIMPBase* LookupCompletionDataByType( TInt32 aInterfaceId ) const;


public:

    /** 
     * Set request id
     * @param aRequestId New requestId.
     */
    IMPORT_C void SetRequestId( const TXIMPRequestId& aRequestId );

    /**
     * Add data object as a parameter to this event. Ownership
     * transferred.
     * @param aObj The fully constructed data object to add
     */ 
    IMPORT_C void AppendParamL( CXIMPApiDataObjBase* aObj );
    
    
    IMPORT_C CXIMPStatusImp& CompletionResultImp();    

public: // From API base interfaces

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


    /**
     * Implementation of MXIMPEventBase interface and
     * CXIMPApiEventBase methods
     *
     * @see MXIMPEventBase
     * @see CXIMPApiEventBase
     */
    XIMPIMP_DECLARE_EVENT_BASE_METHODS

private: // data

    /**
     * Completed request.
     * Own.
     */
    TXIMPRequestId iReqId;


    /**
     * Request result.
     * This is set to KErrNoMemory for default value.
     * Reason for that is error handling in low memory situation
     * CXIMPEventCapsule will send KErrNoMemory version of event if
     * event cannot be updated because of low memory.
     * Own.
     */
    CXIMPStatusImp* iResult;
    
    /**
     * Array of params
     */
    RPrReqParamArray iParamArray;
    };


#endif // CXIMPREQUESTCOMPLETEEVENTIMP_H
