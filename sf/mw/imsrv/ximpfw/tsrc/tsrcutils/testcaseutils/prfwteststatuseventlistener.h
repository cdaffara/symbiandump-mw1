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
* Description: XIMP Framework Test Code prfwteststatuseventlistener.h
*
*/

#ifndef CPRFWTESTSTATUSEVENTLISTENER_H__
#define CPRFWTESTSTATUSEVENTLISTENER_H__


#include <e32base.h>
#include <ximpcontextobserver.h>
#include <ximpcontextstate.h>



// CLASS DESCRIPTION
/**
 * Helper class to store snapshot from
 * received status events:
 * - MXIMPRequestCompleteEvent
 * - MXIMPContextStateEvent
 */
class TXIMPTestStatusEventSnapshot
    {
public: //Value enums
    enum
        {
        EStatusObjNotPresent = 100,
        EStateValueNotPresent = 101
        };

public: //Constructors

    //Default c++ constructor
    IMPORT_C TXIMPTestStatusEventSnapshot();

    //Constructor accepting integer fields
    IMPORT_C TXIMPTestStatusEventSnapshot( TInt aEventType,
                                           TInt aCtxState,
                                           TInt aResultCode );

public: //Comparator functions

    IMPORT_C TBool Equals( const TXIMPTestStatusEventSnapshot& aOther ) const;


public: //Data
    TInt iEventType;
    MXIMPContextState::TState iCtxState;
    TXIMPRequestId iReqId;
    TInt iResultCode;
    TBuf< 24 > iPrtResultDescription;
    };


/**
 * Enumeration defining supported
 * status event templates.
 */
enum TXIMPTestStatusEventTemplate
    {
    EXIMPTestStatusEvents_BindingOk,
    EXIMPTestStatusEvents_BindingFailKErrAbort,
    EXIMPTestStatusEvents_BindingFailInvalidReqId,
    EXIMPTestStatusEvents_BindingFailServerTermination,

    EXIMPTestStatusEvents_UnbindingOk,

    EXIMPTestStatusEvents_RequestOk,
    EXIMPTestStatusEvents_RequestOOM,

    EXIMPTestStatusEvents_PublishOk,
    EXIMPTestStatusEvents_PublishFailKErrAbort,

    EXIMPTestStatusEvents_SubscribeOwnPresenceSingleCtx,
    EXIMPTestStatusEvents_UnsubscribeOwnPresenceSingleCtx,

    EXIMPTestStatusEvents_SubscribePresentityPresenceSingleCtx,
    EXIMPTestStatusEvents_UnsubscribePresentityPresenceSingleCtx,

    EXIMPTestStatusEvents_SubscribePresentityGroupListCtx,
    EXIMPTestStatusEvents_UnsubscribePresentityGroupListCtx,

    EXIMPTestStatusEvents_SubscribePresentityGroupContentSingleCtx,
    EXIMPTestStatusEvents_UnsubscribePresentityGroupContentSingleCtx,

    EXIMPTestStatusEvents_HandlePresentityGroupList,

    EXIMPTestStatusEvents_CreatePresentityGroup,

    EXIMPTestStatusEvents_SubscribePresenceWatcherListCtx,
    EXIMPTestStatusEvents_UnsubscribePresenceWatcherListCtx,
    EXIMPTestStatusEvents_HandlePresenceWatcherList,

    EXIMPTestStatusEvents_HandlePresentityPresence,
    EXIMPTestStatusEvents_HandlePresentityPresencePending,
    EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort,
    };


/**
 * Presence context listener to record status events
 * and to wait request completion
 * Handles following event types:
 * - MXIMPRequestCompleteEvent
 * - MXIMPContextStateEvent
 *
 * For other XIMP event types, see other
 * test listeners.
 */
