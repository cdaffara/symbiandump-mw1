/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#include "mt_atspc.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <remcon/remcontargetselectorplugin.h>
#include <ecom/ecom.h>
#include <remcon/remcontargetselectorplugininterface.h>
#include <remconaddress.h>
#include <remcon/clientinfo.h>
#include <remcon/bearersecurity.h>
#include <remconextensionapi.h>
#include <tspclientmapper.h>
#include <remconcoreapi.h>
#include <cfclient.h>

static const TInt KAdvancedTspControllerUid = 0x10205045;

//  INTERNAL INCLUDES
NONSHARABLE_CLASS( TWaitInfo )
    {
    public:
    
        CPeriodic* iPeriodic;
        CActiveSchedulerWait* iWait;
    };

/**
* Call back method when we need to stop active scheduler wait.
*/
LOCAL_C TInt WaitCallBack( TAny* aSelf )
    {
    if( aSelf )
        {
        TWaitInfo* info = static_cast<TWaitInfo*>( aSelf );
        if( info->iPeriodic )
            {
            info->iPeriodic->Cancel();
            }
        if( info->iWait )
            {
            if( info->iWait->IsStarted() )
                {
                info->iWait->AsyncStop();
                }
            }
        }
    
    return KErrNone;
    }

/**
* Helper method to wait current scheduler before teardown is completed.
*/
LOCAL_C void Wait( TInt aIntervalInMicorseconds )
    {
    TWaitInfo info;
    
    // Construct periodic
    CPeriodic* periodic = CPeriodic::NewL( CActive::EPriorityStandard );
    CleanupStack::PushL( periodic );
    info.iPeriodic = periodic;
    
    // Construct active scheduler wait
    CActiveSchedulerWait* wait = new( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    info.iWait = wait;
    
    // Start timer and wait
    TCallBack cb( WaitCallBack, &info );
    periodic->Start( aIntervalInMicorseconds, aIntervalInMicorseconds, cb );
    wait->Start();
    
    // Cleanup
    CleanupStack::PopAndDestroy( wait );
    CleanupStack::PopAndDestroy( periodic );
    }

// - Construction -----------------------------------------------------------

mt_atspc* mt_atspc::NewL()
    {
    mt_atspc* self = mt_atspc::NewLC();
    CleanupStack::Pop();
    return self;
    }

mt_atspc* mt_atspc::NewLC()
    {
    mt_atspc* self = new( ELeave ) mt_atspc();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

mt_atspc::~mt_atspc()
    {
    }

mt_atspc::mt_atspc()
    {
    }

void mt_atspc::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void mt_atspc::SetupL(  )
    {

    // publish context so that rule tables are loaded
    CCFClient* cfClient = CCFClient::NewLC( *this );
    
    _LIT( KSource, "Tsp" );
    _LIT( KType, "Status" );
    //_LIT( KValue, "Ready" );
    _LIT( KValue, "Notready" ); // invalidate context so that tables are loaded upon plugin initialization
    CCFContextObject* obj = CCFContextObject::NewL( KSource, KType, KValue );
    cfClient->PublishContext( *obj );
    CleanupStack::PopAndDestroy( cfClient );
    delete obj;
    
    
    iAtspc = CRemConTargetSelectorPlugin::NewL( TUid::Uid( KAdvancedTspControllerUid ), *this);
    // ensure that implements all interfaces
    iTspIf2 = static_cast<MRemConTargetSelectorPluginInterfaceV2*>( iAtspc->GetInterface( TUid::Uid( KRemConTargetSelectorInterface2 ) ) );
    EUNIT_ASSERT_DESC( iTspIf2, "Interface V2 support" );
    EUNIT_ASSERT( iAtspc->GetInterface( TUid::Uid( KRemConTargetSelectorInterface1 ) ) ); // check also interface 1
    
    // rule tables are loaded asynchronously, wait until context indication
    Wait( 1000000L ); // microseconds 
    
    // keep screensaver away
    User::ResetInactivityTime();

    }
    

void mt_atspc::Teardown(  )
    {
    delete iAtspc;
    iAtspc = NULL;
    REComSession::FinalClose();
    }

void mt_atspc::T_TestAddressIncomingCommandCoreApiL()
    {
    /** Rules assumed by this test. 
    <Tsp
    routingTable='01'
    keyEventRemConVolumeUp='41'
    keyEventRemConVolumeDown='42'
    />
    
    <Tsp
    routingTable='01'
    ruleForeground='01'
    ruleActiveAudio='00'
    ruleLastPlayedAudio='03'
    ruleDefaultApp='02'
    defaultApplicationPhone='100058B3'
    />
                        
    <Tsp
    routingTable='09'
    keyEventRemConF4='74'            
    />
    
    <Tsp
    routingTable='09'
    ruleDefaultLaunchApp='04'
    defaultLaunchApp='EFFFFFFF'
    />
    */
    TSglQue<TClientInfo> clients;
    RProcess thisProc;
    TClientInfo thisClient;
    thisClient.ProcessId() = thisProc.Id();
    TProcessId fakeId( thisProc.Id() + 100 );
    TClientInfo fakeClient;
    fakeClient.ProcessId() = fakeId.Id();
    TClientInfo anotherFakeClient;
    anotherFakeClient.ProcessId() = fakeId.Id() + 100;
    TClientInfo fakeClientPhone;
    fakeClientPhone.ProcessId() = fakeId.Id() + 200;
    fakeClientPhone.SecureId() = TUid::Uid( 0x100058B3 );
    
    // 1. Test the foreground application rule
    clients.AddLast( thisClient );
    clients.AddLast( fakeClient );
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
    
    EUNIT_ASSERT( iObserverResponse == KErrNone );
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go to foreground application" );
    TClientInfo* client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == thisProc.Id(), "Found client id not matching" );
    
    // 2. Test the active audio rule
    // current process is not added to list because events must not fall to foreground application
    clients.Reset();
    clients.AddLast( fakeClient );
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    
    CTspClientMapper* tspCm = CTspClientMapper::NewLC(); // register fake client to TSP client mapper, so it is considered as active audio
    tspCm->SetTspTargetClient( CTspClientMapper::EPlayingClients, fakeId );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
    tspCm->RemoveTspTargetClient( CTspClientMapper::EPlayingClients, fakeId );
    CleanupStack::PopAndDestroy( tspCm );
    
    EUNIT_ASSERT( iObserverResponse == KErrNone );
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go to the active audio application" );
    client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == fakeId, "Found client id not matching" );
    
    // 3. Test the last played application rule
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClient );
    clients.AddLast( fakeClientPhone );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
    EUNIT_ASSERT( iObserverResponse == KErrNone );    
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go to last played application" );
    client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == fakeId, "Found client id not matching" );
    
    // 4. Test the default application (=Phone) rule
    // drop the 1st fake client so we won't fall to last played application rule
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
    EUNIT_ASSERT( iObserverResponse == KErrNone );
            
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go the default application Phone" );
    client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == fakeClientPhone.ProcessId(), "Found client id not matching" );
    
    // 5. Test an key event that does not have a rule assigned to it
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiF5, clients );
    EUNIT_ASSERT( iObserverResponse == KErrNone );    
    
    // 6. Test a key event that attempts to launch an (non-existing) application
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiF4, clients );
    EUNIT_ASSERT( iObserverResponse == KErrNone );
    }

