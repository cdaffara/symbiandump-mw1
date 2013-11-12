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
* Description: XIMP Framework Test Code 
*
*/

#ifndef T_PRESENCEWATCHERLISTMANAGEMENT_H
#define T_PRESENCEWATCHERLISTMANAGEMENT_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include <ximpcontextobserver.h>
#include "presencetypehelpers.h"
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;
class CPresenceWatcherInfoImp;
class CPresenceWatcherListEventImp;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP presence watcher list.
 *
 * @since S60 v4.0
 */
class T_PresenceWatcherListManagement : public CEUnitTestSuiteClass, 
    public MXIMPContextObserver
    {

public:
    static T_PresenceWatcherListManagement* NewL();
    virtual ~T_PresenceWatcherListManagement();


private:
    T_PresenceWatcherListManagement();
    void ConstructL();

    enum TValidateType
        {
        EWatcherList,
        EInvalid = 1000
        };

private:    // Test case functions

    void Setup_L();
    void SetupMultiple_L();
    void BindL();
    void BindAllL();
    void Teardown();
    void UnbindL();
    void UnbindAllL();
    void SubscribeL();
    void UnsubscribeL();

    void T_BindUnbind_L();
    void T_SubscribePresenceWatcherList_Single_L();
    void T_SubscribePresenceWatcherList_Multiple_L();
    void T_HandlePresenceWatcherList_L();
    void T_IndiaCase_L();
    void SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, 
    TXIMPTestStatusEventTemplate aEventType );

    void SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, 
            TXIMPTestStatusEventTemplate aEventType );

    //TBool ValidateWatcherListEvent( RXIMPObjOwningPtrArray<CPresenceWatcherInfoImp>& aExpectedArray, TValidateType aType );


private: // From MXIMPContextObserver

    void HandlePresenceContextEvent(
                const MXIMPContext& aContext,
                const MXIMPBase& aEvent );


private:    // Test helpers

    /**
     * Fake a server-originated (=mobile terminated) message.
     * The message appears to adaptation and framework as if
     * it came from the network. Uses only one group.
     * @param aUri The URI of group
     * @param aDispName The display name for the group
     * @param aMsgType The message type for CXIMPTestFileSrvMsg
     */
    void SendSrvMsgL(
            const TDesC& aUri, const TDesC& aDispName,
            TInt aMsgType );

    /**
     * Fake a server-originated message with empty content.
     * Whether to use this depends on the event you expect.
     * @see SendSrvMsgL
     */
    void SendSrvMsgL( TInt aMsgType );

    // @see below
    enum TTestPWlOperation
        {
        ETestPWlNew = 0,       // new watchers
        ETestPWlCurrent,        // current watchers
        ETestPWlDisappeared    // disappeared watchers
        };


    /**
     * Creates an event for new watcher, current, or disappred presence watcher
     * list. The caller provides an identity and current presence watcher list.
     * Depending on the provided operation an event is created. 
     *
     * In cleanupstack there are 4 items, FIFO:
     *   - new list
     *   - current list
     *   - disappeared list
     *   - event imp
     *
     * This is because this event won't take ownership to the arrays
     * when created this way.
     *
     * @param aUri Uri
     * @param aDispName Displayname
     * @param aOperation operation for which caller wants to generates event
     * @param aCurrentList Current watcher list provided by caller, method takes
     *         ownership of the current list, and it is included in above 3 lists.
     *         a NULL pointer with operation ETestPWlCurrent causes empty event.
     * @return The suitably filled event.
     */
    CPresenceWatcherListEventImp* CreateWatcherListEventLCX(
            const TDesC& aUri,
            const TDesC& aDispName,
            MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
            MXIMPDataSubscriptionState::TDataState aDataState,
            TTestPWlOperation aOperation,
            RPrWatLstInfoImpArray* aCurrentList = NULL
            );


private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    // owned
    CXIMPTestContextWrapperMgr* iWrapperMgr;

    //
    TValidateType iLastEvent;
    TInt iLastError;
    RXIMPObjOwningPtrArray< CPresenceWatcherInfoImp > iValidateArray;
    };


#endif      //  T_PRESENCEWATCHERLISTMANAGEMENT_H



// end of file



