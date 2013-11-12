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
* Description:
*
*/

#ifndef T_PRESENCEBLOCKING_H
#define T_PRESENCEBLOCKING_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"
#include "ximpdatasubscriptionstateimp.h"
#include "presencetypehelpers.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;
class CPresenceBlockInfoImp;
class CPresenceBlockListEventImp;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_PresenceBlocking : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceBlocking* NewL();
    virtual ~T_PresenceBlocking();


private:
    T_PresenceBlocking();
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

    void T_SubscribeBlockList_Single_L();
    void T_SubscribeBlockListRefresh_Single_L();
    void T_SubscribeBlockList_Multiple_L();
    void T_HandlePresenceBlockList_L();
    void T_BlockPresence_L();
    void T_CancelBlockedPresence_L();
    void T_HandlePresenceBlocked_L();
    void T_HandlePresenceBlockCanceled_L();
    

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
    void SendBlockedSrvMsgL(
            const TDesC& aUri, const TDesC& aDispName,
            TInt aMsgType );
    void SendCanceledSrvMsgL(
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
    enum TTestPBlArraySpecifier   //Waqas: Not used anymore, use next one
        {
        ETestPBlNewList = 0,    // new block list
        ETestPBlAdded,     // added block
        ETestPBlCurrent,        // current block list
        ETestPBlRemoved,        // removed block
        ETestPBlUpdated,        // updated block
        ETestPBlEmpty,          // make an empty array for empty event
        };*/
        
    enum TTestPBlOperationSpecifier
        {
        ETestPBlAdded = 0,      // added block
        ETestPBlCurrent,        // current block list
        ETestPBlRemoved,        // removed block
        ETestPBlUpdated        // updated block
        };

    /**
     * Creates an event for adding, current, removing or updating
     * block list. The caller provides an identity and current blocked list
     * and depending on the provided operation an event is created. 
     *
     * In cleanupstack there are 4 items, FIFO:
     *   - created list
     *   - updated list
     *   - deleted list
     *   - event imp
     *
     * This is because this event won't take ownership to the arrays
     * when created this way.
     *
     * @param aUri Uri
     * @param aDispName Displayname
     * @param aOperation operation for which caller wants to generates event
     * @param aCurrentList Current blocked list provided by caller, method takes
     *         ownership of the current list, and it is included in above 4 items. 
     *         a NULL pointer with operation ETestPBlCurrent causes empty event. 
     * @return The suitably filled event.     
     */
    CPresenceBlockListEventImp* CreateBlockListEventLCX(
            const TDesC& aUri,
            const TDesC& aDispName,
            MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
            MXIMPDataSubscriptionState::TDataState aDataState,
            TTestPBlOperationSpecifier aOperation,
            RPrBlockInfoImpArray* aCurrentList = NULL
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
    RXIMPObjOwningPtrArray< CPresenceBlockInfoImp > iValidateArray;
    };


#endif      //  T_PRESENCEBLOCKING_H



// end of file



