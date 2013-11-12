/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CFCLIENT_H__
#define __MT_CFCLIENT_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <cflistener.h>
#include <f32file.h>
#include <cfcontextsourceinterface.h>
#include <cfcontextsourcecommand.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CCFClient;
class CCFContextObject;
class CTestContextData;
class CCFTestDelay;
class CCFKeyValuePair;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
#define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( MT_CFClient ):
public CEUnitTestSuiteClass,
public MCFListener, public MCFContextSource
    {
public: // Constructors and destructors

    /**
     * Two phase construction
     */
    static MT_CFClient* NewL();
    static MT_CFClient* NewLC();
    /**
     * Destructor
     */
    ~MT_CFClient();

private: // Constructors and destructors

    MT_CFClient();
    void ConstructL();

public: // From observer interface

    // @see MCFListener
    void ContextIndicationL( const CCFContextIndication& aChangedContext );

    // @see MCFListener
    void ActionIndicationL( const CCFActionIndication& aActionToExecute );

    // @see MCFListener
    void HandleContextFrameworkError( TCFError aError,
        const TDesC& aSource,
        const TDesC& aType );

    // @see MCFListener
    TAny* Extension( const TUid& aExtensionUid ) const;

public: // from MCFContextSource

    // from MCFContextSource
    void Subscribers( const TDesC& aContextSource,
        const TDesC& aContextType );

    // from MCFContextSource
    void NoSubscribers( const TDesC& aContextSource,
        const TDesC& aContextType );

    // from MCFContextSource
    void HandleCommand( const CCFContextSourceCommand& aCommand );

private: // New methods

    void ConfigureContextL( CCFContextObject& aContext );
    void UpdateContextValueL( CCFContextObject& aContext );
    void UpdateContextTypeL( CCFContextObject& aContext );
    HBufC* ConfigureActionIdLC();
    void ResetCustomAction();

    HBufC8* LoadScriptFromFile( const TDesC& aFilePath );

    void SetupL();
    void SetupClientL();
    void SetupNewClientL();
    void SetupSourceSettingTestClientL();
    void Teardown();
    void TeardownAndScriptCleanup();

    void CheckContextIndicationL( const CCFContextIndication& aChangedContext );
    void CheckContextDataIndicationL( const CCFContextIndication& aChangedContext );

    TBool LastActionIdEquals( const TDesC& aId );
    void DeleteLastActionId();

    void CheckActivateSourceCommandParams(
            const RSourceCommandParameterArray& aParams );
    void CheckDeactivateSourceCommandParams(
            const RSourceCommandParameterArray& aParams );

    void MT_CCFClient_NewLL();
    void MT_CCFClient_NewLCL();
    void MT_CCFClient_DefineContextL();
    void MT_CCFClient_SubscribersNosubscribersL();
    void MT_CCFClient_SubscribersNosubscribersPluginL();
    void MT_CCFClient_SubscribersNosubscribersPlugin2L();
    void MT_CCFClient_SubscribersNosubscribersPlugin3L();
    void MT_CCFClient_ContextSourceCommandPluginL();
    void MT_CCFClient_ContextSourceCommandClientL();
    void MT_CCFClient_PublishContextL();
    void MT_CCFClient_PublishContextWithDataL();
    void MT_CCFClient_SubscribeContext1L();
    void MT_CCFClient_SubscribeContext2L();
    void MT_CCFClient_SubscribeContext3L();
    void MT_CCFClient_SubscribeContext4L();
    void MT_CCFClient_SubscribeContext5L();
    void MT_CCFClient_SubscribeContext6L();
    void MT_CCFClient_SubscribeContext7L();
    void MT_CCFClient_SubscribeContext8L();
    void MT_CCFClient_SubscribeContext9L();
    void MT_CCFClient_SubscribeContextFromDifferentClientsL();
    void MT_CCFClient_SubscribeContextCombinedL();
    void MT_CCFClient_SubscribeContextData1L();
    void MT_CCFClient_SubscribeContextData2L();
    void MT_CCFClient_SubscribeContextData3L();
    void MT_CCFClient_SubscribeContextData4L();
    void MT_CCFClient_SubscribeContextDataCombinedL();
    void MT_CCFClient_UnsubscribeContext1L();
    void MT_CCFClient_UnsubscribeContext2L();
    void MT_CCFClient_UnsubscribeContextCombinedL();
    void MT_CCFClient_RequestContext1L();
    void MT_CCFClient_RequestContext2L();
    void MT_CCFClient_RequestContext3L();
    void MT_CCFClient_RequestContextCombinedL();
    void MT_CCFClient_RequestContextSetL();
    void MT_CCFClient_DefineActionL();
    void MT_CCFClient_SubscribeAction1L();
    void MT_CCFClient_SubscribeAction2L();
    void MT_CCFClient_SubscribeAction3L();
    void MT_CCFClient_SubscribeAction4L();
    void MT_CCFClient_SubscribeActionFromDifferentClientsL();
    void MT_CCFClient_SubscribeActionFromDifferentClients2L();
    void MT_CCFClient_SubscribeActionCombinedL();
    void MT_CCFClient_UnsubscribeActionL();
    void MT_CCFClient_RegisterScriptL();
    void MT_CCFClient_DeregisterScriptL();
    void MT_CCFClient_RegisterScriptDesL();
    void MT_CCFClient_SaveContextSourceSettingL();
    void MT_CCFClient_SaveContextSourceSetting2L();
    void MT_CCFClient_DeleteContextSourceSettingL();
    void MT_CCFClient_DeleteContextSourceSetting2L();
    void MT_CCFClient_UpgradeContextSourceSettingL();
    void MT_CCFClient_SaveScriptL();
    void MT_CCFClient_DeleteScriptL();
    void MT_CCFClient_DeleteScriptsL();
    void MT_CCFClient_UpgradeRomScriptL();
    void MT_CCFClient_RestoreRomScriptL();
    void MT_CCFClient_OperationUnsubscribingL();
    void MT_CCFClient_RomScriptContextAutoDefineL();
    void MT_CCFClient_RomScriptContextAutoDefine2L();
    void MT_CCFClient_RomScriptContextAutoDefine3L();
    void MT_CCFClient_RomScriptContextAutoDefine4L();

private: // Data

    EUNIT_DECLARE_TEST_TABLE;

    CCFClient* iClient;
    CCFClient* iAnotherClient;
    CCFContextObject* iContextA;
    CCFContextObject* iContextB; // This context can be used to indicate subscription errors
    CCFContextObject* iContextC; // This context can only be used to indicate subscription errors
    CActiveSchedulerWait* iWait;
    TBool iCheckA;
    TBool iCheckB;
    TBool iCheckC;
    TBool iCheckRegisterScriptAction;
    TBool iDeleteClient;
    TBool iCountActionAIndications;
    TInt iExpectedActionAIndicationCount;
    RFs iFs;
    TBool iCheckContextData;
    CTestContextData* iContextDataA;
    CTestContextData* iContextDataB;

    TBool iReturnContextSourceInterface;

    TBool iCheckSubscribersNoSubscribersCalled;
    TBool iSubscribersCalled;
    TBool iNoSubscribersCalled;
    TBuf<100> iExpectedSubscribersSource;
    TBuf<100> iExpectedSubscribersType;

    TBool iCheckPluginSubscribersCalled;
    TBool iPluginSubscribersCalled;
    TBuf<100> iExpectedSubscribersValue;
    CCFTestDelay* iTestDelay;

    TBuf<100> iCustomActionId;
    TBool iCheckCustomAction;
    RPointerArray<CCFKeyValuePair> iCustomActionParams;
    TBool iCustomCheckOk;

    HBufC* iLastActionId;
    TBool iCheckSourceCommandHandled;
    TBool iSourceCommandHandled;
    TBool iHandleCommandCalled;
    RArray< TCFSourceCommandSenderId > iActiveRequests;

    TBool iCheckRomScriptContextAutoDefine;
    TBool iCheckRomScriptContextAutoDefineDetected;
    };

#endif      //  __MT_CFCLIENT_H__

