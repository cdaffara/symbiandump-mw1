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
* Description: implementation of CXIMPContextEventQueue
 *
*/

#ifndef XIMPCONTEXTEVENTQUEUE_H
#define XIMPCONTEXTEVENTQUEUE_H

#include <e32base.h>
#include <ximpbase.h>

class CXIMPEventCapsule;
class CXIMPContextEventFilter;
class MXIMPEventQueueObserver;
class CXIMPApiEventBase;
class CXIMPRequestCompleteEventImp;

/**
 * Event queue for client to process.
 */
class CXIMPContextEventQueue : public CBase
    {
public:     // Constructors and destructors

    /**
     * Construction
     */
    IMPORT_C static CXIMPContextEventQueue* NewL();
    
    /**
     * Destruction
     */
    ~CXIMPContextEventQueue();

protected:  // Constructors

    /**
     * Construction
     */
    CXIMPContextEventQueue();

    /**
     * Construction
     */
    void ConstructL();

public: // New functions

    /**
     * Set event filter for filtering events
     * @param aFilter New filter.
     */
    IMPORT_C void SetEventFilter( CXIMPContextEventFilter& aFilter );
    
    /**
     * Start consuming events.
     * @param aQueueObserver Observer to notify when event is ready
     */
    IMPORT_C void StartConsuming( MXIMPEventQueueObserver& aQueueObserver );
    
    /**
     * Stop event consuming.
     */
    IMPORT_C void StopConsuming();

    /**
     * Check if there is elements.
     */
    IMPORT_C TBool HasElements() const;
    IMPORT_C void SelectTopEventIfNeededL();
    IMPORT_C const TDesC8& TopEventDataL();
    IMPORT_C TXIMPRequestId TopEventReqIdL();
    IMPORT_C void DropTopEvent();

    /** 
     * Add new event to queue.
     */
    void AddEventL( CXIMPEventCapsule& aEvent );
    
    /**
     * Activate new request complete event to queue.
     * @param aEvent Event to activate.
     */
    void ActivateRequestCompleteEvent( CXIMPRequestCompleteEventImp& aEvent );
    
    /**
     * Remove request complete event from queue
     * @param aEvent Event to remove
     */
    void RemoveRequestCompleteEvent( CXIMPRequestCompleteEventImp& aEvent );
    
    /**
     * Activate preserved event for sending.
     */
    void ActivatePreservedEventL( CXIMPApiEventBase& aEvent );

    /**
     * Get id of queue.
     * @return Id of queue.
     */
    inline TUint32 SessionIdOfQueue() const { return iId; }
    
    /**
     * Set id of queue
     * @param aId Id of queue.
     */
    inline void SetSessionIdOfQueue( TUint32 aId ) { iId = aId; }

private:    // Helpers

    /**
     * Clear queued events.
     */
    void ClearQueuedEvents();
    
    /**
     * Add new preserved event to queue.
     */
    void AddPreservedEventL( CXIMPApiEventBase& aEvent );
    
    /**
     * Find next ready event from array. Timestamp is used to choose first one.
     * iNextEventToGo is filled with oldest event. iNextEventToGo can be filled
     * with reference event before call.
     * @param aArray. Array of events to choose.
     * @return Index of found event in array
     */ 
    TInt FindNextEvent( RPointerArray< CXIMPEventCapsule >& aArray );
    
private:    // Data

    /**
     * Queue ID.
     */
    TUint32 iId;

    /**
     * Events array. Reference to events own.
     */
    RPointerArray< CXIMPEventCapsule > iEvents;
    
    /** 
     * Preserved events array. Reference to events own.
     */
    RPointerArray< CXIMPEventCapsule > iPreservedEvents;
    
    /**
     * Request complete events waiting for completion.
     */
    RPointerArray< CXIMPEventCapsule > iRequestCompleteEvents;    

    /**
     * Event filter. Own.
     */
    CXIMPContextEventFilter* iEventFilter;

    /**
     * Queue observer. Ref.
     */
    MXIMPEventQueueObserver* iObserver;

    /**
     * Helper variable to contain next event to go.
     */
    CXIMPEventCapsule* iNextEventToGo;
    
    /**
     * Timestamp for events.
     */
    TUint32 iTimeStamp;

    };

#endif      //  XIMPCONTEXTEVENTQUEUE_H


