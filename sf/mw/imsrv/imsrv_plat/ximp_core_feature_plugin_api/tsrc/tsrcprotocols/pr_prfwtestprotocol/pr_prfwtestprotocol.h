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


#ifndef T_PRFWTESTPROTOCOL_H
#define T_PRFWTESTPROTOCOL_H


#include <e32base.h>

#include <ximpprotocolpluginbase.h>
#include <protocolpresencepublishing.h>
#include <protocolpresencewatching.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>
#include <ximpprotocolconnection.h>
#include <ximpprotocolconnectionhostobserver.h>
#include <ximpbase.h>

#include "prfwwaitobserver.h"

#include "prfwtestmessaging.h"

class CXIMPTestGeneralWaiter;
class CXIMPTestMessenger;
class CPeriodic;
class CXIMPTestFileLogicHelper;
class CXIMPTestFileTool;
class MXIMPRestrictedObjectCollection;
class MProtocolImFeatures;
#define PRIMP_CLSID_CPRESENTITYGROUPINFOIMP 15000

class T_XIMPTestConnection :public CBase,
                            public MXIMPProtocolConnection,
                            public MProtocolPresenceFeatures,
                            public MProtocolPresencePublishing,
                            public MProtocolPresenceWatching,
                            public MProtocolPresenceAuthorization,
                            public MProtocolPresentityGroups,
                            public MXIMPProtocolConnectionHostObserver,
                            public T_MWaitObserver
                            
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYGROUPINFOIMP };


        class ClientWrapper : public CBase
            {
            private:

                void ConstructL( const TDesC& aClientId );

            public:

                static ClientWrapper* NewLC( const TDesC& aClientId );
                void InitializeL( MXIMPProtocolConnectionHost& aConnectionHost );
                ~ClientWrapper();

            public:

                RBuf iId;
                CXIMPTestFileTool* iFileTool;
                CXIMPTestMessenger* iMessenger;
                CXIMPTestFileLogicHelper* iFileLogicHelper;
                TInt iPluginIndex;
            };

    public:

        static T_XIMPTestConnection* NewL( const MXIMPServiceInfo& aServiceInfo,
                                           const MXIMPContextClientInfo& aClientCtxInfo );
        static T_XIMPTestConnection* NewLC( const MXIMPServiceInfo& aServiceInfo,
                                            const MXIMPContextClientInfo& aClientCtxInfo );

        ~T_XIMPTestConnection();


    private:

        T_XIMPTestConnection();
        void ConstructL( const MXIMPServiceInfo& aServiceInfo,
                         const MXIMPContextClientInfo& aClientCtxInfo );

    public: // From MXIMPProtocolConnection

        /**
         * host for connection
         */
        void PrimeHost( MXIMPProtocolConnectionHost& aHost );

        void OpenSessionL( const MXIMPContextClientInfo& aClientCtxInfo,
                               TXIMPRequestId aReqId  );

        
        void OpenSessionL(
                const TInt& aSettingsId,
                TXIMPRequestId aReqId );

        void CloseSession( const MXIMPContextClientInfo& aClientCtxInfo,
                               TXIMPRequestId aReqId );
                               
        MProtocolPresenceFeatures& ProtocolPresenceFeatures();
		
		//MProtocolImFeatures* ProtocolImFeatures();
		
		TAny* GetProtocolInterface(TInt aInterfaceId);
		
    public: // MProtocolPresenceFeatures
        MProtocolPresenceWatching& PresenceWatching();
        MProtocolPresencePublishing& PresencePublishing();
        MProtocolPresentityGroups& PresentityGroups();
        MProtocolPresenceAuthorization& PresenceAuthorization();
        
        
    public: // From MXIMPProtocolConnectionHostObserver        
    
        void HandleProtocolConnectionHostEvent(
                const MXIMPProtocolConnectionHost& aHost,
                const MXIMPBase& aEvent );


    public: // from MPscPlugin


        void GetSupportedFeaturesL(
                CDesC8Array& aFeatures ) const;

        void DoPublishOwnPresenceL(
                const MPresenceInfo& aPresence,
                TXIMPRequestId aReqId );

        void DoSubscribeOwnPresenceL( const MPresenceInfoFilter& aPif,
                                          TXIMPRequestId aReqId );

        void DoUnsubscribeOwnPresenceL( TXIMPRequestId aReqId );

        void DoUpdateOwnPresenceSubscriptionPifL( const MPresenceInfoFilter& aPif,
                                          TXIMPRequestId aReqId );


        void DoSubscribePresenceWatcherListL( TXIMPRequestId aReqId );
        void DoUnsubscribePresenceWatcherListL( TXIMPRequestId aReqId );

        void DoSubscribePresentityGroupListL( TXIMPRequestId aReqId );
        void DoUnsubscribePresentityGroupListL( TXIMPRequestId aReqId );

        void DoCreatePresentityGroupL( const MXIMPIdentity& aIdentity,
                                           const TDesC16& aDisplayName,
                                           TXIMPRequestId aReqId );
        void DoDeletePresentityGroupL( const MXIMPIdentity& aIdentity,
                                               TXIMPRequestId aReqId );
        void DoUpdatePresentityGroupDisplayNameL( const MXIMPIdentity& aIdentity,
                                               const TDesC16& aDisplayName,
                                               TXIMPRequestId aReqId );
        void DoSubscribePresentityPresenceL(
                                    const MXIMPIdentity& aIdentity,
                                    const MPresenceInfoFilter& aPif,
                                    TXIMPRequestId aReqId );
        void DoUpdatePresentityPresenceSubscriptionPifL(
                                    const MXIMPIdentity& aIdentity,
                                    const MPresenceInfoFilter& aPif,
                                    TXIMPRequestId aReqId );
        void DoUnsubscribePresentityPresenceL( const MXIMPIdentity& aIdentity,
                                    TXIMPRequestId aReqId );

        void DoSubscribePresentityGroupContentL( const MXIMPIdentity& aGroupId,
                                                     TXIMPRequestId aReqId );

        void DoUnsubscribePresentityGroupContentL( const MXIMPIdentity& aGroupId,
                                                       TXIMPRequestId aReqId );

        void DoAddPresentityGroupMemberL( const MXIMPIdentity& aGroupId,
                                              const MXIMPIdentity& aMemberId,
                                              const TDesC16& aMemberDisplayName,
                                              TXIMPRequestId aReqId );

        void DoRemovePresentityGroupMemberL( const MXIMPIdentity& aGroupId,
                                                 const MXIMPIdentity& aMemberId,
                                                 TXIMPRequestId aReqId );

        void DoUpdatePresentityGroupMemberDisplayNameL( const MXIMPIdentity& aGroupId,
                                                            const MXIMPIdentity& aMemberId,
                                                            const TDesC16& aMemberDisplayName,
                                                            TXIMPRequestId aReqId );
        void DoSubscribePresentityGroupMembersPresenceL(
                                        const MXIMPIdentity& aIdentity,
                                        const MPresenceInfoFilter& aPif,
                                        TXIMPRequestId aReqId );

        void DoUpdatePresentityGroupMembersPresenceSubscriptionPifL(
                                        const MXIMPIdentity& aIdentity,
                                        const MPresenceInfoFilter& aPif,
                                        TXIMPRequestId aReqId );

        void DoUnsubscribePresentityGroupMembersPresenceL( const MXIMPIdentity& aIdentity,
                                        TXIMPRequestId aReqId );

        void DoSubscribePresenceGrantRequestListL( TXIMPRequestId aReqId );

        void DoUnsubscribePresenceGrantRequestListL( TXIMPRequestId aReqId );

        void DoGrantPresenceForPresentityL( const MXIMPIdentity& aIdentity,
                                                   const MPresenceInfoFilter& aPif,
                                                   TXIMPRequestId aReqId );
        void DoGrantPresenceForPresentityGroupMembersL( const MXIMPIdentity& aGroupId,
                                                                const MPresenceInfoFilter& aPif,
                                                                TXIMPRequestId aReqId );

        void DoGrantPresenceForEveryoneL( const MPresenceInfoFilter& aPif,
                                            TXIMPRequestId aReqId );

        void DoUpdatePresenceGrantPifForPresentityL( const MXIMPIdentity& aIdentity,
                                                   const MPresenceInfoFilter& aPif,
                                                   TXIMPRequestId aReqId );

        void DoUpdatePresenceGrantPifForPresentityGroupMembersL( const MXIMPIdentity& aGroupId,
                                                                const MPresenceInfoFilter& aPif,
                                                                TXIMPRequestId aReqId );

        void DoUpdatePresenceGrantPifForEveryoneL( const MPresenceInfoFilter& aPif,
                                                   TXIMPRequestId aReqId );

        void DoWithdrawPresenceGrantFromPresentityL( const MXIMPIdentity& aIdentity, TXIMPRequestId aReqId );

        void DoWithdrawPresenceGrantFromPresentityGroupMembersL( const MXIMPIdentity& aGroupId,
                                                                    TXIMPRequestId aReqId );

        void DoWithdrawPresenceGrantFromEveryoneL( TXIMPRequestId aReqId );

        void DoSubscribePresenceBlockListL( TXIMPRequestId aReqId );
        void DoUnsubscribePresenceBlockListL( TXIMPRequestId aReqId );
        void DoBlockPresenceForPresentityL( const MXIMPIdentity& aPresentityId,
                                            TXIMPRequestId aReqId );
        void DoCancelPresenceBlockFromPresentityL( const MXIMPIdentity& aPresentityId,
                                                   TXIMPRequestId aReqId );

    public: // from T_MWaitObserver

        void WaitCompleted( TInt aRetVal );


    public: // from MXIMPBase

        TAny* GetInterface(
                TInt32 aInterfaceId,
                TIfGetOps aOptions );

        const TAny* GetInterface(
                TInt32 aInterfaceId,
                TIfGetOps aOptions ) const;

        TInt32 GetInterfaceId() const;

    private:
        /**
         * All plugin API methods should call this method
         * to handle test code to plugin communication
         * properly.
         * Add more common stuff here (such as new special messages
         * which behave in a common way to all plugin API calls, etc.)
         * @param aOpId The request id
         */
        void PluginAPIMethodCommonL( TXIMPRequestId& aOpId );

        /**
         * Callback for aux timer
         * @see TCallBack
         */
        static TInt ReconnectSimulator( TAny* aMyself );

        /**
         * Actual callback called from ReconnectSimulator
         */
        void DoReconnectSimulator();

        /**
         * Leaving version of aux timer callback.
         */
        void DoReconnectSimulatorL();

        void SetBooleanToCurrent( TXIMPTestPropertyKeys aKey );
        void SetBoolean( TXIMPTestPropertyKeys aKey );

    public:
        static TInt OrderOfConnections( const T_XIMPTestConnection& aFirst,
                                        const T_XIMPTestConnection& aSecond );

    private:

        static TInt OrderOfClients( const ClientWrapper& aFirst,
                                    const ClientWrapper& aSecond );
    private:

        HBufC* iServerAddress;
        HBufC* iUserName;
        HBufC* iPassword;
        HBufC* iClientId;

        //TInt iConnectedClients;

        RPointerArray< ClientWrapper > iClients;
        ClientWrapper* iClientToBeDeleted;

        TXIMPRequestId iReqId;
        CXIMPTestGeneralWaiter* iWaitter;

        MXIMPStatus* iStatus;

        // last received message
        TXIMPTestMsg iLastMsg;

        // ETrue if reconnect is to be faked for session lost.
        TBool iSessionLostReconnect;

        // ETrue if all reconnects are to be failed.
        TBool iSessionLostFailReconnects;

        // auxiliary timer (session lost simulation)
        CPeriodic* iAuxTimer;


        /**
         * File tool for monitoring events.
         */
        //CXIMPTestFileTool* iFileTool;

        /**
         * Host for connection
         */
        MXIMPProtocolConnectionHost* iConnectionHost;

        CXIMPTestMessenger* iCurrentMessenger;
    };