class CXIMPTestStatusEventListener : public CBase,
                                     public MXIMPContextObserver
    {
public:

    /**
     * Instantiates new listener object.
     *
     * If valid event source is given, reqisters this object as
     * event observer to it.
     */
    IMPORT_C static CXIMPTestStatusEventListener* CXIMPTestStatusEventListener::NewL(
                    TInt aStackSize,
                    MXIMPContext* aEventSource );

    ~CXIMPTestStatusEventListener();

private:
    CXIMPTestStatusEventListener( TInt aStackSize,
                                  MXIMPContext* aEventSource );
    void ConstructL();


public:

    /**
     * Sets new event source from where to listen events.
     * Unregisters from previous one.
     * Can be NULL, in wich case just performs the
     * unregistration.
     */
    IMPORT_C void SetEventSourceL( MXIMPContext* aEventSource );

    /**
     * Access to accepted event types list.
     * Client can alter the list.
     *
     * By default list has:
     * - MXIMPRequestCompleteEvent
     * - MXIMPContextStateEvent
     *
     * NOTE: remember to push edited list to event source with
     *       ReRegisterEventFilterL()
     */
    IMPORT_C RArray< TInt32 >& AcceptedEventTypes();


    /**
     * ReRegisters this listener to event source.
     * In practice, takes new event filter in use.
     */
    IMPORT_C void ReRegisterEventFilterL();


    /**
     * Fills the given array with event templates.
     *
     * Given enumeration defines which template to retrieve.
     */
    IMPORT_C void GetEventTemplateL( RArray< TXIMPTestStatusEventSnapshot >& aArray,
                                     TXIMPTestStatusEventTemplate aTemplate );

    /**
     * Method to wait with CActiveSchedulerWait
     * the given request completion.
     * NOTE: Only one request can be waited at time.
     * Stacks received MXIMPRequestCompleteEvent and
     * MXIMPContextStateEvent details to
     * event stack.
     *
     * @param aReqToWait The request to wait.
     */
    IMPORT_C void WaitRequestAndStackEvents( const TXIMPRequestId& aReqToWait, TBool aAnyReq = EFalse );
    IMPORT_C void WaitAnyEvent();


    /**
     * Verifies event stack contents against the given traits.
     *
     * NOTE: You can use GetEventTemplateL() to retrieve a
     * common event traits.
     *
     * Error title is printed in the error assertion description.
     */
    IMPORT_C void VerifyEventStackL( const TArray< TXIMPTestStatusEventSnapshot >& aEventTraits,
                                     const TDesC8& aErrorTitle ) const;

    
    
    IMPORT_C TInt GetReqCompletionErrL( const TXIMPRequestId& aReqId ) const;



    /**
     * Resets the event stack contents.
     */
    IMPORT_C void ResetEventStack();
    IMPORT_C void ResetEventTypeStack();

    /**
     * Set event observer when needed.
     */
    IMPORT_C void SetEventObserver( MXIMPContextObserver* aObserver );


private:  //From MXIMPContextObserver

    void HandlePresenceContextEvent(
            const MXIMPContext& aContext,
            const MXIMPBase& aEvent );


private: //Helpers
    void HandleRequestCompleteReceived();
    TBool IsAcceptedEventType( TInt32 aEventType ) const;
    void AddSnapshotToStack( TXIMPTestStatusEventSnapshot aSnapshot );
    const TXIMPTestStatusEventSnapshot& SnapshotFromStack( TInt aIndex ) const;

    TBool HasSameEvents( const TArray< TXIMPTestStatusEventSnapshot >& aEventTraits ) const;
    void GetNameForEvent( TDes8& aTarget, TInt aEventType ) const;
    void SafeAppendEventName( TDes8& aTarget, const TDesC8& aName ) const;




private:  //Data

    //OWN: Active scheduler wait for waiting the completion
    CActiveSchedulerWait    iWait;


    //OWN: Request ID identifying request which completion event to wait
    TXIMPRequestId    iReqToWait;


    //REF: Context from where receiving events
    MXIMPContext* iEventSource;


    //OWN: Accepted event types
    RArray< TInt32 > iAcceptedEventTypes;


    //OWN: Variables to maintain event snapshot stack
    const TInt iSnapshotMaxCount;
    TInt iSnapshotCount;
    TXIMPTestStatusEventSnapshot* iSnapshots;

    //OWN: Description buffer to hold the failure description
    mutable RBuf8 iDescription;

    TBool iWaitForAnyReq;

    TBool iWaitForAnyEvent;

    // To handle event by observer.
    MXIMPContextObserver* iObserver;
    };

#endif //CPRFWTESTSTATUSEVENTLISTENER_H__


