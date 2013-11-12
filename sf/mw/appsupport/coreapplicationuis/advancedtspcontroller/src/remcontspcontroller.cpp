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
* Description:  Implements Target Selector Plugin used by Symbian's Remote 
*				 Control Framework. TSP controls sending of operations between 
*				 Series 60 components and remote targets (e.g. headset).
*
*				 This version doesn't really self specify receiver for 
*				 a message but accepts all receivers suggested by RemCon server.
*
*/


// INCLUDE FILES
#include "RemConDebug.h"
#include "remcontspcontroller.h"
#include "remconeventtable.h"
#include <remcon/remcontargetselectorpluginobserver.h>
#include <remconaddress.h>
#include <remcon/clientinfo.h>
#include <remcon/bearersecurity.h>
#include <RemConExtensionApi.h>
#include <ctsydomainpskeys.h>
#include <tspclientmapper.h>
#include <cfclient.h>
#include <cflistener.h>
#include <cfkeyvaluepair.h>
#include <cfactionindication.h>
#include <cfactionsubscription.h>
#include <cfcontextobject.h>
#include <w32std.h>
#include <apgwgnam.h>
#include <apgtask.h>
#include <AudioClientsListPSKeys.h>
#include <remconcoreapi.h>
#include <coemain.h>
#include <apgcli.h>
#include <apacmdln.h>
#include <startupdomainpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <absolutevolumeapi.h>
//Build break: Remove this once BT comes up with a solution
const TInt KRemConMediaBrowseApiUidTemp = 0x10285bbb;

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// MACROS

// LOCAL CONSTANTS AND MACROS

const TUint32 KUid3MusicPlayer = 0x102072C3;
const TUint32 KUid3PhoneApp = 0x100058B3; 
const TUint32 KUid3VoiceCmdApp = 0x102818e7;

//#define __MODULE_TEST__

#ifdef __MODULE_TEST__
const TUint32 KUid3BTAudioMan = 0x102073E4; // STIF
#else
const TUint32 KUid3BTAudioMan = 0x10208971;
#endif

_LIT( KRoutingTable, "routingTable" );
_LIT( KRemConKeyEvent, "keyEventRemCon*" );
_LIT( KRemConKeyEventExt, "keyEventRemConExt*" );
_LIT( KRemConKeyEventExtCall, "keyEventRemConExtCall*" );
_LIT( KRemConKeyEventExtPoc, "keyEventRemConExtPoc*" );
_LIT( KRemConKeyEventExtSync, "keyEventRemConExtSync*" );
_LIT( KRuleText, "rule*" );
_LIT( KDefaultAppText, "defaultApplication*" );
_LIT( KTransparentAppText, "transparentApplication*" );
_LIT( KRemoveRoutingTable, "removeRoutingTable" );
_LIT( KDefaultLaunchAppText, "defaultLaunchApp*" );
_LIT( KStoreRoutingTable, "storeRoutingTable" );
_LIT( KRestoreRoutingTable, "restoreRoutingTable" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================
static TUint HexStringToInt( const TDesC& aString )
    {
    TUint ret( 0 );
    TLex lex( aString );
    lex.Val( ret, EHex ); // Ignore error
    return ret;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRemConTspController::CRemConTspController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CRemConTspController::CRemConTspController(MRemConTargetSelectorPluginObserver& aObserver )
    : CRemConTargetSelectorPlugin( aObserver )
    , iClientObservers(_FOFF(TClientObserver, iClientObserverQueLink))
    , iAvailableTargets(_FOFF(TClientInfo, iLink))
    , iTargetsForAddressing(_FOFF(TClientInfo, iLink2))
	{
	}
    
// -----------------------------------------------------------------------------
// CRemConTspController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRemConTspController* CRemConTspController::NewL( 
								MRemConTargetSelectorPluginObserver& aObserver )
    {
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::NewL()" );
	
    CRemConTspController* self = new( ELeave ) CRemConTspController( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CRemConTspController::ConstructL()
    {
    iIdle = CRemConIdleListener::NewL( *this );
	}

// Destructor
CRemConTspController::~CRemConTspController()
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::~CRemConTspController()" );
	if( iIdle )
	    {
	    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::~CRemConTspController() - iIdle exists, destroy" );
	    delete iIdle;
	    iIdle = NULL;
	    }
	if( iCFClient )
	    {
	    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::~CRemConTspController() - iCFClient exists, destroy" );
	    delete iCFClient;
	    iCFClient = NULL;
	    }
    iArrayOfTables.ResetAndDestroy();
    iArrayOfStoredTables.ResetAndDestroy();
    
    TClientInfo* clientInfo;
    while(!iAvailableTargets.IsEmpty())
        {
        clientInfo = iAvailableTargets.First();
        iAvailableTargets.Remove(*clientInfo);
        delete clientInfo;
        }

    TClientObserver* clientObserver;
    while(!iClientObservers.IsEmpty())
        {
        clientObserver = iClientObservers.First();
        iClientObservers.Remove(*clientObserver);
        delete clientObserver;
        }
    iTargetsForAddressing.Reset(); 
    delete iTriggerEventsWatcher;
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::~CRemConTspController() - return" );
	}

// -----------------------------------------------------------------------------
// CRemConTspController::GetInterface
// Provides implementation of this (TSP) interface. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TAny* CRemConTspController::GetInterface( TUid aUid )
	{
	COM_TRACE_2( "[REMCONTSPCONTROLLER] CRemConTspController::GetInterface() this=%d, aUid=0x%x", this, aUid );

	TAny* ret = NULL;
	
	// TSP interface is asked
	if ( aUid == TUid::Uid( KRemConTargetSelectorInterface1 ) )
		{
		// convert this object to TAny
		ret = reinterpret_cast< TAny* >( 
				static_cast< MRemConTargetSelectorPluginInterface* >( this ) );
		}
	else if ( aUid == TUid::Uid(KRemConTargetSelectorInterface2) )
		{
		ret = reinterpret_cast<TAny*>(
			static_cast<MRemConTargetSelectorPluginInterfaceV2*>(this) );
		}
	else if ( aUid == TUid::Uid(KRemConTargetSelectorInterface3) )
		{
		ret = reinterpret_cast<TAny*>(
			static_cast<MRemConTargetSelectorPluginInterfaceV3*>(this) );
		}
    else if ( aUid == TUid::Uid(KRemConTargetSelectorInterface4) )
        {
        ret = reinterpret_cast<TAny*>(
            static_cast<MRemConTargetSelectorPluginInterfaceV4*>(this) );
        }
    else if ( aUid == TUid::Uid(KRemConTargetSelectorInterface5) )
        {
        ret = reinterpret_cast<TAny*>(
            static_cast<MRemConTargetSelectorPluginInterfaceV5*>(this) );
        }
	COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetInterface() this=%d", ret );
	return ret;
	}

// -----------------------------------------------------------------------------
// CRemConTspController::AddressOutgoingCommand
// Defines bearers (represents remote target) to which command will be sent. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::AddressOutgoingCommand(
		TUid aInterfaceUid,
		TUint aOperationId, 
		const TClientInfo& aSender,
		TSglQue<TRemConAddress>& aConnections,
		TSglQue<TBearerSecurity>& aBearerSecurity)
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressOutgoingCommand() starts" );
	
	// Avoid warnings for unused parameters
	(void)aInterfaceUid;
	(void)aOperationId;
	(void)aSender;
		
	TInt err = KErrNotFound;
	TSglQueIter<TBearerSecurity> iter( aBearerSecurity );
	TBearerSecurity* sec = iter;
	
	// Go through all bearers
	while( sec )
		{
		// No security check in this version.
		// Could maybe be smth like "if ( sec->SecurityPolicy().CheckPolicy( aSender.Message() ) )"
				
		// Address command to all given bearers
		TRemConAddress* conn = NULL;
		conn = new TRemConAddress;
		if ( conn )
			{
			COM_TRACE_1( "[REMCONTSPCONTROLLER] Sender's secure id = 0x%x", aSender.SecureId().iId );
			COM_TRACE_1( "[REMCONTSPCONTROLLER] Command sent to bearer 0x%x", sec->BearerUid().iUid );
			
			conn->BearerUid().iUid = sec->BearerUid().iUid; 
			// No connection-specific data for this bearer.
			// Ownership of conn is passed back to RemCon.
			aConnections.AddLast( *conn );
			err = KErrNone;
			}
		else
			{
			COM_TRACE_1( "[REMCONTSPCONTROLLER] Memory allocation failed for bearer 0x%x", sec->BearerUid().iUid );
			
			err = KErrNoMemory;
			break;	// Get out from while() and return KErrNoMemory
			}
		iter++;
		sec = iter;
		}

	COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::AddressOutgoingCommand() err=%d", err );
	Observer().OutgoingCommandAddressed( err );
	
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressOutgoingCommand() ends" );
	}

