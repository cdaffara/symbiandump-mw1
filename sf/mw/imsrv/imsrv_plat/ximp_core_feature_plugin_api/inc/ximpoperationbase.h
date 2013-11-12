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
* Description:  Base class for operations
 *
*/

#ifndef C_XIMPPSCOPERATIONBASE_H
#define C_XIMPPSCOPERATIONBASE_H

#include <e32base.h>

class CXIMPRequestCompleteEventImp;
class CXIMPRestrictedObjectCollectionImp;
class CXIMPPscContext;
class MXIMPHost;
class MXIMPStatus;
class CXIMPStatusImp;
class MXIMPPscContext ;
class MXIMPRestrictedObjectCollection ;


/**
 * Base C-class for operations
 *
 * @lib ximpoperation.lib
 * @since S60 v4.0
 */
class CXIMPOperationBase : public CBase
    {
    public:

        /**
         * Call this, then BaseConstructL in derived class.
         */
        IMPORT_C CXIMPOperationBase();

        IMPORT_C virtual ~CXIMPOperationBase();

        /**
         * @param aRequestId The request id
         */
        IMPORT_C void BaseConstructL( TXIMPRequestId aRequestId,
                                    CXIMPPscContext* aContext );

    public: // common services

        virtual void ConstructL( const TDesC8& aParamPck ) = 0;

        /**
         * Set host for operation. ProcessL cannot be called before
         * host is set.
         */        
        IMPORT_C virtual void SetHost( MXIMPHost& aMyHost );

        /**
         * Process this operation.
         */
        IMPORT_C virtual void ProcessL();

        /**
         * Request made by the operation has been completed.
         */
        IMPORT_C virtual void RequestCompletedL();

        /**
         * @return The request complete event
         */
        IMPORT_C virtual CXIMPRequestCompleteEventImp* GetReqCompleteEvent();

        /**
         * @return The operation type which this operation implements.
         */
        IMPORT_C virtual TInt Type() const;

        /**
         * @param aContext The context of operation
         */
        IMPORT_C virtual void SetOpL( CXIMPPscContext* aContext );

        /**
         * @param aRequestId The request id
         */
        IMPORT_C virtual void SetReqId( TXIMPRequestId aRequestId );

        /**
         * @return Context object of operation
         */
        IMPORT_C virtual CXIMPPscContext* Context();

        /**
         * @return The current request id
         */
        IMPORT_C virtual TXIMPRequestId RequestId() const;
        
        /**
         * Set result code for operation
         */
        IMPORT_C virtual void SetResultCode( TInt aResultCode );
        
        /**
         * Set status for operation
         */
        IMPORT_C virtual void SetStatus( MXIMPStatus* aStatus );

        /**
         * Set new object collection for operator.
         */        
        IMPORT_C virtual void SetObjectCollection( MXIMPBase* aCollection );

        /**
         * 
         */
        IMPORT_C virtual void BeginOperation();

        /**
         *
         */        
        IMPORT_C virtual void EndOperation();
        
        /**
         *
         */
        IMPORT_C virtual MXIMPStatus& Status() const;
        
        
        /**
         *
         */
        IMPORT_C TBool IsServerOperation() const;
        
        /**
         *
         */
        IMPORT_C void SetServerOperation();
        
         /**
         *
         */
        IMPORT_C MXIMPPscContext* GetContext();
 
        /**
         *
         */
        IMPORT_C MXIMPRestrictedObjectCollection* GetObjCollection();
      
    protected: // Internal methods
    
        /**
         * Create request complete event
         */
        IMPORT_C virtual void CreateRequestCompleteEventL();
    
    protected: // data

        /**
         * Req id
         */
        TXIMPRequestId iReqId;

        /**
         * Host of this operation. Shared owning. Use close to destroy
         */
        MXIMPHost* iMyHost;

        /**
         * Request complete event
         * Owned.
         */
        CXIMPRequestCompleteEventImp* iReqCompleteEvent;

        /**
         * Context of operation
         */
        CXIMPPscContext* iContext;

        /**
         * If ETrue, operation is not active
         */
        TBool iNotActive;
        
        /**
         * Internal state of the host
         */
        CXIMPStatusImp* iStatusObj;
        
        /**
         * Holder for parameters. Owned
         */
        CXIMPRestrictedObjectCollectionImp* iObjCollection;        
        
        /**
         * True if server initiated operation
         */
        TBool iServerOperation;
    };

#endif // C_XIMPPSCOPERATIONBASE_H
