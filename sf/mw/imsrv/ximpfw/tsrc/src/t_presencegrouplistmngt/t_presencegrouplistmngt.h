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

#ifndef T_PRESENCEGROUPLISTMANAGEMENT_H
#define T_PRESENCEGROUPLISTMANAGEMENT_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;
class CPresentityGroupInfoImp;
class CPresentityGroupListEventImp;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_PresenceGroupListManagement : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceGroupListManagement* NewL();
    virtual ~T_PresenceGroupListManagement();


private:
    T_PresenceGroupListManagement();
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
    void SubscribeGroupL( const TDesC& aGroupId );
    void UnsubscribeGroupL( const TDesC& aGroupId );
    void UnsubscribeL();

    void T_SubscribePresentityGroup_Single_L();
    void T_UnsubscribePresentityGroup_Error_Single_L();
    void T_SubscribePresentityGroup_Error_Single_L();
    void T_SubscribePresentityGroup_Multiple_L();
    void T_UpdateDisplayName_Single_L();
    void T_HandlePresentityGroupList_L();
    void T_HandlePresentityGroupList_Subscribed_L();
    void T_HandlePresentityGroupCreateAndDelete_L();
    void T_HandlePresentityGroupUpdate_L();
    void T_HandlePresentityGroupUpdate_WithHelpers_L();
    void T_SubscribePresentityGroup_One_Multiple_L();
    void T_RefreshPresentityGroup_One_Multiple_L();
    void T_UnsubNonsubscribed_Single_L();
    void T_RefreshPresentityGroup_Multiple_L();
    void T_SubscribeAll_One_Unsub_Multiple_L();

    void T_UpdateDisplayNameL( 
            const TDesC& aGroupId, 
            const TDesC& aDispName );

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

private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    // owned
    CXIMPTestContextWrapperMgr* iWrapperMgr;
    
    // 
    TInt iLastError;
    RXIMPObjOwningPtrArray< CPresentityGroupInfoImp > iValidateArray;
    };


#endif      //  T_PRESENCEGROUPLISTMANAGEMENT_H



// end of file



