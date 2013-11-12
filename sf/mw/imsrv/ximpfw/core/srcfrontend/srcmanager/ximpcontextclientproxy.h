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
* Description:  Proxy class to manage context client session.
*
*/

#ifndef CXIMPCONTEXTCLIENTPROXY_H
#define CXIMPCONTEXTCLIENTPROXY_H


#include "ximpsrvcontextclient.h"
#include <e32base.h>



/**
 * Type of the request.
 */
enum TXIMPAsyncReqType
    { 
    EXIMPAsyncOpenReq, 
    EXIMPAsyncCloseReq, 
    EXIMPAsyncGeneralReq 
    };


/**
 * Request entry structure for client side request wait list.
 */
NONSHARABLE_CLASS( TXIMPAsyncReqEntry )
    {
public:
    
    /**
     * Id of the request.
     * Own.
     */
    TXIMPRequestId  iReqId;

    /**
     * Type of request.
     * Own.
     */
    TXIMPAsyncReqType  iReqType;
    };



/**
 * Proxy class to manage context client session.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextClientProxy ): public CBase
    {
public:
    CXIMPContextClientProxy();
    ~CXIMPContextClientProxy();


public: // New methods

    /**
     * Opens context client session to 
     * server handling the given protocol.
     *
     * Handles errors by leaving. Pushes 
     * close to cleanupstack to close 
     * automaticly the session in 
     * case of leave. 
     * 
     * @param aProtocolUid Protocol implementation uid. 
     */
    void InitServerSessionLC( TUid aProtocolUid );



    /**
     * Issues context connection open request to server.
     *
     * @param aOperationType In parameter identifying the operation.
     * @param aOperationParams In parameter package for operation.
     * @return Request ID identifying started request.
     */
    TXIMPRequestId QueueAsyncConnectionOpenL(
                TInt aOperationType, 
                const TDesC8& aOperationParams );

     /**
     * Issues context connection open request to server.
     * To be used when binding with settings is
     *
     * @param aOperationType In parameter identifying the operation.
     * @param aOperationParams In parameter package for operation.
     * @return Request ID identifying started request.
     */
    TXIMPRequestId QueueAsyncConnectionOpen2L(
                TInt aOperationType, 
                const TDesC8& aOperationParams );


    /**
     * Issues context connection close request to server.
     *
     * @param aOperationType In parameter identifying the operation.
     * @param aOperationParams In parameter package for operation.
     * @return Request ID identifying started request.
     */
    TXIMPRequestId QueueAsyncConnectionCloseL(
                TInt aOperationType, 
                const TDesC8& aOperationParams );



    /**
     * Issues context operation request to server.
     *
     * @param aOperationType In parameter identifying the operation.
     * @param aOperationParams In parameter package for operation.
     * @return Request ID identifying started request.
     */
    TXIMPRequestId QueueOperationL(
                TInt aOperationType, 
                const TDesC8& aOperationParams );

    /**
     * Gets RXIMPSrvContextClient session. 
     * If there isn't a valid session, handles 
     * error by leaving. 
     * 
     * @return Valid RXIMPSrvContextClient session.
     */
    RXIMPSrvContextClient& EventPumpSessionL();


    /**
     * Gets RXIMPSrvContextClient session. 
     * If there isn't a valid session, returns NULL. 
     * 
     * @return Valid RXIMPSrvContextClient session.
     */
    RXIMPSrvContextClient* EventPumpSession();

    
    
    /**
     * Gets RXIMPSrvContextClient session. 
     * If there isn't a valid session, handles 
     * error by leaving. 
     * 
     * @return Valid RXIMPSrvContextClient session.
     */
    const RXIMPSrvContextClient& DirectSessionL() const;
    
        
    
    /**
     * Closes RXIMPSrvContextClient session.
     */
    void Close();

    
    /**
     * Removes identified request from wait list and 
     * updates properly the proxy server connection 
     * status based on the given request completion code.
     *
     * @param aReqId Request id to drop from wait list.
     * @param aCompletionCode Completion code indicating 
     *        how request was completed.
     */
    void HandleReqCompleted( TXIMPRequestId aReqId, TInt aCompletionCode );


    /**
     * Gets topmost request ID from wait list. 
     *
     * @param aReqId On the return contains topmost req id from wait list.
     * @return ETrue if the wait list contained elements. 
     *         EFalse if the wait list was empty. 
     */
    TBool GetTopPendingReq( TXIMPRequestId& aReqId ) const;
    


private: // Helpers

    TXIMPRequestId DoQueueAsyncOpL( TInt aOperationType, 
                                    const TDesC8& aOperationParams,
                                    TXIMPAsyncReqType aReqType );

                                    
    TXIMPRequestId DoQueueAsyncOp2L( TInt aOperationType, 
                                     const TDesC8& aOperationParams,
                                     TXIMPAsyncReqType aReqType );
                                    

    void UpdateCtxConnectionStatus( TXIMPAsyncReqType aReqType, 
                                    TInt aCompletionCode );


private: // data

    /**
     * Managed context session to server.
     * Own.
     */
    RXIMPSrvContextClient iCtxConnection;


    /**
     * Flag to maintain session status.
     * Own.
     */
    enum TCtxConnectionState { ENotConnected, EInTransition, EConnected };     
    TCtxConnectionState iCtxConnectionState;
    
    /**
     * Array to maintain pending clientside requests.
     * Own.
     */
    CArrayFixSeg< TXIMPAsyncReqEntry > iReqWaitList;

    };


#endif // CXIMPCONTEXTCLIENTPROXY_H