void mt_atspc::T_TestAddressIncomingCommandSynchApiL()
    {
    // No rules for synchronization API in default configuration, just call the function and verify response
    TSglQue<TClientInfo> clients;
    RProcess thisProc;
    TClientInfo thisClient;
    thisClient.ProcessId() = thisProc.Id();
    clients.AddLast( thisClient );
    
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConExtSynchronizationApiUid), ERemConExtSynchronization, clients ); 
    EUNIT_ASSERT( iObserverResponse == KErrNone );

    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client list resetted" );
    TClientInfo* client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == thisProc.Id(), "Found client id not matching - rule file changed or corrupt?" );
    }

void mt_atspc::T_TestAddressIncomingCommandPocApiL()
    {
    /** Rules assumed by this test.
    <Tsp
     routingTable='07'
     keyEventRemConExtPocButtonUp='01'
     />
            
     <Tsp
     routingTable='07'
     ruleForeground='01'
     defaultApplicationPhone='100058B3'
     /> */
     
    TSglQue<TClientInfo> clients;
    RProcess thisProc;
    TClientInfo thisClient;
    thisClient.ProcessId() = thisProc.Id();
    clients.AddLast( thisClient );
    
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConExtPocApiUid), ERemConExtAPOCButtonUp, clients ); // test only one key event
    EUNIT_ASSERT( iObserverResponse == KErrNone );

    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client list resetted" );
    TClientInfo* client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == thisProc.Id(), "Found client id not matching - rule file changed or corrupt?" );
    }

