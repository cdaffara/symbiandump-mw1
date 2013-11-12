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
* Description:  Context session.
 *
*/

#ifndef XIMPCONTEXTSESSION_H
#define XIMPCONTEXTSESSION_H


#include "ximpsrvsession.h"
#include "ximpeventqueueobserver.h"
#include <e32base.h>


class MXIMPSrvMessage;
class CXIMPPscContext;


/**
 * Context session implementation.
 */
class CXIMPContextSession : public CBase,
                            public MXIMPSrvSession,
                            public MXIMPEventQueueObserver
    {
public:     // Constructors and destructors
    static CXIMPContextSession* NewL( TUint32 aSessionId );
    virtual ~CXIMPContextSession();


protected:  // Constructors
    CXIMPContextSession();
    void ConstructL( TUint32 aSessionId );


public: // From MXIMPSrvSession
    void TryHandleMessageL( MXIMPSrvMessage& aMessage );


public: // MXIMPEventQueueObserver
    void NewEventAvailable();


private:    // New methods


    void DoQueueOperationL( MXIMPSrvMessage& aMessage );

    void DoInstallEventFilterL( MXIMPSrvMessage& aMessage );
    void DoSetEventListenScoutL( MXIMPSrvMessage& aMessage );
    void DoCancelEventListenScout( MXIMPSrvMessage& aMessage );
    void DoFetchTopEventDataSizeL( MXIMPSrvMessage& aMessage );
    void DoFetchTopEventDataL( MXIMPSrvMessage& aMessage );
    void DoFetchTopEventReqIdL( MXIMPSrvMessage& aMessage );
    void DoDropTopEventL( MXIMPSrvMessage& aMessage );
    
    
    void DoPrepareContextFeaturesL( MXIMPSrvMessage& aMessage );
    void DoGetPreparedDataL( MXIMPSrvMessage& aMessage );



private:    // Data

    /**
     *
     */
    MXIMPSrvMessage* iEventListenScout;

    /**
     * Buffer caching the prepared data
     */
    HBufC8* iPreparedData;
    
    /**
     * Storage for client data.
     */
    CXIMPPscContext* iClientData;
    };

#endif      //  XIMPSESSION_H__


