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
* Description:  Test Protocol implementation for XIMP Framework
 *
*/


#include <badesca.h>
#include <ecom/implementationproxy.h>

#include <ximpstatus.h>
#include <ximpserviceinfo.h>
#include <ximpcontextclientinfo.h>

#include <ximpprotocolconnectionhost.h>
#include <protocolpresencepublishingdatahost.h>
#include <protocolpresencefeatures.h>
#include <ximprestrictedobjectcollection.h>
#include <ximpidentity.h>
#include <presentitygroupinfo.h>
#include <presentitygroupmemberinfo.h>
#include <ximpobjectfactory.h>
#include <ximpprotocolrequestcompleteevent.h>
#include <presencefeatures.h>
#include <presenceobjectfactory.h>
#include <protocolpresencedatahost.h>


#include "pr_prfwtestprotocol.h"
#include "prfwgeneralwaiter.h"
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestfilelogichelper.h"
#include "prfwtestfiletool.h"
#include "ximprequestidbuilder.h"
#include "prfwtestprotocols.h"



const TInt KWaitTime = 1;

T_XIMPTestConnection::ClientWrapper* T_XIMPTestConnection::ClientWrapper::NewLC( const TDesC& aClientId )
    {
    ClientWrapper* self = new ( ELeave ) ClientWrapper;
    CleanupStack::PushL( self );
    self->ConstructL( aClientId );
    return self;
    }

void T_XIMPTestConnection::ClientWrapper::ConstructL( const TDesC& aClientId )
    {
    iId.CreateL( aClientId );
    }

void T_XIMPTestConnection::ClientWrapper::InitializeL( MXIMPProtocolConnectionHost& aConnectionHost )
    {
    if( !iMessenger )
        {
        TLex lex( iId );
        TInt index = 0;
        User::LeaveIfError( lex.Val( index ) );
        // TEST CODE
        __ASSERT_ALWAYS( index != -1, User::Panic(_L("Protocol"), 666));
        iMessenger = CXIMPTestMessenger::NewForAdaptationSideL( index );
        iPluginIndex = index;
        }

    if( !iFileLogicHelper )
        {
        iFileLogicHelper = CXIMPTestFileLogicHelper::NewL( aConnectionHost, *iMessenger );
        }

    if( !iFileTool )
        {
        // TODO FIX FILETOOL CREATION
        iFileTool = CXIMPTestFileTool::NewL( K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID, iId );
        iFileTool->RegisterObserverL( iFileLogicHelper );
        }
    }

T_XIMPTestConnection::ClientWrapper::~ClientWrapper()
    {
    if( iFileTool && iFileLogicHelper )
        {
        iFileTool->UnregisterObserver( iFileLogicHelper );
        }
    delete iFileTool;
    delete iFileLogicHelper;
    delete iMessenger;

    iId.Close();
    }