/**
 * Example protocol plugin
 *
 *
 * @lib ?library
 * @since S60 v4.0
 */
class T_XIMPTestProtocol : public CXIMPProtocolPluginBase
    {
    public:

        static T_XIMPTestProtocol* NewL();
        static T_XIMPTestProtocol* NewLC();

        ~T_XIMPTestProtocol();

    private:

        T_XIMPTestProtocol();
        void ConstructL();

    public: // From MXIMPProtocolPlugin

        void PrimeHost( MXIMPProtocolPluginHost& aHost );

        MXIMPProtocolConnection& AcquireConnectionL(
                    const MXIMPServiceInfo& aServiceInfo,
                    const MXIMPContextClientInfo& aClientCtxInfo );
        void ReleaseConnection(
                        MXIMPProtocolConnection& aConnection );

    public: // from MXIMPBase

        TAny* GetInterface(
                TInt32 aInterfaceId,
                TIfGetOps aOptions );

        const TAny* GetInterface(
                TInt32 aInterfaceId,
                TIfGetOps aOptions ) const;

        TInt32 GetInterfaceId() const;


    private: // Data

        // Prime host
        MXIMPProtocolPluginHost* iHost;
        RXIMPObjOwningPtrArray< T_XIMPTestConnection > iConnections;
    };

#endif // T_PRFWTESTPROTOCOL_H