void mt_atspc::T_TestAddressIncomingCommandCallHandlingApiL()
    {
    /** Rules assumed by this test. 
    <Tsp
    routingTable='04'
    keyEventRemConExtCallAnswerCall='00'
    keyEventRemConExtCallEndCall='01'
    keyEventRemConExtCallAnswerEnd='02'
    />
    
    <Tsp
    routingTable='04'
    ruleForeground='01'
    ruleActiveAudio='00'
    ruleLastPlayedAudio='03'
    />
    */
    TSglQue<TClientInfo> clients;
    RProcess thisProc;
    TClientInfo thisClient;
    thisClient.ProcessId() = thisProc.Id();
    TProcessId fakeId( thisProc.Id() + 100 );
    TClientInfo fakeClient;
    fakeClient.ProcessId() = fakeId.Id();
    TClientInfo anotherFakeClient;
    anotherFakeClient.ProcessId() = fakeId.Id() + 100;
    
    // 1. Test the foreground application rule
    clients.AddLast( thisClient );
    clients.AddLast( fakeClient );
    clients.AddLast( anotherFakeClient );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConExtCallHandlingApiUid), ERemConExtAnswerCall, clients ); // test only one key event
    
    EUNIT_ASSERT( iObserverResponse == KErrNone );
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go to foreground application" );
    TClientInfo* client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == thisProc.Id(), "Found client id not matching" );
    
    // 2. Test the active audio rule
    // current process is not added to list because events must not fall to foreground application
    clients.Reset();
    clients.AddLast( fakeClient );
    clients.AddLast( anotherFakeClient );
    
    CTspClientMapper* tspCm = CTspClientMapper::NewLC(); // register fake client to TSP client mapper, so it is considered as active audio
    tspCm->SetTspTargetClient( CTspClientMapper::EPlayingClients, fakeId );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConExtCallHandlingApiUid), ERemConExtAnswerCall, clients ); // test only one key event
    tspCm->RemoveTspTargetClient( CTspClientMapper::EPlayingClients, fakeId );
    CleanupStack::PopAndDestroy( tspCm );
    
    EUNIT_ASSERT( iObserverResponse == KErrNone );
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go to the active audio application" );
    client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == fakeId, "Found client id not matching" );
    
    // 3. Test the last played application rule
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClient );
    iObserverResponse = KErrGeneral;
    iTspIf2->AddressIncomingCommand( TUid::Uid(KRemConExtCallHandlingApiUid), ERemConExtAnswerCall, clients ); // test only one key event
    
    EUNIT_ASSERT( iObserverResponse == KErrNone );
    EUNIT_ASSERT_DESC( !clients.IsEmpty(), "Client not found, should go to last played application" );
    client = clients.Last();
    EUNIT_ASSERT_DESC( client->ProcessId() == fakeId, "Found client id not matching" );
    }

void mt_atspc::T_TestAddressOutgoingCommandL()
    {
    RProcess thisProc;
    TBearerSecurity bearer( thisProc.SecureId(), TSecurityPolicy() );
    TSglQue<TBearerSecurity> bearerSecurity;
    bearerSecurity.AddLast( bearer );
    
    TSglQue<TRemConAddress> connections;
    TClientInfo client;
    client.SecureId() = thisProc.SecureId();
      
    iObserverResponse = KErrGeneral;
    
    iTspIf2->AddressOutgoingCommand( TUid(), 0,  client, connections, bearerSecurity );
           
    EUNIT_ASSERT_DESC( iObserverResponse == KErrNone, "AddressOutgoingCommand response not KErrNone")
    EUNIT_ASSERT_DESC( !connections.IsEmpty(), "No connection added" );
    
    // if successful, item from bearerSecurity (allocated to the heap) has been added to list
    TRemConAddress* conn = connections.Last();
    
    EUNIT_ASSERT( conn );
    
    EUNIT_ASSERT_DESC( conn->BearerUid() == thisProc.SecureId(), "UIDs not matching" );
    
    delete conn;
    }

void mt_atspc::T_TestAddressIncomingNotifyL()
    {
    TClientInfo client;
    TSglQue<TClientInfo> clients;
    
    clients.AddLast( client );
       
    iObserverResponse = KErrGeneral;
       
    iTspIf2->AddressIncomingNotify( TUid(), 0,  clients );
       
    EUNIT_ASSERT_DESC( iObserverResponse == KErrNone, "AddressIncomingNotify response not KErrNone")
    }