// ---------------------------------------------------------------------------
// OrderOfClients()
// ---------------------------------------------------------------------------
//
TInt T_XIMPTestConnection::OrderOfClients( const T_XIMPTestConnection::ClientWrapper& aFirst,
                                           const T_XIMPTestConnection::ClientWrapper& aSecond )
    {
    return aFirst.iId.Compare( aSecond.iId );
    }

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// OrderOfConnections()
// ---------------------------------------------------------------------------
//
TInt T_XIMPTestConnection::OrderOfConnections( const T_XIMPTestConnection& aFirst,
                                               const T_XIMPTestConnection& aSecond )
    {
    TInt retVal = aFirst.iServerAddress->Compare( *aSecond.iServerAddress );
    if( retVal != 0 )
        {
        return retVal;
        }
    retVal = aFirst.iUserName->Compare( *aSecond.iUserName );
    if( retVal != 0 )
        {
        return retVal;
        }

    return aFirst.iPassword->Compare( *aSecond.iPassword );
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::T_XIMPTestConnection()
// ---------------------------------------------------------------------------
//
T_XIMPTestConnection::T_XIMPTestConnection()
    {
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::ConstructL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::ConstructL( const MXIMPServiceInfo& aServiceInfo,
                                       const MXIMPContextClientInfo& aClientCtxInfo )
    {
    iWaitter = CXIMPTestGeneralWaiter::NewL( this );

    iAuxTimer = CPeriodic::NewL( CActive::EPriorityIdle );

    iServerAddress = aServiceInfo.ServiceAddress().AllocL();
    iUserName = aServiceInfo.UserId().AllocL();
    iPassword = aServiceInfo.Password().AllocL();
    iClientId = aClientCtxInfo.ClientId().Identity().AllocL();
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::NewLC()
// ---------------------------------------------------------------------------
//
T_XIMPTestConnection* T_XIMPTestConnection::NewLC( const MXIMPServiceInfo& aServiceInfo,
                                                   const MXIMPContextClientInfo& aClientCtxInfo )
    {
    T_XIMPTestConnection* self = new( ELeave ) T_XIMPTestConnection(  );
    CleanupStack::PushL( self );
    self->ConstructL( aServiceInfo, aClientCtxInfo );
    return self;
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::NewL()
// ---------------------------------------------------------------------------
//
T_XIMPTestConnection* T_XIMPTestConnection::NewL( const MXIMPServiceInfo& aServiceInfo,
                                                  const MXIMPContextClientInfo& aClientCtxInfo )
    {
    T_XIMPTestConnection* self = T_XIMPTestConnection::NewLC( aServiceInfo, aClientCtxInfo );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::~T_XIMPTestConnection()
// ---------------------------------------------------------------------------
//
T_XIMPTestConnection::~T_XIMPTestConnection()
    {
    SetBoolean( EXIMPPlgTestDeath );

    if ( iAuxTimer )
        {
        iAuxTimer->Deque();
        }
    delete iAuxTimer;

    delete iWaitter;

    iClients.ResetAndDestroy();

    delete iServerAddress;
    delete iUserName;
    delete iPassword;
    delete iClientId;

    delete iClientToBeDeleted;
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PrimeHost()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::PrimeHost( MXIMPProtocolConnectionHost& aHost )
    {
    iConnectionHost = &aHost;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PresenceFeatures()
// ---------------------------------------------------------------------------
//
MProtocolPresenceFeatures& T_XIMPTestConnection::ProtocolPresenceFeatures()
    {
    return *this;
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PresenceFeatures()
// ---------------------------------------------------------------------------
//	MProtocolImFeatures* ProtocolImFeatures();
//MProtocolImFeatures* T_XIMPTestConnection::ProtocolImFeatures()
//    {
//    return NULL;
//    }
// ---------------------------------------------------------------------------
// T_XIMPTestConnection::GetProtocolInterface()
// ---------------------------------------------------------------------------
TAny* T_XIMPTestConnection::GetProtocolInterface(TInt aInterfaceId)
	{
	return NULL ;
	}
    
    
    
// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PresenceWatching()
// ---------------------------------------------------------------------------
//
MProtocolPresenceWatching& T_XIMPTestConnection::PresenceWatching()
    {
    return *this;
    }
// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PresencePublishing()
// ---------------------------------------------------------------------------
//
MProtocolPresencePublishing& T_XIMPTestConnection::PresencePublishing()
    {
    return *this;
    }
// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PresentityGroups()
// ---------------------------------------------------------------------------
//
MProtocolPresentityGroups& T_XIMPTestConnection::PresentityGroups()
    {
    return *this;
    }
// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PresenceAuthorization()
// ---------------------------------------------------------------------------
//
MProtocolPresenceAuthorization& T_XIMPTestConnection::PresenceAuthorization()
    {
    return *this;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::HandleProtocolConnectionHostEvent()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::HandleProtocolConnectionHostEvent(
    const MXIMPProtocolConnectionHost& aHost,
    const MXIMPBase& aEvent )
    {
    if( &aHost != iConnectionHost )
        {
        User::Panic(_L("Protocol"), -42 );
        }
    
    
    if( aEvent.GetInterfaceId() == MXIMPProtocolRequestCompleteEvent::KInterfaceId )
        {
        const MXIMPProtocolRequestCompleteEvent* eventIf =
        TXIMPGetInterface< const MXIMPProtocolRequestCompleteEvent >::From( aEvent,
                                                                            MXIMPBase::EPanicIfUnknown );
        RDebug::Print( _L("   T_XIMPTestConnection: received ProtocolRequestCompleteEvent with result [%d]"), 
                       eventIf->ResultCode() );
        }
    }
    
    

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::OpenSessionL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::OpenSessionL( const MXIMPContextClientInfo& aClientCtxInfo,
                                       TXIMPRequestId aReqId )
    {
    iConnectionHost->RegisterObserverL( *this );

    ClientWrapper* client = ClientWrapper::NewLC( aClientCtxInfo.ClientId().Identity() );

    TLinearOrder< T_XIMPTestConnection::ClientWrapper > order( T_XIMPTestConnection::OrderOfClients );
    TInt indexOfClient = iClients.FindInOrder( client, order );

    if( indexOfClient != KErrNotFound )
        {
        User::LeaveIfError( indexOfClient );
        CleanupStack::PopAndDestroy(); //id
        client = iClients[ indexOfClient ];
        }
    else
        {
        client->InitializeL( *iConnectionHost );
        iCurrentMessenger = client->iMessenger;
        }

    PluginAPIMethodCommonL( aReqId );

    if( indexOfClient == KErrNotFound )
        {
        iClients.InsertInOrderL( client, order );
        CleanupStack::Pop(); //id
        }


    SetBooleanToCurrent( EXIMPPlgTestOpenSessionCalled );
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::OpenSessionL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::OpenSessionL(
                const TInt& aSettingsId,
                TXIMPRequestId aReqId )

    {
    iConnectionHost->RegisterObserverL( *this );

    ClientWrapper* client = ClientWrapper::NewLC( _L("0") );

    TLinearOrder< T_XIMPTestConnection::ClientWrapper > order( T_XIMPTestConnection::OrderOfClients );
    TInt indexOfClient = iClients.FindInOrder( client, order );

    if( indexOfClient != KErrNotFound )
        {
        User::LeaveIfError( indexOfClient );
        CleanupStack::PopAndDestroy(); //id
        client = iClients[ indexOfClient ];
        }
    else
        {
        client->InitializeL( *iConnectionHost );
        iCurrentMessenger = client->iMessenger;
        }

    PluginAPIMethodCommonL( aReqId );

    if( indexOfClient == KErrNotFound )
        {
        iClients.InsertInOrderL( client, order );
        CleanupStack::Pop(); //id
        }


    SetBooleanToCurrent( EXIMPPlgTestOpenSessionCalled );
    iWaitter->WaitForL( KWaitTime );
    }
        
        

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::CloseSession()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::CloseSession( const MXIMPContextClientInfo& aClientCtxInfo,
                                         TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    iClientToBeDeleted = NULL;
    TInt count = iClients.Count();
    TBool found(EFalse);
    for( TInt a = 0; a < count && !iClientToBeDeleted; ++a )
        {
        if( aClientCtxInfo.ClientId().Identity().Compare( iClients[ a ]->iId ) == 0 )
            {
            iClientToBeDeleted = iClients[ a ];
            iClients.Remove( a );
            found = ETrue;
            }
        }

    SetBooleanToCurrent( EXIMPPlgTestCloseSessionCalled );

    if(found)
    iWaitter->WaitForL( KWaitTime );
    else
    	{
	    MXIMPStatus* status = iConnectionHost->ObjectFactory().NewStatusLC();
	    CleanupStack::Pop(); // status.
	    status->SetResultCode(KErrNone);
    	iConnectionHost->HandleRequestCompleted( aReqId, status);    		
    	}
    	
    
    	
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::GetSupportedFeaturesL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::GetSupportedFeaturesL( CDesC8Array& aFeatures ) const
    {
    if ( ! iClients.Count() )
        {
        User::LeaveIfError( KErrNotReady );
        }

    // first empty the whole array
    aFeatures.Reset();

    // then add two features there: 1 supported by XIMP, 1 not
    aFeatures.AppendL( _L8("feat/pres/pub") );
    aFeatures.AppendL( _L8("feat/worlddomination") );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoSubscribeOwnPresenceL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribeOwnPresenceL( const MPresenceInfoFilter& aPif,
        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestSubscribeOwnPresenceCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUpdateOwnPresenceSubscriptionL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUpdateOwnPresenceSubscriptionPifL( const MPresenceInfoFilter& aPif,
        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUpdateOwnSubscriptionCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }


// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoSubscribePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribePresenceWatcherListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestSubscribePresenceWatcherListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoUnsubscribePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribePresenceWatcherListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUnsubscribePresenceWatcherListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoSubscribePresentityGroupListL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribePresentityGroupListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestSubscribeGroupListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUnsubscribePresentityGroupListL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribePresentityGroupListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUnsubscribeGroupListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PublishPresenceL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoPublishOwnPresenceL(
        const MPresenceInfo& aPresence,
        TXIMPRequestId aReqId )
    {
    // With SetupLeave we leave here
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestPublishPresenceCalled );

    MProtocolPresenceDataHost& presHost = iConnectionHost->ProtocolPresenceDataHost();
    // Set own presence data in data cache
    MPresenceInfo* presence = presHost.PresenceObjectFactory().NewPresenceInfoLC();
    TXIMPRequestId reqid = presHost.PublishingDataHost().HandleSubscribedOwnPresenceL( presence );
    CleanupStack::Pop(); //presence

    
    if(iCurrentMessenger->GetError()==KErrTimedOut)
    	{
	    MXIMPStatus* status1 = iConnectionHost->ObjectFactory().NewStatusLC();
	    iConnectionHost->HandleConnectionTerminated(status1);

		CleanupStack::PopAndDestroy(1);
    	}

    iWaitter->WaitForL( KWaitTime );

    // TODO when implemented, write the data received for test case verification
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUnsubscribeOwnPresenceL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribeOwnPresenceL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUnsubscribeOwnPresenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoCreatePresentityGroupL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoCreatePresentityGroupL( const MXIMPIdentity& aIdentity,
                                           const TDesC16& aDisplayName,
                                           TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestCreatePresentityGroupCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoCreatePresentityGroupL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoDeletePresentityGroupL( const MXIMPIdentity& aIdentity,
                                           TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestDeletePresentityGroupCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoCreatePresentityGroupL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUpdatePresentityGroupDisplayNameL( const MXIMPIdentity& aIdentity,
                                           const TDesC16& aDisplayName,
                                           TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUpdatePresentityGroupDisplayNameCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoSubscribePresentityPresenceL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribePresentityPresenceL(
                                    const MXIMPIdentity& aIdentity,
                                    const MPresenceInfoFilter& aPif,
                                    TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestSubscribePresentityPresesenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUpdatePresentityPresenceSubscriptionL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUpdatePresentityPresenceSubscriptionPifL(
                                    const MXIMPIdentity& aIdentity,
                                    const MPresenceInfoFilter& aPif,
                                    TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUpdatePresentityPresesenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUnsubscribePresentityPresenceL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribePresentityPresenceL(
                                        const MXIMPIdentity& aIdentity,
                                        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUnsubscribePresentityPresesenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }


// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoSubscribePresentityGroupMembersPresenceL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribePresentityGroupMembersPresenceL(
                                    const MXIMPIdentity& aIdentity,
                                    const MPresenceInfoFilter& aPif,
                                    TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUpdatePresentityPresenceSubscriptionL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUpdatePresentityGroupMembersPresenceSubscriptionPifL(
                                    const MXIMPIdentity& aIdentity,
                                    const MPresenceInfoFilter& aPif,
                                    TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUpdatePresentityGroupMembersPresesenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::DoUnsubscribePresentityPresenceL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribePresentityGroupMembersPresenceL(
                                        const MXIMPIdentity& aIdentity,
                                        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }



// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoSubscribePresentityGroupContentL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribePresentityGroupContentL(
                                            const MXIMPIdentity& aGroupId,
                                            TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestSubscribePresentityGroupContentCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoUnsubscribePresentityGroupContentL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribePresentityGroupContentL(
                                            const MXIMPIdentity& aGroupId,
                                            TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );
    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUnsubscribePresentityGroupContentCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );

    }

// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoAddPresentityGroupMemberL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoAddPresentityGroupMemberL(
                                            const MXIMPIdentity& aGroupId,
                                            const MXIMPIdentity& aMemberId,
                                            const TDesC16& aMemberDisplayName,
                                            TXIMPRequestId aReqId )
    {
        /*
        //Req complete parameter template
        {
        MXIMPRestrictedObjectCollection* collection =
            iConnectionHost->ObjectFactory().NewRestrictedObjectCollectionLC();

        MXIMPIdentity* identityClone = iConnectionHost->ObjectFactory().NewIdentityLC();
        identityClone->SetIdentityL( aMemberId.Identity() );

        MPresentityGroupMemberInfo* grpMember =
        iConnectionHost->ObjectFactory().NewPresentityGroupMemberInfoLC( *identityClone,
                                                                         _L("AlteredDisplayname") );
        CleanupStack::Pop(); //grpMember
        CleanupStack::PopAndDestroy(); //identityClone

        CleanupDeletePushL( grpMember );
        collection->AddOrReplaceTypeL( grpMember );
        CleanupStack::Pop(); //grpMember

        delete iReqCompleteParams;
        iReqCompleteParams = NULL;
        iReqCompleteParams = collection;

        CleanupStack::Pop(); //collection
        }
        */

    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestAddPresentityGroupMemberCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoRemovePresentityGroupMemberL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoRemovePresentityGroupMemberL(
                                            const MXIMPIdentity& aGroupId,
                                            const MXIMPIdentity& aMemberId,
                                            TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestRemovePresentityGroupMemberCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoUpdatePresentityGroupMemberDisplayNameL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUpdatePresentityGroupMemberDisplayNameL(
                                            const MXIMPIdentity& aGroupId,
                                            const MXIMPIdentity& aMemberId,
                                            const TDesC16& aMemberDisplayName,
                                            TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    // prepare the message for test code
    SetBooleanToCurrent( EXIMPPlgTestUpdatePresentityGroupMemberDisplayNameCalled );
    // TODO when implemented, read the data given by test case and
    //      generate the required events
    iWaitter->WaitForL( KWaitTime );
    }


// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoSubscribePresenceGrantRequestListL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoSubscribePresenceGrantRequestListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestSubscribePresenceGrantRequestListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }

// ---------------------------------------------------------------------------
// From MPscPlugin class.
// T_XIMPTestConnection::DoUnsubscribePresenceGrantRequestListL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::DoUnsubscribePresenceGrantRequestListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );
    }


void T_XIMPTestConnection::DoGrantPresenceForPresentityL( const MXIMPIdentity& aIdentity,
                                           const MPresenceInfoFilter& aPif,
                                           TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestGrantPresenceForPresentityCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }
void T_XIMPTestConnection::DoGrantPresenceForPresentityGroupMembersL( const MXIMPIdentity& aGroupId,
                                                        const MPresenceInfoFilter& aPif,
                                                        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoGrantPresenceForEveryoneL( const MPresenceInfoFilter& aPif,
                                                        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestGrantPresenceForEveryoneCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }


void T_XIMPTestConnection::DoUpdatePresenceGrantPifForPresentityL( const MXIMPIdentity& aIdentity,
                                           const MPresenceInfoFilter& aPif,
                                           TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUpdateGrantPresenceForPresentityCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoUpdatePresenceGrantPifForPresentityGroupMembersL( const MXIMPIdentity& aGroupId,
                                                        const MPresenceInfoFilter& aPif,
                                                        TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUpdateGrantPresenceForPresentityGroupMembersCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoUpdatePresenceGrantPifForEveryoneL( const MPresenceInfoFilter& aPif,
                                                                 TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUpdateGrantPresenceForEveryoneCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }


void T_XIMPTestConnection::DoWithdrawPresenceGrantFromPresentityL( const MXIMPIdentity& aIdentity, TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestWithdrawPresenceFromPresentityCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoWithdrawPresenceGrantFromEveryoneL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestWithdrawPresenceFromEveryoneCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoWithdrawPresenceGrantFromPresentityGroupMembersL( const MXIMPIdentity& aGroupId,
                                                            TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoSubscribePresenceBlockListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestSubscribePresenceBlockListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }

void T_XIMPTestConnection::DoUnsubscribePresenceBlockListL( TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestUnsubscribePresenceBlockListCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }
void T_XIMPTestConnection::DoBlockPresenceForPresentityL( const MXIMPIdentity& aPresentityId,
                                    TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestBlockPresenceForPresentityCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }
void T_XIMPTestConnection::DoCancelPresenceBlockFromPresentityL( const MXIMPIdentity& aPresentityId,
                                           TXIMPRequestId aReqId )
    {
    PluginAPIMethodCommonL( aReqId );

    SetBooleanToCurrent( EXIMPPlgTestCancelPresenceBlockFromPresentityCalled );

    // TODO do something, like in PublishPresence

    iWaitter->WaitForL( KWaitTime );

    }



// ---------------------------------------------------------------------------
// T_XIMPTestConnection::PluginAPIMethodCommonL
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::PluginAPIMethodCommonL( TXIMPRequestId& aOpId )
    {
    iReqId = aOpId;

    TInt index = CXIMPTestMessenger::GetPluginIndex();
    TInt count = iClients.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iClients[ a ]->iPluginIndex == index )
            {
            iCurrentMessenger = iClients[ a ]->iMessenger;
            }
        }
    iCurrentMessenger->HandleLeaveL();
    // handle other errors here
    if ( iCurrentMessenger->GetBoolean( EXIMPTestPlgBehaveInvalidReqId ) )
        {
        // arbitrarily mangle the given request id
        TXIMPRequestIdBuilder idBuilder;
        idBuilder.SetProtocolId( 123 );
        idBuilder.SetSessionId( 456 );
        idBuilder.SetRequestId( 789 );
        //iReqId = TXIMPRequestId( 123, 456, 789 );
        iReqId = idBuilder.BuildRequestId();
        }
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::WaitCompleted()
// ---------------------------------------------------------------------------
//
void T_XIMPTestConnection::WaitCompleted( TInt /* aRetVal */ )
    {
    TInt errorCode = iCurrentMessenger->GetError();

/*
    if ( iSessionLostFailReconnects )
        {
        // fail all reconnects
        errorCode = KErrTimedOut;
        iConnectedClients = 0;
        }
*/
    MXIMPStatus* status = iConnectionHost->ObjectFactory().NewStatusLC();
    CleanupStack::Pop(); // status.
    status->SetResultCode( errorCode );

    MXIMPRestrictedObjectCollection* reqCompleteParams = iCurrentMessenger->GetReqCompleteParams();

    if( reqCompleteParams )
        {
        iConnectionHost->HandleRequestCompleted( iReqId, status, reqCompleteParams );
        reqCompleteParams = NULL;
        }
    else
        {
        iConnectionHost->HandleRequestCompleted( iReqId, status );
        }

    if ( iSessionLostFailReconnects )
        {
        // doing a sequence of failing reconnects
        return;
        }

    // check if test code wants us to fake session lost
    TInt val = iCurrentMessenger->GetValueFor( EXIMPTestPlgSessionLostStrategy );

    if ( val != EXIMPPrtValSessionLostEmpty )
        {
        // yep, fake session lost

        // reset the old value! otherwise timer will restart upon next operation.
        iCurrentMessenger->SetValueFor( EXIMPTestPlgSessionLostStrategy, EXIMPPrtValSessionLostEmpty );

        iSessionLostReconnect = val == EXIMPPrtValSessionLostReconnect;
        iSessionLostFailReconnects = val == EXIMPPrtValSessionLostReconnectExhaustAttempts;

        // start another wait (5 sec) with callback.
        TInt time = 1000000 * 5;
        TCallBack callback( ReconnectSimulator, this );
        iAuxTimer->Start( time, time, callback );
        }

    delete iClientToBeDeleted;
    iClientToBeDeleted = NULL;
    
    }

// ---------------------------------------------------------------------------
// RECONNECTION SIMULATOR ROUTINES
// ---------------------------------------------------------------------------
//
TInt T_XIMPTestConnection::ReconnectSimulator( TAny* aMyself )
    {
//    (static_cast<T_XIMPTestConnection*>(aMyself))->DoReconnectSimulator();
    return 0; // ignored by CPeriodic
    }

void T_XIMPTestConnection::DoReconnectSimulator()
    {
/*    TRAPD( err, DoReconnectSimulatorL() );
    if ( err != KErrNone )
        {
        // TODO what to do? is this enough to fail testcode
        User::Panic( _L("XIMPTestPrt"), 424242 );
        }
        */
    }

void T_XIMPTestConnection::DoReconnectSimulatorL()
    {
    iAuxTimer->Cancel();    // must stop it from running again
/*
    // connection dies.
    iConnectedClients = 0;

    if ( iSessionLostReconnect || iSessionLostFailReconnects )
        {
        // tell host to try to reconnect
        iConnectionHost->HandleConnectionLost( iStatus );
        }
    else
        {
        // tell host to kill connection
        iConnectionHost->HandleConnectionTerminated( iStatus );
        }
*/
    }

void T_XIMPTestConnection::SetBooleanToCurrent( TXIMPTestPropertyKeys aKey )
    {
    if( iCurrentMessenger )
        {
        iCurrentMessenger->SetBoolean( aKey );
        }
    }

void T_XIMPTestConnection::SetBoolean( TXIMPTestPropertyKeys aKey )
    {
    TInt count = iClients.Count();
    for( TInt a = 0; a < count ; ++a )
        {
        iClients[ a ]->iMessenger->SetBoolean( aKey );
        }
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* T_XIMPTestConnection::GetInterface(
        TInt32 aInterfaceId,
        TIfGetOps /* aOptions */ )
    {
    if ( aInterfaceId == MXIMPProtocolConnection::KInterfaceId )
        {
        // caller wants this interface
        MXIMPProtocolConnection* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresencePublishing::KInterfaceId )
        {
        // caller wants this interface
        MProtocolPresencePublishing* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresenceWatching::KInterfaceId )
        {
        // caller wants this interface
        MProtocolPresenceWatching* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresenceAuthorization::KInterfaceId )
        {
        // caller wants this interface
        MProtocolPresenceAuthorization* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresentityGroups::KInterfaceId )
        {
        // caller wants this interface
        MProtocolPresentityGroups* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == T_XIMPTestConnection::KClassId )
        {
        return this;
        }
    else
        {
        return NULL;
        }
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::GetInterface()
// ---------------------------------------------------------------------------
//
const TAny* T_XIMPTestConnection::GetInterface(
        TInt32 aInterfaceId,
        TIfGetOps /* aOptions */ ) const
    {
    if ( aInterfaceId == MXIMPProtocolConnection::KInterfaceId )
        {
        // caller wants this interface
        const MXIMPProtocolConnection* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresencePublishing::KInterfaceId )
        {
        // caller wants this interface
        const MProtocolPresencePublishing* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresenceWatching::KInterfaceId )
        {
        // caller wants this interface
        const MProtocolPresenceWatching* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresenceAuthorization::KInterfaceId )
        {
        // caller wants this interface
        const MProtocolPresenceAuthorization* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == MProtocolPresentityGroups::KInterfaceId )
        {
        // caller wants this interface
        const MProtocolPresentityGroups* myIf = this;
        return myIf;
        }
    else if( aInterfaceId == T_XIMPTestConnection::KClassId )
        {
        return this;
        }
    else
        {
        return NULL;
        }
    }

// ---------------------------------------------------------------------------
// T_XIMPTestConnection::GetInterfaceId()
// ---------------------------------------------------------------------------
//
TInt32 T_XIMPTestConnection::GetInterfaceId() const
    {
    return MXIMPProtocolConnection::KInterfaceId;
    }


// ---------------------------------------------------------------------------
// Key value pair table to identify correct constructor
// function for the requested interface.
// ---------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID,
                                T_XIMPTestProtocol::NewL )
    };


// ---------------------------------------------------------------------------
// Exported function to return the implementation proxy table
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::T_XIMPTestProtocol()
// ---------------------------------------------------------------------------
//
T_XIMPTestProtocol::T_XIMPTestProtocol()
    {
    }


// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::ConstructL()
// ---------------------------------------------------------------------------
//
void T_XIMPTestProtocol::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::NewLC()
// ---------------------------------------------------------------------------
//
T_XIMPTestProtocol* T_XIMPTestProtocol::NewLC()
    {
    T_XIMPTestProtocol* self = new( ELeave ) T_XIMPTestProtocol();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::NewL()
// ---------------------------------------------------------------------------
//
T_XIMPTestProtocol* T_XIMPTestProtocol::NewL()
    {
    T_XIMPTestProtocol* self = T_XIMPTestProtocol::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::~T_XIMPTestProtocol()
// ---------------------------------------------------------------------------
//
T_XIMPTestProtocol::~T_XIMPTestProtocol()
    {
    iConnections.Close();
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::PrimeHost()
// ---------------------------------------------------------------------------
//
void T_XIMPTestProtocol::PrimeHost( MXIMPProtocolPluginHost& aHost )
    {
    iHost = &aHost;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::AcquireConnectionL()
// ---------------------------------------------------------------------------
//
MXIMPProtocolConnection& T_XIMPTestProtocol::AcquireConnectionL(
                                    const MXIMPServiceInfo& aServiceInfo,
                                    const MXIMPContextClientInfo& aClientCtxInfo )
    {
    T_XIMPTestConnection* newConnection = T_XIMPTestConnection::NewLC( aServiceInfo, aClientCtxInfo );
    TLinearOrder< T_XIMPTestConnection > order( T_XIMPTestConnection::OrderOfConnections );
    TInt index = iConnections.FindInOrder( newConnection, order );
    if( index != KErrNotFound )
        {
        User::LeaveIfError( index );
        CleanupStack::PopAndDestroy(); // newConnection
        newConnection = iConnections[ index ];
        }
    else
        {
        iConnections.InsertInOrderL( newConnection, order );
        CleanupStack::Pop(); //newConnection
        }
    return *newConnection;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::ReleaseConnection()
// ---------------------------------------------------------------------------
//
void T_XIMPTestProtocol::ReleaseConnection( MXIMPProtocolConnection& aConnection )
    {
    TLinearOrder< T_XIMPTestConnection > order( T_XIMPTestConnection::OrderOfConnections );
    T_XIMPTestConnection* connection = ( T_XIMPTestConnection* )( aConnection.GetInterface(
                    T_XIMPTestConnection::KClassId,
                    MXIMPBase::EPanicIfUnknown ) );
    TInt index = iConnections.FindInOrder( connection, order );
    if( index >= 0 )
        {
        iConnections.Remove( index );
        }
    delete connection;
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* T_XIMPTestProtocol::GetInterface(
        TInt32 aInterfaceId,
        TIfGetOps /* aOptions */ )
    {
    if ( aInterfaceId == MXIMPProtocolPlugin::KInterfaceId )
        {
        // caller wants this interface
        MXIMPProtocolPlugin* myIf = this;
        return myIf;
        }
    else
        {
        return NULL;
        }
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::GetInterface()
// ---------------------------------------------------------------------------
//
const TAny* T_XIMPTestProtocol::GetInterface(
        TInt32 aInterfaceId,
        TIfGetOps /* aOptions */ ) const
    {
    if ( aInterfaceId == MXIMPProtocolPlugin::KInterfaceId )
        {
        // caller wants this interface
        const MXIMPProtocolPlugin* myIf = this;
        return myIf;
        }
    else
        {
        return NULL;
        }
    }

// ---------------------------------------------------------------------------
// T_XIMPTestProtocol::GetInterfaceId()
// ---------------------------------------------------------------------------
//
TInt32 T_XIMPTestProtocol::GetInterfaceId() const
    {
    return MXIMPProtocolPlugin::KInterfaceId;
    }



// End of file

