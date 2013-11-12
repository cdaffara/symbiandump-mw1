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

#ifndef T_PRESENCEGRANTREQUESTMANAGEMENT_H
#define T_PRESENCEGRANTREQUESTMANAGEMENT_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"
#include "ximpdatasubscriptionstateimp.h"
#include "presencetypehelpers.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;
class CPresenceGrantRequestInfoImp;
class CPresenceGrantRequestListEventImp;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_PresenceGrantRequestManagement : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceGrantRequestManagement* NewL();
    virtual ~T_PresenceGrantRequestManagement();


private:
    T_PresenceGrantRequestManagement();
    void ConstructL();

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

    void T_SubscribeGrantRequestList_Single_L();
    void T_SubscribeGrantRequestListRefresh_Single_L();
    void T_SubscribeGrantRequestList_Multiple_L();
    void T_HandlePresenceGrantRequestList_L();
    void T_HandlePresenceGrantRequestReceivedObsoleted_L();

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

    // version for sending just the info instead of array
    void SendSrvMsgInfoL(
            const TDesC& aUri, const TDesC& aDispName,
            TInt aMsgType );
    /**
     * Fake a server-originated message with empty content.
     * Whether to use this depends on the event you expect.
     * @see SendSrvMsgL
     */
    void SendSrvMsgL( TInt aMsgType );

    // @see below
    /*
    enum TTestPGRArraySpecifier //Waqas: Not used anymore, use the next one
        {
        ETestPGRNewList = 0,    // grant request list
        ETestPGRNewReq,         // new grant request
        ETestPGRPending,         // new grant request
        ETestPGRObsoleted,      // obsoleted grant request
        ETestPGREmpty,          // make an empty array for empty event
        };*/
        
    enum TTestPGROperation
        {
        ETestPGRNewReq = 0,     // new grant request i.e. new req comes
        ETestPGRPending,        // new grant request i.e. subscribe to
        ETestPGRObsoleted      // obsoleted grant request i.e. req expired                        
        };

    /**
     * Creates an event for newRequest, pending, or obselete presence grant
     * request list. The caller provides an identity and current presence grant
     * request listand depending on the provided operation an event is created. 
     *
     * In cleanupstack there are 4 items, FIFO:
     *   - new list
     *   - pending list
     *   - obsoleted list
     *   - event imp
     *
     * This is because this event won't take ownership to the arrays
     * when created this way.
     *
     * @param aUri Uri
     * @param aDispName Displayname
     * @param aOperation operation for which caller wants to generates event
     * @param aPendingList Current pending list provided by caller, method takes
     *         ownership of the pending list, and it is included in above 3 lists.
     *         a NULL pointer with operation ETestPGRPending causes empty event.
     * @return The suitably filled event.
     */
    CPresenceGrantRequestListEventImp* CreateGrantRequestListEventLCX(
            const TDesC& aUri,
            const TDesC& aDispName,
            MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
            MXIMPDataSubscriptionState::TDataState aDataState,
            TTestPGROperation aOperation,
            RPrGrntReqInfoImpArray* aPendingList = NULL
            );

private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    // owned
    CXIMPTestContextWrapperMgr* iWrapperMgr;

    //
    TInt iLastError;
    RXIMPObjOwningPtrArray< CPresenceGrantRequestInfoImp > iValidateArray;
    };


#endif      //  T_PRESENCEGRANTREQUESTMANAGEMENT_H



// end of file



