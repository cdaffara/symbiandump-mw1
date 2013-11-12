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
* Description:  Event capsulation for shared ownership.
*
*/

#ifndef CXIMPEVENTCAPSULE_H
#define CXIMPEVENTCAPSULE_H

#include <e32base.h>
#include <ximpbase.h>

class CXIMPApiEventBase;

/**
 * Event capsulation for shared ownership.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
class CXIMPEventCapsule : public CObject
    {

public:

    /**
     * Construction
     * @aEvent. Event for capsulation. Ownership transferred.
     */
    IMPORT_C static CXIMPEventCapsule* NewL( CXIMPApiEventBase& aEvent, 
                                             TBool aEventReady = ETrue );

private:

    /**
     * Direct deletion not allowed.
     * Deletion happens through reference counting.
     */
    ~CXIMPEventCapsule();
    CXIMPEventCapsule( TBool aEventReady );
    void ConstructL( CXIMPApiEventBase& aEvent );

public: // new methods

    /**
     * Reference to capsulated event
     * @return packed event.
     */
    IMPORT_C const TDesC8& EventData() const;


    /**
     * Req Id of capsulated event
     * @return Req Id. If Capsuled event is 
     *         some other type than RequestComplete event, 
     *         then req id has TXIMPRequestId::Null() value. 
     */
    IMPORT_C TXIMPRequestId EventReqId() const;

    /**
     * Event type for filtering events.
     * @return Event interface id.
     */
    IMPORT_C TInt32 EventType() const;
    
    /**
     * Event is ready
     * @return ETrue if event is ready. EFalse if not.
     */
    IMPORT_C TBool Ready() const;
    
    /**
     * Update event data.
     */
    IMPORT_C void UpdateEventDataL( CXIMPApiEventBase& aEvent );
    IMPORT_C void UpdateEventData( CXIMPApiEventBase& aEvent );
    
    /**
     * Set event ready for sending.
     */
    IMPORT_C void SetReady( TBool aReady );   
    
    /**
     * Check which one should be handled first.
     */
    TBool operator<( const CXIMPEventCapsule& aOther ) const;
      
    /**
     * Set timestamp for order check.
     */
    void SetTimeStamp( TUint32 aTimeStamp );


private: // Helpers
    
    /**
     * Initializes the reg id member if the passed event is 
     * req complete type.
     */
    void InitReqId( const CXIMPApiEventBase& aEvent );

private: // Data

    /**
     * Capsulated event. Owns.
     */
    RBuf8 iEventBuf;

    /**
     * Event type for filtering
     */
    TInt32 iEventType;
    
    /**
     * If event is not ready this is set as ETrue
     */
    TBool iEventReady;
    
    /**
     * Used for packing.
     */
    CBufFlat* iBuffer;
    
    /**
     * Time stamp for order.
     */
    TUint32 iTimeStamp;
    
    /**
     * Request ID.
     */
    TXIMPRequestId iReqId;
    };

#endif //CXIMPEVENTCAPSULE_H