// -----------------------------------------------------------------------------
// CRemConTspController::PermitOutgoingCommand
// Defines whether connection-oriented controller can send a command to bearer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::PermitOutgoingCommand(
		TUid aInterfaceUid,
		TUint aOperationId, 
		const TClientInfo& aSender,
		const TRemConAddress& aConnection )
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingCommand()" );
	
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingCommand() start" );
	COM_TRACE_2( "[REMCONTSPCONTROLLER] aInterfaceUid = %d, aOperationId = %d", aInterfaceUid.iUid, aOperationId );
	COM_TRACE_1( "[REMCONTSPCONTROLLER] sender's secure id =0x%x", aSender.SecureId().iId );
				
	// Avoid warnings for unused parameters
	(void)aInterfaceUid;
	(void)aOperationId;
	(void)aSender;
	(void)aConnection;
	
	// Permit all connection oriented controllers to send commands 
	Observer().OutgoingCommandPermitted( ETrue );
	
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingCommand() ends" );
	}

// -----------------------------------------------------------------------------
// CRemConTspController::CancelOutgoingCommand
// Cancels the current AddressOutgoingCommand or PermitOutgoingCommand command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::CancelOutgoingCommand()
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::CancelOutgoingCommand()" );
	// Nothing to do until AddressOutgoingCommand() and/or PermitOutgoingCommand() 
	// will be implemented asynchronously.
	}


#ifdef _DEBUG
static void TraceRemconTargets( TSglQue<TClientInfo>& aClients )
    {
    // For debuging purpose write all remote target ids to trace
    TSglQueIter<TClientInfo> iter( aClients );
    TClientInfo* target = iter;
    while( target )
        {
        RProcess process;
        if ( TInt err = process.Open( target->ProcessId().Id() ) == KErrNone )
            {
            TFullName name( process.FullName() );
            COM_TRACE_2( "[REMCONTSPCONTROLLER] Registered target [%S] process id=%d", &name, target->ProcessId().Id() );
            }
        else
            {
            COM_TRACE_2( "[REMCONTSPCONTROLLER] Registered target ERROR process id=%d", err , target->ProcessId().Id() );
            }
        
        
        iter++;
        target = iter;
        }
    }
#endif

// -----------------------------------------------------------------------------
// CRemConTspController::AddressIncomingCommand
// Defines remote targets to which command will be sent.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::AddressIncomingCommand(
		TUid aInterfaceUid,
		TUint aOperationId, 
		TSglQue<TClientInfo>& aClients )
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressIncomingCommand() start" );
	COM_TRACE_2( "[REMCONTSPCONTROLLER] aInterfaceUid = %d, aOperationId = %d", aInterfaceUid.iUid, aOperationId );

#ifdef _DEBUG	
	TraceRemconTargets( aClients );
#endif
	TRAPD( err, GetCorrectClientL( aInterfaceUid, aOperationId, aClients, ETrue ) );
	Observer().IncomingCommandAddressed( err );
	
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressIncomingCommand() ends" );
	}

// -----------------------------------------------------------------------------
// CRemConTspController::AddressOutgoingNotify
// Not supported right now.
// -----------------------------------------------------------------------------
//
void CRemConTspController::AddressOutgoingNotify(
        TUid /*aInterfaceUid*/,
        TUint /*aOperationId*/, 
        const TClientInfo& /*aSender*/,
        TSglQue<TBearerSecurity>& /*aBearerSecurity*/)
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressOutgoingNotify() start" );           
 
    // Not yet implemented for connectionless controller. 
    // S60 AVRCP 1.4 controller is connection-oriented.
    Observer().OutgoingNotifyCommandAddressed( NULL, KErrNotSupported );
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressOutgoingNotify() ends" );     
    }

// -----------------------------------------------------------------------------
// CRemConTspController::PermitOutgoingNotifyCommand
// 
// -----------------------------------------------------------------------------
//
void CRemConTspController::CancelOutgoingNotifyCommand()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::CancelOutgoingNotifyCommand()" );
    // Nothing to do until AddressOutgoingNotify() or PermitOutgoingNotifyCommand() 
    // will be implemented asynchronously.  
    }

// -----------------------------------------------------------------------------
// CRemConTspController::PermitOutgoingNotifyCommand
// By default, permit all connection oriented controllers to send notify commands
// -----------------------------------------------------------------------------
//
void CRemConTspController::PermitOutgoingNotifyCommand(
        TUid /*aInterfaceUid*/,
        TUint /*aOperationId*/, 
        const TClientInfo& /*aSender*/,
        const TRemConAddress& /*aConnection*/)
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingNotifyCommand() start" );        
   
    // Permit all connection oriented controllers to send notify commands 
    Observer().OutgoingNotifyCommandPermitted( ETrue );
    
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingNotifyCommand() ends" );      
    }

// -----------------------------------------------------------------------------
// CRemConTspController::PermitIncomingCommand
// By default, permit all incoming addressed commands.  Check if the command
// comes from the AVRCP bearer an if so launch the music player.
// -----------------------------------------------------------------------------
void CRemConTspController::PermitIncomingCommand(
    TUid aInterfaceUid,
    TUint aOperationId, 
    const TClientInfo& aClient)
    {
	(void) aClient;  // Not used. 
    // Here we need to check the incoming command.  If it is an AVRCP play
    // command and there is no sensible handler running we should launch the
    // MPX Music Player.  This is the same as for AddressIncomingCommand.
    // Check if appropriate handler running
    if ((aInterfaceUid != TUid::Uid(KRemConCoreApiUid)) ||
         (aOperationId != ERemConCoreApiPlay) ||
         GetLocalAddressedClient())
        {
        // no action needed, allow commmand
        Observer().IncomingCommandPermitted(ETrue);
        }
     else 
        {
        // Launch an appropriate player in playing state. 
        TRAPD(err, ActivateApplicationL(TUid::Uid(KUid3MusicPlayer)))
        if(err != KErrNone)
            {

            }

        //deny this command
        Observer().IncomingCommandPermitted(EFalse);

        // We will be informed when the MPX music player connects its client 
        // session.  That will trigger a rules evaluation which will result in
        // us informing interested bearers of the new local addressed player.
        }
    }

