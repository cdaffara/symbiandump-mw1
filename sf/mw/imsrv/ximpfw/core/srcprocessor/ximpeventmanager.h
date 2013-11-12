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
* Description:  XIMP Framework server event manager implementation
*
*/

#ifndef C_PSCEVENTMANAGER_H
#define C_PSCEVENTMANAGER_H


#include "ximpbase.h"
#include <e32base.h>

class CXIMPContextEventQueue;
class CXIMPApiEventBase;
class CXIMPPscContext;
class CXIMPRequestCompleteEventImp;

/**
 * Event handling.
 *
 * @since S60 v4.0
 */
class CXIMPEventManager : public CBase
    {

public:

    /**
     * Construction
     */
    IMPORT_C static CXIMPEventManager* NewL();

    /**
     * Destruction
     */
    virtual ~CXIMPEventManager();

private:

    /** 
     * Construction
     */
    CXIMPEventManager();
    
    /** 
     * Construction
     */
    void ConstructL();


public: // New methods

    /**
     * Register session queue.
     * @param aQueue Session queue for registration.
     */
    IMPORT_C void RegisterSessionQueueL( CXIMPContextEventQueue& aQueue );
    
    /**
     * Unregister session by id.
     * @param aSessionId, Session id for unregistration.
     */
    IMPORT_C void UnregisterSessionQueue( CXIMPContextEventQueue& aQueue );
    
    /**
     * Add event to session.
     * @param aEvent Event to add.
     * @param aSessionId, Session that receives event.
     */
    IMPORT_C void AddEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext  );
    IMPORT_C void ActivateEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext  );
    IMPORT_C void ActivateRequestCompleteEvent( CXIMPRequestCompleteEventImp& aEvent, 
                                                CXIMPPscContext* aContext );

    /**
     *  Add event to session.
     * @param aEvent Event to add.
     * @param aSessionId, Array of sessions that receives event.
     */
    IMPORT_C void AddEventL( CXIMPApiEventBase& aEvent, 
                             RPointerArray< CXIMPPscContext >& aSessions );
private: // data

    /**
     * Array of event queues. 
     * Does not own items.
     */
    RPointerArray< CXIMPContextEventQueue > iEventQueues;
    };


#endif // C_PSCEVENTMANAGER_H
