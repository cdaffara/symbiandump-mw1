/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements Target Selector Plugin (TSP) used by Symbian's 
*				 Remote Control Framework. TSP controls sending of operations
*			     between Series 60 components and remote targets.
*
*/


#ifndef CREMCONTSPCONTROLLER_H
#define CREMCONTSPCONTROLLER_H

//  INCLUDES
#include <e32base.h>
#include <remcon/remcontargetselectorplugin.h>
#include <remcon/remcontargetselectorplugininterface.h>
#include <e32property.h>
#include <cfclient.h>
#include <cflistener.h>
#include <AudioClientsListPSData.h>
#include <remcon/clientinfo.h>
#include "remconidlelistener.h"
#include "remconeventtable.h"
#include "tsptriggerevents.h"
// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRemConLogger;
class TClientInfo;


// CLASS DECLARATION

/**
*  Controls delivering of commands between Series 60 components and remote 
*  targets.
*
*  @lib RemConTspController.dll
*  @since Series 60 3.0
*/
class CRemConTspController : public CRemConTargetSelectorPlugin,
                 public MRemConTargetSelectorPluginInterfaceV2,
                 public MRemConTargetSelectorPluginInterfaceV3,
                 public MRemConTargetSelectorPluginInterfaceV4,
                 public MRemConTargetSelectorPluginInterfaceV5,
			     public MCFListener,
                 public MTspRulesTriggerObserver
	{
	public:		// Constructors and destructor
		
		/**
        * Two-phased constructor.
        */
		static CRemConTspController* NewL( 
			MRemConTargetSelectorPluginObserver& aObserver );
		
		/**
        * Destructor.
        */
		virtual ~CRemConTspController();
	
	public: 	// Functions from base classes
	
	    void CreateAfterIdleL();
	
	protected:  // New functions
	
	protected:  // Functions from base classes
	
	private:
		
		/**
        * C++ default constructor.
        */
        CRemConTspController( MRemConTargetSelectorPluginObserver& aObserver );
                
	private: // from CRemConTargetSelectorPlugin
		
		/**
        * Called by RemCon server to get a pointer to an object which 
        * implements the TSP interface with UID aUid. This is a mechanism 
        * for allowing future change to the TSP API without breaking BC 
        * in existing (non-updated) TSPs.
        * 
        * @param aUid	Queried TSP interface version.
        * @return TAny	A pointer to implementation of this TSP version, if 
        *				quaried TSP interface version is the one implemented 
        *				by this TSP. Otherwise a NULL will be returned.	
        */
		TAny* GetInterface( TUid aUid );

	private: // from MRemConTargetSelectorPluginInterface
	
		/**
		* Called by RemCon server to get the TSP to address an outgoing 
		* command (from a connectionless controller client) to zero or 
		* more remote targets.
		* @param aInterfaceUid 		The UID of the client interface.
		* @param aOperationId 		The operation ID of the command.
		* @param aSender The 		TClientInfo of the sending session.
		* @param aConnections 		An empty collection of connections. 
		*							TRemConAddresses must be made on the heap.
		*							RemCon takes ownership of any items in the 
		*							collection. 
		* @params aBearerSecurity 	Contains all the bearer security policies.
		* @return TAny				void 
		*/
		void AddressOutgoingCommand(
							TUid aInterfaceUid,
							TUint aOperationId, 
							const TClientInfo& aSender,
							TSglQue<TRemConAddress>& aConnections,
							TSglQue<TBearerSecurity>& aBearerSecurity );
		
		/**
		* Called by RemCon server to find out from the TSP whether the given 
		* connection-oriented controller client is permitted to send the given 
		* command to remote target.
		* @param aInterfaceUid 		The UID of the client interface.
		* @param aOperationId 		The operation ID of the command.
		* @param aSender 			The TClientInfo of the sending session.
		* @param aConnection 		The remote the command will be sent over if
		*							permission is granted.
		* @return void				 
		*/	
		void PermitOutgoingCommand(
							TUid aInterfaceUid,
							TUint aOperationId, 
							const TClientInfo& aSender,
							const TRemConAddress& aConnection );
		
		/** 
		* Called by RemCon server to cancel the current AddressOutgoingCommand 
		* or PermitOutgoingCommand command.
		* @return void 
		*/					
		void CancelOutgoingCommand();
	
		/**
		* Called by RemCon server to get the TSP to address an incoming command 
		* (from a remote target to zero or more Series 60 components). There's 
		* no 'cancel' method for AddressIncomingCommand.
		* @since Series 60 3.0
		* @param aInterfaceUid 	The UID of the client interface.
		* @param aOperationId 	The operation ID of the command.
		* @param aClients 		A collection of existing target clients. TSP's 
		*						responsibility is to modify this parameter, if 
		*						it wants that command will not be sent to all 
		*						proposed clients. Thus, when this method has 
		*						been finished, aClients will include those 
		*						target clients to whom command should be sent.
		* @return void  
		*/
		void AddressIncomingCommand(
							TUid aInterfaceUid,
							TUint aOperationId, 
							TSglQue<TClientInfo>& aClients );
							
        /**
        * Called by RemCon to get the TSP to decide which client should be
        * allowed to respond to a command.
        * @param aInterfaceUid The UID of the client interface.
        * @param aOperationId The operation ID of the command.
        * @param aClient The client which is trying to send a response
        * @param aClients A list of clients which are still expected to respond,
        *         including the one specified in aClient
        */
        void PermitOutgoingResponse(
                TUid aInterfaceUid,
                TUint aOperationId,
                const TClientInfo& aClient,
                TClientInfoConstIter& aClients); 

        /** 
        * Called by RemCon to cancel the current PermitOutgoingResponse request.
        */
        void CancelOutgoingResponse(); 
            
        /**
        * Called by RemCon to get the TSP to address an incoming notify (from a 
        * remote to zero or more target clients). 
        * @param aInterfaceUid The UID of the client interface.
        * @param aOperationId The operation ID of the command.
        * @param aClients A collection of existing target clients. The implementor 
        *         may wish to start a target client and call IncomingNotifyAddressed
        *         with a new TClientInfo. The new TClientInfo item must be made on the stack. 
        *         Note when creating the new TClientInfo, only the process ID needs to 
        *         be correctly populated.
        */
        void AddressIncomingNotify(
                TUid aInterfaceUid,
                TUint aOperationId,
                TSglQue<TClientInfo>& aClients); 

        // From MRemConTargetSelectorPluginInterfaceV3

        /**
        Called by RemCon to get the TSP to address an outgoing notify command (from a 
        connectionless controller client) to zero or one remote. 
        @param aInterfaceUid The UID of the client interface.
        @param aOperationId The operation ID of the command.
        @param aSender The TClientInfo of the sending session.
        @param aBearerSecurity Contains all the bearer security policies.
        */
        void AddressOutgoingNotify(
                TUid aInterfaceUid,
                TUint aOperationId, 
                const TClientInfo& aSender,
                TSglQue<TBearerSecurity>& aBearerSecurity);
        
        /** 
        Called by RemCon to cancel the current AddressOutgoingNotify or 
        PermitOutgoingNotifyCommand command.
        */
        void CancelOutgoingNotifyCommand();
        
        /**
        Called by RemCon to find out from the TSP whether the given 
        connection-oriented controller client is permitted to send the given notify
        command to the given remote at this time. 
        @param aInterfaceUid The UID of the client interface.
        @param aOperationId The operation ID of the command.
        @param aSender The TClientInfo of the sending session.
        @param aConnection The remote the command will be sent over if permission 
        is granted.
        */
        void PermitOutgoingNotifyCommand(
                TUid aInterfaceUid,
                TUint aOperationId, 
                const TClientInfo& aSender,
                const TRemConAddress& aConnection);
        
        /**
        Called by RemCon to get the TSP to permit an incoming command. This is called
        if the bearer has provided a target client for the command.  
        
        The implementor should decide if they wish to allow this command and then call
        IncomingCommandPermitted on the observer with a suitable error. 
        
        @param aInterfaceUid The UID of the client interface.
        @param aOperationId The operation ID of the command.
        @param aClient a TClientInfo referring to the selected client
        */
        virtual void PermitIncomingCommand(
            TUid aInterfaceUid,
            TUint aOperationId, 
            const TClientInfo& aClient);
        
        /**
        Called by RemCon to get the TSP to permit an incoming Notify. This is called
        if the bearer has provided a target client for the Notify.  
        
        The implementor should decide if they wish to allow this Notify and then call
        IncomingNotifyPermitted on the observer with a suitable error. 
        
        @param aInterfaceUid The UID of the client interface.
        @param aOperationId The operation ID of the Notify.
        @param aClient a TClientInfo referring to the selected client
        */
        virtual void PermitIncomingNotify(
            TUid aInterfaceUid,
            TUint aOperationId, 
            const TClientInfo& aClient);

        /** 
        Called by RemCon when a bearer that can address commands wishes to
        inform the system that there has been a remote user action to 
        select a different addressed client.
        
        The bearer will then route addressed commands to this client until
        such time as SetRemoteAddressedClient is called again or the TSP
        calls SetLocalAddressedClient.
        
        @param aBearerUid The bearer that has changed its addressed client
        @param aClient The RemCon client that is now selected by the bearer
        */
        virtual void SetRemoteAddressedClient(const TUid& aBearerUid, 
                const TClientInfo& aClient);
        /** Called by RemCon when a new target client has connected.
         
         @aClientInfo The information about the new client.
         */
        void TargetClientAvailable(const TClientInfo& aClientInfo);
        
        /** Called by RemCon when a target client has disconnected. 
         
         @aClientInfo The information about the client that has disconnected.
         */
        void TargetClientUnavailable(const TClientInfo& aClientInfo);
        
        /** Called by RemCon when a bearer wishes to begin being informed when
        the locally addressed player changes.  Once this function has been called
        the TSP should inform RemCon via SetLocalAddressedPlayer each time the
        player to which incoming commands from aBearer would be routed changes.
        This might occur for example if a new application is launched, or if the
        foreground application changes, depending on what the TSP's rules are
        for deciding the target of the incoming message.  These updates should
        occur until UnregisterLocalAddressedClientObserver is called.
        
        @param aBearerUid The bearer that wishes to be informed of updates
        */
        TInt RegisterLocalAddressedClientObserver(const TUid& aBearerUid);

        /** Called by RemCon when a bearer wishes to stop being informed of 
        changes to the local addresse client.
        
        @param aBearerUid The bearer that no longer wishes to be informed of updates
        */
        TInt UnregisterLocalAddressedClientObserver(const TUid& aBearerUid);

        
	    // From MCFListener
	    
	    /**
        * Indicates a change in context for subscribed clients.
        * If client has subscribed to partial context class path,
        * only the changed context will be indicated.
        * 
        * @param aIndication: Context indication.
        * @return None
        */
        void ContextIndicationL(
            const CCFContextIndication& aChangedContext );
            
        /**
        * Indicates that specified action is needed to be executed.
        * When action indication is received certain rule has been triggered
        * and action is needed to be performed.
        *
        * @param aActionToExecute Action indication.
        * @return None
        */
        void ActionIndicationL(
            const CCFActionIndication& aActionToExecute );
        
        /**
        * Returns an extension interface.
        * The extension interface is mapped with the extension UID.
        *
        * If the client does not support the requested interface,
        * client must return NULL. Otherwise client needs to return
        * the correct interface combined with the UID.
        *
        * @since S60 5.0
        * @param aExtensionUid: The identifier of the extension.
        * @return Pointer to the extension.
        */    
        TAny* Extension( const TUid& aExtensionUid ) const;
            
        /**
        * Indicates that error has occured.
        * 
        * @param aError Error code.
        * @return None
        */
        void HandleContextFrameworkError( TCFError aError,
            const TDesC& aSource,
            const TDesC& aType );
            
        // from MTspRulesTriggerObserver
        void MtrtoEvaluateRoutingRules();
        /**
        * Gets the foreground application.
        * 
        * @param aError Error code.
        * @return None
        */
        void GetActiveWindowClientL();
        
        /**
        * Checks if process is connected to Remcon
        */
        TClientInfo* FindRemconConnection( TProcessId aProcessId, TSglQue<TClientInfo>& aClients ) const;

        /**
        * Checks if client with given secure id is connected to Remcon
        */
        TClientInfo* FindRemconConnection( TSecureId aSecureId, TSglQue<TClientInfo>& aClients ) const;

        /**
        * Sorts the audio clients to a correct order. The last played is first.
        */
        void GetActivePidsL( RArray<TProcessId>& aArray );
        
#if ( defined COMPONENT_TRACE_FLAG && _DEBUG )
        void TraceAudioPolicyClients( const TUint32 aState, TAudioPolicyProcessIdList aIdList );
#endif
	
	public:     // Data
	
	protected:  // Data
	
	    void ConstructL();
	
    private:
	
	    void GetCorrectClientL(
            TUid aInterfaceUid,
        	TUint aKeyEvent,
        	TSglQue<TClientInfo>& aClients,
        	TBool aLaunchingNewApplicationAllowed);

        void SetKeyEventTableL( const CCFActionIndication& aActionToExecute );        

        void ActivateApplicationL( const TUid aUid ) const;

        TBool IsStateNormal() const;

        TInt FindRoutingTablePos(
                const RPointerArray<CRemConEventTable>& aTableArray, TInt aTableNum ) const;

        CRemConEventTable* FindRoutingTable(
                const RPointerArray<CRemConEventTable>& aTableArray, TInt aTableNum ) const;
        
        TBool FindActiveAudioFromAudioPolicy( TSglQue<TClientInfo>& aClients );
        
        TBool FindActiveAudioFromCustomIfL( TSglQue<TClientInfo>& aClients );
        
        /**
        * Returns whether system lock is active.
        */
        TBool DeviceLocked() const;

        /**
         * Decide if locally addressed client should be updated.
         */
        TClientInfo* GetLocalAddressedClient();

private:
        NONSHARABLE_STRUCT(TClientObserver)
            {
        public:
            TClientObserver(TUid aBearerUid) : iBearerUid(aBearerUid) {};
        public:
            TUid iBearerUid;
            TSglQueLink iClientObserverQueLink;
            };

private:    // Data
        // owned
        CRemConIdleListener* iIdle;

        // Interface to P&S key that returns call state
        RProperty iProperty;
        
        CCFClient* iCFClient;
        
        RPointerArray<CRemConEventTable> iArrayOfTables;
        
        TUid iUidForeground;
        
        TProcessId iProcessIdForeground;
        
        TProcessId iProcessIdActive;

        RPointerArray<CRemConEventTable> iArrayOfStoredTables;
        
        TSglQue<TClientObserver> iClientObservers;
        
        TSglQue<TClientInfo> iAvailableTargets;
        
        TSglQue<TClientInfo> iTargetsForAddressing;
        
        TClientInfo* iLocalAddressedClient;
        
        CTspTriggerEventsWatcher* iTriggerEventsWatcher;
	
	public:     // Friend classes
	
	protected:  // Friend classes
	
	private:    // Friend classes

	};

#endif		// CREMCONTSPCONTROLLER_H