// -----------------------------------------------------------------------------
// CRemConTspController::PermitIncomingNotify
// By default, permit all incoming addressed commands.  Check if the command
// comes from the AVRCP bearer an if so launch the music player.
// -----------------------------------------------------------------------------
void CRemConTspController::PermitIncomingNotify(
    TUid /*aInterfaceUid*/,
    TUint /*aOperationId*/, 
    const TClientInfo& /*aClient*/)
    {
    // No reason to stop these, just allow all
    Observer().IncomingNotifyPermitted(ETrue);
    }

// -----------------------------------------------------------------------------
// CRemConTspController::SetRemoteAddressedClient
// Ignore this event.  We don't use what the remote has selected to influence
// our routing policy.
// -----------------------------------------------------------------------------
void CRemConTspController::SetRemoteAddressedClient(const TUid& /*aBearerUid*/, 
        const TClientInfo& /*aClient*/)
    {
    }

// -----------------------------------------------------------------------------
// CRemConTspController::TargetClientAvailable
// A new client has connected.  Trigger a rule evaluation to see if we want to switch
// to this client.
// -----------------------------------------------------------------------------
void CRemConTspController::TargetClientAvailable(const TClientInfo& aClientInfo)
    {
    COM_TRACE_1("[REMCONTSPCONTROLLER] CRemConTspController::TargetClientAvailable aClientInfo.SecureId=0x%x", aClientInfo.SecureId().iId);

    TClientInfo* clientInfo;
    TSglQueIter<TClientInfo> iter(iAvailableTargets);
    
    while((clientInfo = iter++) != NULL)
        {
        if(clientInfo->SecureId() == aClientInfo.SecureId())
            {
            // Found a client and clientInfo points to that now. 
            break; 
            }
        }
    
    // If clientInfo was not found, create it and add to the available targets queue. 
    if(!clientInfo)
        {
        TClientInfo* newTarget = new TClientInfo();
        if(newTarget)
            {
            newTarget->ProcessId() = aClientInfo.ProcessId();
            newTarget->SecureId() = aClientInfo.SecureId();
            
            // Add to our list of available targets
            iAvailableTargets.AddLast(*newTarget);
            }
        }

    // Re-evaluate what the default addressed player should be if someone is interested to know 
    if(!iClientObservers.IsEmpty())
        {
        MtrtoEvaluateRoutingRules();
        }
    }

// -----------------------------------------------------------------------------
// CRemConTspController::TargetClientUnavailable
// A client has disconnected.  Trigger a rule evaluation to see if our default
// player has changed.
// -----------------------------------------------------------------------------
void CRemConTspController::TargetClientUnavailable(const TClientInfo& aClientInfo)
    {
    COM_TRACE_1("[REMCONTSPCONTROLLER] CRemConTspController::TargetClientUnavailable aClientInfo.SecureId=0x%x", aClientInfo.SecureId().iId);

    // Remove this from our list of available targets
    if(!iAvailableTargets.IsEmpty())
        {
        TClientInfo* clientInfo;
        TSglQueIter<TClientInfo> iter(iAvailableTargets);
        while((clientInfo = iter++) != NULL)
            {
            if(clientInfo->SecureId() == aClientInfo.SecureId())
                {
                iAvailableTargets.Remove(*clientInfo);
                delete clientInfo;
                break;
                }
            }
        }

    if(!iClientObservers.IsEmpty())
        {
        // Re-evaluate what the default addressed player should be if someone is listening. 
        MtrtoEvaluateRoutingRules();
        }
    }

