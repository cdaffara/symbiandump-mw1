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
* Description:  Presence context event pump.
*
*/

#ifndef CXIMPPRESENCECONTEXTEVENTPUMP_H
#define CXIMPPRESENCECONTEXTEVENTPUMP_H

#include "ximpcontexteventpump.h"
#include <e32base.h>


class MXIMPContext;
class MXIMPContextObserver;
class CXIMPContextClientProxy;
class CXIMPCntxtEventObsArray;
class CXIMPContextEventFilter;
class CXIMPRequestCompleteEventImp;
class CXIMPContextStateEventImp;
class MApiDataObjFactoryAccessor;

/**
 * Presence context event pump.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextEventPump )
    : public CActive
    {
public:


    static CXIMPContextEventPump* NewL( MXIMPContext& aCntxtInterface,
                                                CXIMPContextClientProxy& aContextProxy );
    virtual ~CXIMPContextEventPump();

private:

    CXIMPContextEventPump( MXIMPContext& aCntxtInterface,
                                   CXIMPContextClientProxy& aContextProxy );
    void ConstructL();




public:


    /**
     * Registers observer with event type filter. 
     *
     * Combines event type filter from so far 
     * registered observers and installs filter 
     * to server if there exists a valid server connection.  
     * 
     * @param aObserver The observer to notify from presence events.
     * @param aEventFilter Event filter array for limiting accepted events.
     */
    void RegisterObserverL( MXIMPContextObserver& aObserver,
                            const TArray< TInt32 >* aEventFilter );

    /**
     * Unregisters observer. 
     * 
     * @param aObserver The observer to unregister.
     */
    void UnRegisterObserver( MXIMPContextObserver& aObserver );


    /**
     * Combines event type filter from so far registered 
     * observers and installs filter to server. 
     */
    void StartEventPumpLC();
    
    
    /**
     * Stops event pump from pumping events.
     */
    void Close();



public:     // From CActive

    /**
     * Handles an active object’s request completion event.
     * Gets event data from server side and notifies observers.
     */
    void RunL();
    

    /**
     * Handles a leave occurring in the RunL().
     *
     * @param aError The leave code from RunL().
     * @return Always KErrNone.
     */
    TInt RunError( TInt aError );


    /**
     * Implements cancellation of an outstanding request.
     * This function is called as part of the active object’s Cancel().
     */
    void DoCancel();


private:

    void TryRefreshEventRequest();
    void TryInstallEventFilterL( const CXIMPContextEventFilter& aFilter );



private: // data



    /**
     * Client interface for event notify.
     * Not own.
     */
    MXIMPContext& iCntxtInterface;


    /**
     * Context client for getting events. 
     * Not own.
     */
    CXIMPContextClientProxy* iCtxClientProxy;


    /**
     * Array of observers.
     * Own.
     */
    CXIMPCntxtEventObsArray* iObservers;
    
    
    /**
     * Spare req complete event.
     * Own.
     */
    CXIMPRequestCompleteEventImp* iSpareReqCompleteEvent;


    /**
     * Spare ctx state change event.
     * Own.
     */
    CXIMPContextStateEventImp* iSpareCtxStateEvent;

    };


#endif // CXIMPPRESENCECONTEXTEVENTPUMP_H
