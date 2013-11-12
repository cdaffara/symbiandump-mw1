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
* Description:  XIMP Framework PSC Server context session.
 *
*/

#ifndef XIMPSRVCONTEXTCLIENT_H__
#define XIMPSRVCONTEXTCLIENT_H__

#include <e32std.h>
#include <ximpbase.h>


class CXIMPContextEventFilter;
class CXIMPServiceInfoImp;
class CXIMPFeatureInfoImp;
class CBufFlat;


/**
 * XIMP Framework server context session.
 *
 * @lib ximpsrvclient.dll
 * @since S60 v3.2
 */
class RXIMPSrvContextClient : public RSessionBase
    {
    public:

        /**
         * Default constructor.
         */
        IMPORT_C RXIMPSrvContextClient();


        /**
         * Session connection method.
         * @param aProtocolImpUid The UID of protocol to associate with 
         *        this context client.
         */
        IMPORT_C void ConnectL( TUid aProtocolImpUid );


        /**
         * Closes server connection.
         */
        IMPORT_C void Close();


        /**
         * Issues context operation request to server.
         *
         * @param aOperationType In parameter identifying the operation.
         * @param aOperationParams In parameter package for operation.
         * @param aReqId Request ID identifying started request.         
         * @return Error from issuing the request.
         */
        IMPORT_C TInt DoQueueOperation(
                TInt aOperationType, 
                const TDesC8& aOperationParams, 
                TXIMPRequestId& aReqId );
         

        /**
         * Issues context operation request to server.
         * To be used when binding with settings id
         *
         * @param aOperationType In parameter identifying the operation.
         * @param aOperationParams In parameter package for operation.
         * @param aReqId Request ID identifying started request.         
         * @return Error from issuing the request.
         */
        IMPORT_C TInt DoQueueOperation2(
                TInt aOperationType, 
                const TDesC8& aOperationParams, 
                TXIMPRequestId& aReqId );

        /**
         * Gets presence contexts features.
         *
         * @return Presence context features as an externalized
         * CXIMPFeatureInfoImp. Object ownership is returned to caller.
         */
        IMPORT_C HBufC8* GetContextFeaturesL() const;


        /**
         * Issues asynchronous request to listen presence context
         * events.
         *
         * @param aStatus Request status to complete
         *                when new event object is available.
         */
        IMPORT_C void ListenEvent( TRequestStatus& aStatus );


        /**
         * Cancels event listen request.
         */
        IMPORT_C void CancelListening();
        
        
        /**
         * Installs new event filter for presence context event queue.
         *
         * @return aFilterData Event filter data to use.
         */
        IMPORT_C void InstallEventFilterL( const TDesC8& aFilterData );


        /**
         * Gets event data from event queue.
         *
         * @return Event object data from event queue.
         *         Object ownership is returned to caller.
         *         Leaves with KErrNotFound if there isn't
         *         any event available in event queue.
         */
        IMPORT_C HBufC8* FetchTopEventDataLC();
        
        
        /**
         * Fetch top event req id.
         *
         * @param aReqId On the return contains, the top event req id. 
         *
         * @return Error from req id fetch. 
         */
        IMPORT_C TInt FetchTopEventReqId( TXIMPRequestId& aReqId );

        
        /**
         * Drops top event from event queue.
         *
         * @return Error from drop. 
         */
        IMPORT_C TInt DropTopEvent();


    private:

        /**
         * Helper to prepare and get data from server side.
         *
         * @return Externalized object to be passed onward
         */
        HBufC8* PrepareAndGetDataL( TInt aMsg ) const;


    private:

        /**
         * UID of the used protocol.
         */
        TUid iMyUid;

    };


#endif      //  XIMPSRVCONTEXTCLIENT_H__