// -----------------------------------------------------------------------------
// CRemConTspController::RegisterLocalAddressedClientObserver
// A bearer is interested in what the local addressed player is.  Start observing.
// -----------------------------------------------------------------------------
TInt CRemConTspController::RegisterLocalAddressedClientObserver(const TUid& aBearerUid)
    {
    COM_TRACE_1("[REMCONTSPCONTROLLER] CRemConTspController::RegisterLocalAddressedClientObserver aBearerUid.SecureId=0x%x", aBearerUid.iUid);
    TInt err = KErrNone;
    // Add this to our list of bearers interested in the default addressed player
    TClientObserver* clientObserver = new TClientObserver(aBearerUid);

    if(!clientObserver)
        {
        err = KErrNoMemory; 
        }

    // If this is our first interested bearer kick off the trigger events watcher.
    // This will let us know if any event occurs that should trigger us to re-evaluate
    // our addressing rules
    if(!iTriggerEventsWatcher && err == KErrNone)
        {
        TRAP(err, iTriggerEventsWatcher = CTspTriggerEventsWatcher::NewL(*this));
        if(err != KErrNone)
            {
            // If we couldn't create the events watcher the client Observer is not needed either.
            delete clientObserver;
            }
        }

    if(err == KErrNone)
        {
        // Finally add the observer to the queue if it's not there yet. 
        TSglQueIter<TClientObserver> iter(iClientObservers);
        TClientObserver* obsInQueue;
        while((obsInQueue = iter++) != NULL)
            {
            if(obsInQueue->iBearerUid == aBearerUid)
                {
                err = KErrAlreadyExists; 
                break;
                }
            }

        if(!obsInQueue)
            {
            iClientObservers.AddLast(*clientObserver);
            MtrtoEvaluateRoutingRules();
            }
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::UnregisterLocalAddressedClientObserver
// The bearer is no longer interested in observering default client changes.
// -----------------------------------------------------------------------------
TInt CRemConTspController::UnregisterLocalAddressedClientObserver(const TUid& aBearerUid)
    {
    COM_TRACE_1("[REMCONTSPCONTROLLER] CRemConTspController::UnregisterLocalAddressedClientObserver aBearerUid.SecureId=0x%x", aBearerUid.iUid);
    // Remove this from our list of bearers interested in the default addressed player.
    // If there are no interested bearers left then we can stop watching for rules
    // triggers.
    TSglQueIter<TClientObserver> iter(iClientObservers);
    TClientObserver* clientObserver;
    while((clientObserver = iter++) != NULL)
        {
        if(clientObserver->iBearerUid == aBearerUid)
            {
            iClientObservers.Remove(*clientObserver);
	    delete clientObserver;
            break;
            }
        }
    
    if(iClientObservers.IsEmpty())
        {
        delete iTriggerEventsWatcher;
        iTriggerEventsWatcher = NULL;
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::GetCorrectClientL
// Defines remote targets to which command will be sent.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::GetCorrectClientL(
    TUid aInterfaceUid,
	TUint aKeyEvent,
	TSglQue<TClientInfo>& aClients,
	TBool aLaunchingNewApplicationAllowed)
	{
	COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Start aInterfaceUid %d", aInterfaceUid );
	COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Start aKeyEvent %d", aKeyEvent );

    TInt numOfTables = iArrayOfTables.Count();
    RArray<TInt> rulesArray;
    CleanupClosePushL( rulesArray );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Get Correct Table, table count %d", numOfTables );
    for( TInt i = 0; i != numOfTables; i++ )
        {
        // If an extension api event
        if( ( aInterfaceUid.iUid == KRemConExtCallHandlingApiUid ) ||
          ( aInterfaceUid.iUid == KRemConExtPocApiUid ) ||
          ( aInterfaceUid.iUid == KRemConExtSynchronizationApiUid ) ||
          ( aInterfaceUid.iUid == 0 ) )
    	    {
    	    TBool value( EFalse );
    	    if( aInterfaceUid.iUid == KRemConExtCallHandlingApiUid )
    	        {
    	        value = iArrayOfTables[ i ]->IsRemConEventInTable(
    	                       aKeyEvent, CRemConEventTable::ERemConExtEventCall );
    	        }
    	    if( aInterfaceUid.iUid == KRemConExtPocApiUid )
    	        {
    	        value = iArrayOfTables[ i ]->IsRemConEventInTable(
    	                       aKeyEvent, CRemConEventTable::ERemConExtEventPoc );
    	        }
    	    if( aInterfaceUid.iUid == KRemConExtSynchronizationApiUid )
    	        {
    	        value = iArrayOfTables[ i ]->IsRemConEventInTable(
    	                       aKeyEvent, CRemConEventTable::ERemConExtEventSync );
    	        }
    	    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Table num for extension event aKeyEvent %d ", aKeyEvent );
    	    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Table num for extension event i %d ", i );
    	    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Table num for extension event value %d ", value );
    	    if( value )
    	        {
    	        iArrayOfTables[ i ]->GetRulesForThisTable( rulesArray );
    	        numOfTables = iArrayOfTables[ i ]->TableNum();
    	        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Table num for extension event numOfTables %d ", numOfTables );
    	        break;
    	        }
    	    }
    	else // Rem con core event
    	    {
    	    if( iArrayOfTables[ i ]->IsRemConEventInTable( 
    	                        aKeyEvent, CRemConEventTable::ERemConEvent ) )
    	        {
    	        iArrayOfTables[ i ]->GetRulesForThisTable( rulesArray );
    	        numOfTables = iArrayOfTables[ i ]->TableNum();
    	        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Table num for core event numOfTables %d ", numOfTables );
    	        break;
    	        }
    	    }
        }
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Get client" );

    // Check for current transparent clients
    RPointerArray<TClientInfo> transparentClients; // Pointers are not own
    CleanupClosePushL( transparentClients );
    CRemConEventTable* table = FindRoutingTable( iArrayOfTables, numOfTables );
    if ( table )
        {
        RArray<TInt> transparentApps;
        table->GetTransparentApplicationsForTable( transparentApps );
        for ( TInt i = transparentApps.Count() - 1; i >= 0; --i )
            {
            TSecureId secureId( transparentApps[ i ] );
            TClientInfo* target = FindRemconConnection( secureId, aClients );
            if ( target )
                {
                transparentClients.Append( target ); // Ignore error
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() transparent client 0x%x found", transparentApps[ i ] );
                }
            }
        transparentApps.Close();
        }

    // If there is a rule for this key event this for is entered
    for( TInt i = 0; i != rulesArray.Count(); i++ )
        {
        TBool found( EFalse );
        switch( rulesArray[ i ] )
            {
            case CRemConEventTable::EActiveAudio:
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Get active audio client" );
                
                found = FindActiveAudioFromAudioPolicy( aClients );
                
                if ( !found )
                    {
                    found = FindActiveAudioFromCustomIfL( aClients );
                    }
                break;
                }
                
            case CRemConEventTable::EForegroundApplication:
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Get foreground client" );
                GetActiveWindowClientL();
                TSglQueIter<TClientInfo> iter( aClients );
            	TClientInfo* target = iter;
            	while( target )
			        {
            	    TProcessId processId = target->ProcessId();
            	    if( iProcessIdForeground == processId )
            	        {
                        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Foreground client found" );
            	        aClients.Reset();
            	        aClients.AddFirst( *target );
            	        found = ETrue;
            	        break;
            	        }
            	    iter++;
            	    target = iter;
            	    }
                break;
                }
            case CRemConEventTable::EDefaultApplicationGroup:
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Default application client" );
                // Retrieve default applications from correct table
                CRemConEventTable* table = FindRoutingTable( iArrayOfTables, numOfTables );
                if ( table )
                    {
                    RArray<TInt> defaultApplications;
                    table->GetDefaultApplicationsForTable( defaultApplications );
                    
                    for ( TInt i = 0; i < defaultApplications.Count() && !found; i++ )
                        {
                        TUid defaultApp( defaultApp.Uid( defaultApplications[ i ] ) );
                        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() defaultAppUid 0x%x", defaultApp.iUid );
                        
                        TSglQueIter<TClientInfo> iter( aClients );
                        TClientInfo* target = iter;
                        while( target )
                            {
                            TUid tuid( target->SecureId() );
                            COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Default application client 0x%x", tuid.iUid );
                            
                            if( defaultApp == tuid )
                                {
                                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() default app found" );
                                aClients.Reset();
                                aClients.AddFirst( *target );
                                found = ETrue;
                                break;
                                }
                            iter++;
                            target = iter;
                            }
                        }
                    defaultApplications.Close();
                    }
                break;
                }
            case CRemConEventTable::ELastPlayedMediaApplication:
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Get last played media application client" );

                TClientInfo* active = FindRemconConnection( iProcessIdActive, aClients );
                TClientInfo* fg = FindRemconConnection( iProcessIdForeground, aClients );
                if ( active && !fg )
                    {
                    aClients.Reset();
                    aClients.AddFirst( *active );
                    found = ETrue;
                    }   
                else
                    {
                    RArray<TProcessId> array;
                    CleanupClosePushL( array );
                    GetActivePidsL( array );
                    
                    for( TInt a = 0; a != array.Count(); a++ )
                        {
                        TProcessId processId1 = array[ a ];
                        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() TSP custom IF client ProcessId %d", (TInt)processId1 );
                        TSglQueIter<TClientInfo> iter( aClients );
                        TClientInfo* target = iter;
                        
                        while( target )
                            {
                            TProcessId processId = target->ProcessId();
                            if( processId1 == processId )
                                {
                                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() TSP custom IF client ProcessId found %d", (TInt)processId1 );
                                aClients.Reset();
                                aClients.AddFirst( *target );
                                found = ETrue;
                                break;
                                }
                            iter++;
                            target = iter;
                            }
                        
                        if( found )
                            {
                            break;
                            }
                        }
                    CleanupStack::PopAndDestroy( &array );
                    }
                break;
                }
            case CRemConEventTable::ELaunchDefaultApp:
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() Launch default application" );                
                if ( !DeviceLocked() && aLaunchingNewApplicationAllowed )
                    {
                    TUid defaultLaunchAppUid;
                    
                    CRemConEventTable* table = FindRoutingTable( iArrayOfTables, numOfTables );
                    if ( table )
                        {
                        table->GetDefaultLaunchAppForTable( defaultLaunchAppUid );
                    	}
                    
                    aClients.Reset();
                    
                    ActivateApplicationL( defaultLaunchAppUid );
                    }
                else
                    {
                    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL(): device locked" );
                    aClients.Reset();
                    }
                break;
                }
            default:
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() No client found");
                break;
                }
            }
        if( found )
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() ProcessId found");
            break;
            }
        else if( rulesArray.Count() - 1 == i )
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() No client found");
            // Reset the list to reflect the fact that no clients were found. 
            // The possible transparent clients will be added still. 
            aClients.Reset();
            }
        }

    // Add transparent clients
    for ( TInt i = transparentClients.Count() - 1; i >= 0; --i )
        {
        TClientInfo* target = transparentClients[ i ];
        if ( !FindRemconConnection( target->SecureId(), aClients ) ) // Add client only if not already found
            {
            // Add to the end of the list, any other client should take priority over the transparent clients. 
            aClients.AddLast( *target );  
            COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetCorrectClientL() transparent client 0x%x added", target->SecureId().iId );
            }
        }
    CleanupStack::PopAndDestroy( &transparentClients ); // Pointers are not own
    CleanupStack::PopAndDestroy( &rulesArray );
	}