void mt_atspc::T_TestPermitOutgoingCommandL()
    {
    iObserverResponse = KErrGeneral;
    
    // no implementation logic for this in advancedtspcontroller, should permit all
    iTspIf2->PermitOutgoingCommand( TUid(), 0, TClientInfo(), TRemConAddress() );
    
    EUNIT_ASSERT_DESC( iObserverResponse, "PermitOutgoingCommand response not ETrue")
    }
  
void mt_atspc::T_TestCancelOutgoingCommandL()
    {
    iObserverResponse = ETrue; // should not be changed by CancelOutgoinCommand
    
    // no implementation logic for this in advancedtspcontroller
    iTspIf2->CancelOutgoingCommand();
    
    EUNIT_ASSERT_DESC( iObserverResponse, "CancelOutgoingCommand response not ETrue")
    }

void mt_atspc::T_TestPermitOutgoingResponseL()
    {
    TSglQue<TClientInfo> client;
    TClientInfoConstIter clients( client );
       
    iObserverResponse = KErrGeneral;
       
    // no implementation logic for this in advancedtspcontroller, should permit all
    iTspIf2->PermitOutgoingResponse( TUid(), 0, TClientInfo(), clients );
       
    EUNIT_ASSERT_DESC( iObserverResponse, "PermitOutgoingResponse response not ETrue")
    }

void mt_atspc::T_TestCancelOutgoingResponseL()
    {
    iObserverResponse = ETrue; // should not be changed by CancelOutgoingResponse
    
    // no implementation logic for this in advancedtspcontroller
    iTspIf2->CancelOutgoingResponse();
    
    EUNIT_ASSERT_DESC( iObserverResponse, "CancelOutgoingResponse response not ETrue")
    }




// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    mt_atspc,
    "Module tests for advancedtspcontroller.",
    "MODULE" )

EUNIT_TEST(
    "TestAddressIncomingCommandCoreApiL",
    "",
    "TestAddressIncomingCommandCoreApiL",
    "FUNCTIONALITY",
    SetupL, T_TestAddressIncomingCommandCoreApiL, Teardown)

EUNIT_TEST(
    "TestAddressIncomingCommandSynchApi",
    "",
    "TestAddressIncomingCommandSynchApi",
    "FUNCTIONALITY",
    SetupL, T_TestAddressIncomingCommandSynchApiL, Teardown)
    
EUNIT_TEST(
    "TestAddressIncomingCommandPocApiL",
    "",
    "TestAddressIncomingCommandPocApiL",
    "FUNCTIONALITY",
    SetupL, T_TestAddressIncomingCommandPocApiL, Teardown)

EUNIT_TEST(
    "TestAddressIncomingCommandCallHandlingApiL",
    "",
    "TestAddressIncomingCommandCallHandlingApiL",
    "FUNCTIONALITY",
    SetupL, T_TestAddressIncomingCommandCallHandlingApiL, Teardown)
    
EUNIT_TEST(
    "TestAddressOutgoingCommandL",
    "",
    "TestAddressOutgoingCommandL",
    "FUNCTIONALITY",
    SetupL, T_TestAddressOutgoingCommandL, Teardown)

EUNIT_TEST(
    "TestAddressIncomingNotifyL",
    "",
    "TestAddressIncomingNotifyL",
    "FUNCTIONALITY",
    SetupL, T_TestAddressIncomingNotifyL, Teardown)

EUNIT_TEST(
    "TestPermitOutgoingCommandL",
    "",
    "TestPermitOutgoingCommandL",
    "FUNCTIONALITY",
    SetupL, T_TestPermitOutgoingCommandL, Teardown)
    
EUNIT_TEST(
    "TestCancelOutgoingCommand",
    "",
    "TestCancelOutgoingCommand",
    "FUNCTIONALITY",
    SetupL, T_TestCancelOutgoingCommandL, Teardown)
    
EUNIT_TEST(
    "TestPermitOutgoingResponseL",
    "",
    "TestPermitOutgoingResponseL",
    "FUNCTIONALITY",
    SetupL, T_TestPermitOutgoingResponseL, Teardown)
    
EUNIT_TEST(
    "TestCancelOutgoingResponse",
    "",
    "TestCancelOutgoingResponse",
    "FUNCTIONALITY",
    SetupL, T_TestCancelOutgoingResponseL, Teardown)
    

EUNIT_END_TEST_TABLE