// -----------------------------------------------------------------------------
// CRemConTspController::FindRemconConnection
// Checks if process is connected to Remcon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TClientInfo* CRemConTspController::FindRemconConnection( TProcessId aProcessId, TSglQue<TClientInfo>& aClients ) const
    {
    TClientInfo* ret = NULL;
    TSglQueIter<TClientInfo> iter( aClients );
    TClientInfo* target = iter;
    while( target )
        {
        TProcessId processId = target->ProcessId();
        if( aProcessId == processId )
            {
            ret = target;
            COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindRemconConnection() ProcessId: %d found", (TInt)processId );
            break;
            }
        iter++;
        target = iter;
        }
    if (!ret)
        {
        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindRemconConnection() ProcessId: %d not found", (TInt)aProcessId );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::FindRemconConnection
// Checks if client with given secure id is connected to Remcon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TClientInfo* CRemConTspController::FindRemconConnection( TSecureId aSecureId, TSglQue<TClientInfo>& aClients ) const
    {
    TClientInfo* ret = NULL;
    TSglQueIter<TClientInfo> iter( aClients );
    TClientInfo* target = iter;
    while( target )
        {
        TSecureId secureId = target->SecureId();
        if( aSecureId == secureId )
            {
            ret = target;
            COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindRemconConnection() SecureId: 0x%x found", aSecureId.iId );
            break;
            }
        iter++;
        target = iter;
        }
    if (!ret)
        {
        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindRemconConnection() SecureId: 0x%x not found", aSecureId.iId );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::GetActivePidsL
// Not used.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CRemConTspController::GetActivePidsL( RArray<TProcessId>& aArray )
    {
    CArrayFixFlat<TInt>* windowGroupIds = new( ELeave ) CArrayFixFlat<TInt>( 4 );
    CleanupStack::PushL( windowGroupIds );
    RWsSession session;
    User::LeaveIfError( session.Connect() );
    CleanupClosePushL( session );
    User::LeaveIfError( session.WindowGroupList( windowGroupIds ) );
    TInt count = windowGroupIds->Count();
    
    /**
    * Get audio policy clients and tsp clients
    */
    TAudioPolicyProcessIdList idList;
    idList().iNumOfProcesses = 0;
    
    TInt err = RProperty::Get(
        KPSUidMMFAudioServer,
        KAudioPolicyApplicationAudioStatePaused,
        idList );
   
    if ( err != KErrNone )
        {
        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() KAudioPolicyApplicationAudioStatePaused read error %d", err );
        }
    
#if ( defined COMPONENT_TRACE_FLAG && _DEBUG )
    TraceAudioPolicyClients( KAudioPolicyApplicationAudioStatePaused, idList );
#endif

    RArray<TProcessId> array;
    CleanupClosePushL( array );
    CTspClientMapper* customIf = CTspClientMapper::NewLC();
    err = customIf->GetTspTargetClients(
            CTspClientMapper::EStoppedClients, array );
    CleanupStack::PopAndDestroy( customIf );
    
    /**
    * Now sort the process ids so that last played is first
    */
    for( TInt i = 0; i < count; i++ )
        {
        TBool wasFound( EFalse );
        TApaTask task( session );
        task.SetWgId( windowGroupIds->At( i ) );
        TUint threadId = ( TUint )( task.ThreadId() );
        RThread thread;
        RProcess process;
        TInt err = thread.Open( threadId, EOwnerProcess );
        
        if( err == KErrNone )
            {
            err = thread.Process( process );
            
            if( err == KErrNone )
                {
                TInt errFoundTsp = array.Find( process.Id() );
                
#if ( defined COMPONENT_TRACE_FLAG && _DEBUG )
                TFullName name( process.FullName() );
                COM_TRACE_4( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() checking process %S, id %d, wgid %d, prio %d", 
                    &name, (TInt)process.Id(), task.WgId(), session.GetWindowGroupOrdinalPriority(task.WgId()) );
#endif
                
                if( errFoundTsp != KErrNotFound )
                    {
                    // Check for duplicate window group entries
                    errFoundTsp = aArray.Find( process.Id() );
                    wasFound = ETrue; // was found either way
                    
                    if( errFoundTsp == KErrNotFound )
                        {
                        err = aArray.Append( process.Id() );
                        COM_TRACE_2( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() processId = %d added (tsp client) to array, err %d", 
                            (TInt)process.Id(), err );
                        }
                    else
                        {
                        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() processId = %d already exists", 
                            (TInt)process.Id() );
                        }
                    }
                
                // Don't add it twice, the process id can be found from both lists
                if( !wasFound )
                    {
                    for( TInt j = 0; j < idList().iNumOfProcesses; j++ )
                        {
                        if( idList().iProcessList[ j ] == process.Id() )
                            {
                            aArray.Append( process.Id() );
                            COM_TRACE_2( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() processId = %d added (audio client) to array, err %d", 
                                (TInt)process.Id(), err );
                            }
                        }
                    }
                }
            else
                {
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() error opening process", err );
                }
            }
        else
            {
            COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetActivePidsL() error opening thread", err );
            }
            
        thread.Close();
        process.Close();
        }
    
    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( &session );
    CleanupStack::PopAndDestroy( windowGroupIds );
    }
	
// -----------------------------------------------------------------------------
// CRemConTspController::ContextIndicationL
// Not used.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CRemConTspController::ContextIndicationL(
    const CCFContextIndication&/* aChangedContext*/ )
    {
    }

// -----------------------------------------------------------------------------
// CRemConTspController::SetKeyEventTableL
// Set the key events to the correct table.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CRemConTspController::SetKeyEventTableL(
    const CCFActionIndication& aActionToExecute )
    {
    TInt cellAllocation( 0 );
    TInt cellCount( User::AllocSize( cellAllocation ) );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CELL COUNT: - Enter %d ", cellCount );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CELL ALLOCATION - Enter %d ", cellAllocation );

    const RKeyValueArray& params( aActionToExecute.Parameters() );
    TInt paramCount( params.Count() );
    CRemConEventTable* table = NULL;
    for( TInt i( 0 ); i < paramCount; i++ )
        {
        const CCFKeyValuePair& parameter( *( params[ i ] ) );
        TPtrC paramKey( parameter.Key() );
        TPtrC paramValue( parameter.Value() );
        if( paramKey == KRoutingTable ) // Handle table setup events first
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - routing table " );
            TUint tableNum( HexStringToInt( paramValue ) );
            table = FindRoutingTable( iArrayOfTables, tableNum ); // Save current table for later use
            if( !table )
                {
                table = CRemConEventTable::NewL( tableNum );
                CleanupStack::PushL( table );
                iArrayOfTables.AppendL( table );
                CleanupStack::Pop( table );
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - added routing table %d", tableNum );
                }
            }
        else if( paramKey == KRemoveRoutingTable )
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - removeRoutingTable table " );
            TUint tableNum( HexStringToInt( paramValue ) );
            TInt removePos( FindRoutingTablePos( iArrayOfTables, tableNum ) );
            if ( removePos >= 0 && removePos < iArrayOfTables.Count() ) // Remove table if it exist
                {
                delete iArrayOfTables[ removePos ];
                iArrayOfTables.Remove( removePos );
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - removeRoutingTable table %d", tableNum );
                }
            table = NULL; // Reset the current table
            }
        else if ( paramKey == KStoreRoutingTable )
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - storeRoutingTable " );
            TUint tableNum( HexStringToInt( paramValue ) );
            if ( !FindRoutingTable( iArrayOfStoredTables, tableNum ) ) // Store table if not already stored
                {
                TInt pos( FindRoutingTablePos( iArrayOfTables, tableNum ) );
                if ( pos >= 0 && pos < iArrayOfTables.Count() )
                    {
                    iArrayOfStoredTables.AppendL( iArrayOfTables[ pos ] );
                    iArrayOfTables.Remove( pos );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - storeRoutingTable %d ", tableNum );
                    }
                }
            else
                {
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - storeRoutingTable already stored %d ", tableNum );
                }
            table = NULL; // Reset the current table
            }
        else if ( paramKey == KRestoreRoutingTable )
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - restoreRoutingTable " );
            TUint tableNum( HexStringToInt( paramValue ) );
            TInt storedPos( FindRoutingTablePos( iArrayOfStoredTables, tableNum ) );
            if ( storedPos >= 0 && storedPos < iArrayOfStoredTables.Count() ) // Restore table if stored earlier
                {
                TInt replacePos( FindRoutingTablePos( iArrayOfTables, tableNum ) );
                if ( replacePos >= 0 && replacePos < iArrayOfTables.Count() )
                    {
                    // Delete existing table and take stored in use
                    delete iArrayOfTables[ replacePos ];
                    iArrayOfTables[ replacePos ] = iArrayOfStoredTables[ storedPos ];
                    iArrayOfStoredTables.Remove( storedPos );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - restoreRoutingTable replaced %d ", tableNum );
                    }
                else
                    {
                    // Just take stored table in use
                    iArrayOfTables.AppendL( iArrayOfStoredTables[ storedPos ] );
                    iArrayOfStoredTables.Remove( storedPos );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - restoreRoutingTable added %d ", tableNum );
                    }
                }
            else
                {
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - restoreRoutingTable already restored %d ", tableNum );
                }
            table = NULL; // Reset the current table
            }
        else if ( table ) // Handle the events of current table 
            {
            TUint keyEventValue( HexStringToInt( paramValue ) );
            if( paramKey.Match( KRemConKeyEvent ) != KErrNotFound )
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - key event " );
                if ( paramKey.Match( KRemConKeyEventExt ) == KErrNotFound )
                    {
                    table->AddRemConEventToTable( keyEventValue, CRemConEventTable::ERemConEvent );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - core key event %d", keyEventValue );
                    }
                else if( paramKey.Match( KRemConKeyEventExtCall ) != KErrNotFound )
                    {
                    table->AddRemConEventToTable( keyEventValue, CRemConEventTable::ERemConExtEventCall );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - ext call event %d ", keyEventValue );
                    }
                else if( paramKey.Match( KRemConKeyEventExtPoc ) != KErrNotFound )
                    {
                    table->AddRemConEventToTable( keyEventValue, CRemConEventTable::ERemConExtEventPoc );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - ext poc event %d ", keyEventValue );
                    }
                else if( paramKey.Match( KRemConKeyEventExtSync ) != KErrNotFound )
                    {
                    table->AddRemConEventToTable( keyEventValue, CRemConEventTable::ERemConExtEventSync );
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - ext sync event %d ", keyEventValue );
                    }
                else
                    {
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - unknown key event %S", &paramKey );
                    }
                }
            else if( paramKey.Match( KRuleText ) != KErrNotFound )
                {
                table->AddRuleForThisTable( keyEventValue );
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - rule %d ", keyEventValue );
                }
            else if( paramKey.Match( KDefaultAppText ) != KErrNotFound )
                {
                table->AddDefaultApplicationForThisTable( keyEventValue );
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - default app 0x%x ", keyEventValue );
                }
            else if( paramKey.Match( KDefaultLaunchAppText ) != KErrNotFound )
                {
                table->AddDefaultLaunchAppForThisTable( keyEventValue );
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - default launch app 0x%x ", keyEventValue );
                }
            else if ( paramKey.Match( KTransparentAppText ) != KErrNotFound )
                {
                table->AddTransparentApplicationForThisTable( keyEventValue );
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::SetKeyEventTableL() - transparent app 0x%x ", keyEventValue );
                }
            }
        }

    cellCount =  User::AllocSize( cellAllocation );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CELL COUNT: - Return %d ", cellCount );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CELL ALLOCATION - Return %d ", cellAllocation );
    }

// -----------------------------------------------------------------------------
// CRemConTspController::ActionIndicationL
// Actions related to the context.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::ActionIndicationL(
    const CCFActionIndication& aActionToExecute )
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::ActionIndicationL() " );
    _LIT( KTsp, "Tsp" );
    if( aActionToExecute.Identifier() == KTsp )
        {
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::ActionIndicationL() KTsp " );
        SetKeyEventTableL( aActionToExecute );
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::ActionIndicationL() All actions read" );
        }
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::ActionIndicationL() Everything done" );
    }

// -----------------------------------------------------------------------------
// CRemConTspController::HandleContextFrameworkError
// Not used.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::HandleContextFrameworkError( 
    TCFError /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CCFAPITest::Extension
// -----------------------------------------------------------------------------
//
TAny* CRemConTspController::Extension( const TUid& /*aExtensionUid*/ ) const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::MtrtoEvaluateRoutingRules
// Evaluates the TSP's routing rules to determine if the local addressed player
// has changed, and if so informs interested bearers
// -----------------------------------------------------------------------------
void CRemConTspController::MtrtoEvaluateRoutingRules()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::MtrtoEvaluateRoutingRules() - Enter" ); 
    TClientInfo* localAddressedClient = GetLocalAddressedClient();

    if(!localAddressedClient)
        {
        // If there's no suitable client, then there's nothing to do here.   
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::MtrtoEvaluateRoutingRules() No local addressed client found" ); 
        return; 
        }

    if(!iLocalAddressedClient || (localAddressedClient->SecureId() != iLocalAddressedClient->SecureId()))
        {
        // Local addressed player has changed (or wasn't set before).  
        iLocalAddressedClient = localAddressedClient;
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::MtrtoEvaluateRoutingRules() Local addressed client has changed" ); 
        TSglQueIter<TClientObserver> iter(iClientObservers);
        TClientObserver* clientObserver;
        while((clientObserver = iter++) != NULL)
            {
            COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::MtrtoEvaluateRoutingRules() - informing observer" ); 
            Observer().SetLocalAddressedClient(clientObserver->iBearerUid, *iLocalAddressedClient);
            }
        }
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::MtrtoEvaluateRoutingRules() Local addressed client SID = 0x%x",  iLocalAddressedClient->SecureId().iId);
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::MtrtoEvaluateRoutingRules() - Return" ); 
    }

// -----------------------------------------------------------------------------
// CRemConTspController::CreateAfterIdleL
// Define the contexts when idle state has been loaded.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CRemConTspController::CreateAfterIdleL()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::CreateAfterIdleL() - Enter" );	
    iCFClient = CCFClient::NewL( *this );
    // delete iIdle now, so that CRemConIdleListener won't call this method again if a leave occurs hereafter
    if( iIdle )
        {
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::~CRemConTspController() - iIdle exists, destroy" );
        delete iIdle;
        iIdle = NULL;
        }
    _LIT( KAction, "Tsp" );
    iCFClient->DefineAction( KAction, TSecurityPolicy::EAlwaysPass );
    CCFActionSubscription* temp = CCFActionSubscription::NewLC();
    temp->SetActionIdentifierL( KAction );
    iCFClient->SubscribeAction( *temp );
    CleanupStack::PopAndDestroy( temp );
    _LIT( KSource, "Tsp" );
    _LIT( KType, "Status" );
    _LIT( KValue, "Ready" );
    iCFClient->DefineContext( KSource, KType, TSecurityPolicy::EAlwaysPass );
    CCFContextObject* obj = CCFContextObject::NewL( KSource, KType, KValue );
    iCFClient->PublishContext( *obj );
    delete obj;
    obj = NULL;
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::CreateAfterIdleL() - Return" );	
    }

// -----------------------------------------------------------------------------
// CRemConTspController::GetActiveWindowClientL
// Gets the currently active window.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::GetActiveWindowClientL()
    {
    CArrayFixFlat<TInt>* windowGroupIds = new( ELeave ) CArrayFixFlat<TInt>( 4 );
    CleanupStack::PushL( windowGroupIds );
    RWsSession session;
    User::LeaveIfError( session.Connect() );
    CleanupClosePushL( session );
    User::LeaveIfError( session.WindowGroupList( 0, windowGroupIds ) );

    TApaTask task( session );
    task.SetWgId( windowGroupIds->At( 0 ) );
    TUint threadId = ( TUint )( task.ThreadId() );
    RThread thread;
    TInt err = thread.Open( threadId, EOwnerProcess );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetActiveWindowClientL() threadOpen = %d", err );
    
    if( err == KErrNone )
        {
        RProcess process;
        err = thread.Process( process );
        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetActiveWindowClientL() threadProcess = %d", err );
        //Close thread
        thread.Close();    
        if( err == KErrNone )
            {
            iProcessIdForeground = process.Id();
            }
        //Close RProcess
        process.Close();
        }

    CleanupStack::PopAndDestroy( &session );    
    CleanupStack::PopAndDestroy( windowGroupIds );
    }

// -----------------------------------------------------------------------------
// CRemConTspController::PermitOutgoingResponse
// Defines which client can send a response to a command to bearer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::PermitOutgoingResponse(
		TUid aInterfaceUid,
		TUint aOperationId,
		const TClientInfo& aClient,
		TClientInfoConstIter& aClients)
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingResponse()" );
	
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingResponse() start" );
	COM_TRACE_2( "[REMCONTSPCONTROLLER] aInterfaceUid = %d, aOperationId = %d", aInterfaceUid.iUid, aOperationId );
				
	// Avoid warnings for unused parameters
	(void)aInterfaceUid;
	(void)aOperationId;
	(void)aClient;
	(void)aClients;

	Observer().OutgoingResponsePermitted(ETrue);
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::PermitOutgoingResponse() ends" );
	}


// -----------------------------------------------------------------------------
// CRemConTspController::CancelOutgoingResponse
// Called by RemCon to cancel the current PermitOutgoingResponse request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::CancelOutgoingResponse()
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::CancelOutgoingCommand()" );
	// Nothing to do until PermitOutgoingResponse() 
	// will be implemented asynchronously.
	}


// -----------------------------------------------------------------------------
// CRemConTspController::AddressIncomingNotify
// Defines remote targets to which notify will be sent.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRemConTspController::AddressIncomingNotify(
		TUid aInterfaceUid,
		TUint aOperationId,
		TSglQue<TClientInfo>& aClients)
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressIncomingNotify() start" );
	COM_TRACE_2( "[REMCONTSPCONTROLLER] aInterfaceUid = %08x, aOperationId = %08x", aInterfaceUid.iUid, aOperationId );
	(void)aInterfaceUid;
	(void)aOperationId;

	TClientInfo* clientInfo = aClients.First();
	TSglQueIter<TClientInfo> iter( aClients );
	TClientInfo* target = iter;
	if(target == NULL)
		{
		COM_TRACE_( "[REMCONTSPCONTROLLER] No target!" );
		}
		
	while( target )
		{
		COM_TRACE_1( "[REMCONTSPCONTROLLER]process secure id=%08x", target->SecureId().iId );
		if( ( aInterfaceUid.iUid == KRemConAbsoluteVolumeControllerApiUid &&
		        target->SecureId().iId == KUid3BTAudioMan ) || 
		    ( aInterfaceUid.iUid != KRemConAbsoluteVolumeControllerApiUid &&
		        target->SecureId().iId == KUid3MusicPlayer ) ||
		    ( aInterfaceUid.iUid == KRemConMediaBrowseApiUidTemp &&
		        target->SecureId().iId == KUid3BTAudioMan ))
			{
			COM_TRACE_( "[REMCONTSPCONTROLLER] We are here" );
			COM_TRACE_1( "[REMCONTSPCONTROLLER] Command is sent to process secure id=%08x", target->SecureId().iId );
			break;	// get out from while-loop
			}
		iter++;
		target = iter;
		}

	Observer().IncomingNotifyAddressed(target, KErrNone);

	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::AddressIncomingNotify() ends" );

	}
    
// ----------------------------------------------------------------------------
// CRemConTspController::ActivateApplicationL()
// Activates an application based to UID
// ----------------------------------------------------------------------------
//
void CRemConTspController::ActivateApplicationL( const TUid aUid ) const
    {
    COM_TRACE_( "CRemConTspController::ActivateApplicationL" );
    
    // Applications must not be activated before reaching normal system operation state
    if ( IsStateNormal() )
        {
        TInt err( KErrNone );
        TApaAppInfo appInfo;
        RApaLsSession apaLsSession;
        User::LeaveIfError( apaLsSession.Connect() );
        CleanupClosePushL( apaLsSession );
        err = apaLsSession.GetAppInfo( appInfo, aUid );
        if ( !err )
            {
            COM_TRACE_( "CRemConTspController::ActivateApplicationL: starting application" );
            CApaCommandLine* apaCommandLine = CApaCommandLine::NewLC();
            apaCommandLine->SetExecutableNameL( appInfo.iFullName );
            apaCommandLine->SetCommandL( EApaCommandRun );            
            _LIT8(Kparam, "play");
            apaCommandLine->SetTailEndL( Kparam );
            apaLsSession.StartApp( *apaCommandLine );
            CleanupStack::PopAndDestroy( apaCommandLine );
            }
        else
            {
            COM_TRACE_1( "CRemConTspController::ActivateApplicationL err=%d", err  );
            }                
        // pop and destroy will also close this session
        CleanupStack::PopAndDestroy( &apaLsSession );
        }        
    }

// ----------------------------------------------------------------------------
// CRemConTspController::IsStateNormal()
// Checks is phone started to normal phase.
// ----------------------------------------------------------------------------
TBool CRemConTspController::IsStateNormal() const
    {
    TInt state;
    TInt errorCode =
        RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state );
    if ( errorCode == KErrNone )
        {
        return state == ESwStateNormalRfOn ||
               state == ESwStateNormalRfOff ||
               state == ESwStateNormalBTSap;
        }
    else
        {
        COM_TRACE_1( "CRemConTspController::IsStateNormal: Failed to read global system state, error code %d.", errorCode );
        return EFalse;
        }
    }

// ---------------------------------------------------------
// CRemConTspController::DeviceLocked()
// ---------------------------------------------------------
//
TBool CRemConTspController::DeviceLocked() const
    {
    TInt lockStatus( EAutolockStatusUninitialized );
    TBool locked( EFalse );
    
    TInt err( RProperty::Get( KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, lockStatus ) );
        
    if ( err == KErrNone )
        {
        locked = lockStatus > EAutolockOff;
        }
    
    COM_TRACE_2( "CRemConTspController::DeviceLocked: lockStatus=%d, err=%d", 
                 lockStatus, err );
    
    return locked;
    }

#if ( defined COMPONENT_TRACE_FLAG && _DEBUG )
// -----------------------------------------------------------------------------
// CRemConTspController::TraceAudioPolicyClients
// Prints information about audio policy clients.
// -----------------------------------------------------------------------------
//
void CRemConTspController::TraceAudioPolicyClients( 
    const TUint32 aState, 
    TAudioPolicyProcessIdList aIdList )
    {
    TInt cou = aIdList().iNumOfProcesses;
    COM_TRACE_2( "[REMCONTSPCONTROLLER] CRemConTspController::TraceAudioPolicyClients() Amount of %d-state clients %d (only ids !=0 printed)", 
        aState, cou );

    for( TInt i = 0; i < cou; i++ )
        {
        if (aIdList().iProcessList[ i ].Id() != 0 )
            {
            RProcess process;
            TInt err = process.Open( aIdList().iProcessList[ i ] );
            if( err == KErrNone )
                {
                TFullName name( process.FullName() );
                COM_TRACE_2( "[REMCONTSPCONTROLLER] CRemConTspController::TraceAudioPolicyClients() audio client %S, id %d", 
                    &name, (TInt)process.Id() );
                }
            else
                {
                COM_TRACE_2( "[REMCONTSPCONTROLLER] CRemConTspController::TraceAudioPolicyClients() err %d opening process %d", 
                    err, (TInt)aIdList().iProcessList[ i ] );
                }
            }
        }
    }
#endif

// ----------------------------------------------------------------------------
// CRemConTspController::FindRoutingTablePos()
//
// ----------------------------------------------------------------------------
TInt CRemConTspController::FindRoutingTablePos(
        const RPointerArray<CRemConEventTable>& aTableArray, TInt aTableNum ) const
    {
    COM_TRACE_2( "CRemConTspController::FindRoutingTablePos Table 0x%x, TableNum %d", &aTableArray, aTableNum );

    TInt ret( KErrNotFound );
    TInt count( aTableArray.Count() );
    for( TInt i( 0 ); i < count; i++ )
        {
        CRemConEventTable* table = aTableArray[ i ];
        if( table->TableNum() == aTableNum )
            {
            ret = i;
            break;
            }
        }

    COM_TRACE_1( "CRemConTspController::FindRoutingTablePos Returns %d", ret );
    return ret;
    }

// ----------------------------------------------------------------------------
// CRemConTspController::FindRoutingTable()
//
// ----------------------------------------------------------------------------
CRemConEventTable* CRemConTspController::FindRoutingTable(
        const RPointerArray<CRemConEventTable>& aTableArray, TInt aTableNum ) const
    {
    COM_TRACE_2( "CRemConTspController::FindRoutingTable Table 0x%x, TableNum %d", &aTableArray, aTableNum );

    CRemConEventTable* ret = NULL;
    TInt pos( FindRoutingTablePos( aTableArray, aTableNum ) );
    if ( pos >= 0 && pos < aTableArray.Count() )
        {
        ret = aTableArray[ pos ];
        }

    COM_TRACE_1( "CRemConTspController::FindRoutingTable Returns 0x%x", ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::FindActiveAudioFromAudioPolicy
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CRemConTspController::FindActiveAudioFromAudioPolicy( TSglQue<TClientInfo>& aClients )
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::FindActiveAudioFromAudioPolicy()" );
    
    TAudioPolicyProcessIdList idList;
    idList().iNumOfProcesses = 0;
    TInt err( RProperty::Get(KPSUidMMFAudioServer, KAudioPolicyApplicationAudioStatePlaying, idList ) );
    
    if ( err != KErrNone )
        {
        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindActiveAudioFromAudioPolicy() KAudioPolicyApplicationAudioStatePlaying read error %d", err );
        }
    
#if ( defined COMPONENT_TRACE_FLAG && _DEBUG )
    TraceAudioPolicyClients( KAudioPolicyApplicationAudioStatePlaying, idList );
#endif
    
    if( idList().iNumOfProcesses > 0 ) // There is an active audio client
        {
        TSglQueIter<TClientInfo> iter( aClients );
        TClientInfo* target = iter;
        
        while( target )
            {
            TProcessId processId = target->ProcessId();
            
            for( TInt i = 0; i < idList().iNumOfProcesses; i++ ) // use the normal playing audio client
                {
                TProcessId processId1 = idList().iProcessList[ i ];
                
                if( processId1 == processId )
                    {
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindActiveAudioFromAudioPolicy() found process id %d", (TInt)processId1 );
                    aClients.Reset();
                    aClients.AddFirst( *target );
                    iProcessIdActive = target->ProcessId(); // save active process ID, used in last played application case
                    return ETrue; // found, stop searching
                    }
                }
            iter++;
            target = iter;
            }
        }
    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRemConTspController::FindActiveAudioFromCustomIfL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CRemConTspController::FindActiveAudioFromCustomIfL( TSglQue<TClientInfo>& aClients )
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::FindActiveAudioFromCustomIfL()" );
    
    CTspClientMapper* customIf = CTspClientMapper::NewLC();
    RArray<TProcessId> array;
    TInt err( customIf->GetTspTargetClients( CTspClientMapper::EPlayingClients, array ) );
    CleanupStack::PopAndDestroy( customIf );
    
    if( err != KErrNone )
        {
        COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindActiveAudioFromCustomIfL() error in GetTspClients %d", err );
        }
    else
        {
        TSglQueIter<TClientInfo> iter( aClients );
        TClientInfo* target = iter;
        
        while( target )
            {
            TProcessId processId = target->ProcessId();
            
            for( TInt i = 0; i < array.Count(); i++ ) // check if clients in tsp custom if
                {
                TProcessId processId1 = array[ i ];

                if( processId1 == processId )
                    {
                    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::FindActiveAudioFromCustomIfL() found process id %d", (TInt)processId1 );
                    aClients.Reset();
                    aClients.AddFirst( *target );
                    iProcessIdActive = target->ProcessId(); // save active process ID, used in last played application case
                    array.Close();
                    return ETrue; // found, stop searching
                    }
                }
            iter++;
            target = iter;
            }
        }
    array.Close();
    return EFalse;
    }

//----------------------------------------------------------------------------------
// CRemConTspController::GetLocalAddressedClient
// Uses TSP rules to determine what local addressed client should be.
//-----------------------------------------------------------------------------------
TClientInfo* CRemConTspController::GetLocalAddressedClient()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetLocalAddressedClient() Entry");
    // Create list of available clients to allow re-use of existing rule 
    // evaluation function, GetCorrectClientL
    iTargetsForAddressing.Reset();
    TSglQueIter<TClientInfo> iter(iAvailableTargets);
    TClientInfo* clientInfo;
    while((clientInfo = iter++) != NULL)
        {
        iTargetsForAddressing.AddLast(*clientInfo);
        }
    
#ifdef _DEBUG   
    TraceRemconTargets( iTargetsForAddressing );
#endif
    
    TRAPD(err, GetCorrectClientL(TUid::Uid(KRemConCoreApiUid), ERemConCoreApiPlay, iTargetsForAddressing, EFalse));
    if(iTargetsForAddressing.IsEmpty() || err)
        {
        clientInfo = NULL;
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConTspController::GetLocalAddressedClient() No target Found");
        }
    else
        {
        // We don't want to set the phone application or voice command handler as local addressed client. 
        // So choose the first item that is neither of those. The clientInfo may be NULL in the end. 
        TSglQueIter<TClientInfo> localAddressedIter(iTargetsForAddressing);
        while((clientInfo = localAddressedIter++) != NULL)
            {
            if( clientInfo->SecureId() != TSecureId(KUid3PhoneApp) || 
                clientInfo->SecureId() != TSecureId(KUid3VoiceCmdApp) )
                {
                COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConTspController::GetLocalAddressedClient() Local addressed client SID = %08x",  clientInfo->SecureId().iId);
                break;
                }
            }
        }

    return clientInfo;
    }

// End of file
