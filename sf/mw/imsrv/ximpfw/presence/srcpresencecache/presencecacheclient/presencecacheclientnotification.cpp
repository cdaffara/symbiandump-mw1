/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for presence cache client.
*
*/

#include <e32std.h>

#include <prescachereadhandler.h>

#include "presencecacheclientnotification.h"
#include "presencecachedefs.h"
#include "ximpobjecthelpers.h"
#include "presenceinfoimp.h"
#include "presencebuddyinfoimp.h"
#include "personpresenceinfoimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::NewL
// ---------------------------------------------------------------------------
//
CPresenceCacheClientNotification* CPresenceCacheClientNotification::NewL(
    CPresenceCacheClient& aClient)
    {
    CPresenceCacheClientNotification* self = new(ELeave)CPresenceCacheClientNotification(aClient);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;    
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::~CPresenceCacheClientNotification
// ---------------------------------------------------------------------------
//
CPresenceCacheClientNotification::~CPresenceCacheClientNotification()
    {
    Cancel();
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::CPresenceCacheClientNotification
// ---------------------------------------------------------------------------
//
CPresenceCacheClientNotification::CPresenceCacheClientNotification(CPresenceCacheClient& aClient)
: CActive( EPriorityStandard ), iClient( aClient ), iNotificationSubscribed( EFalse )
    {        
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::ConstructL
// ---------------------------------------------------------------------------
//
void CPresenceCacheClientNotification::ConstructL()
    {
    CActiveScheduler::Add( this );   
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::RunL
// ---------------------------------------------------------------------------
//
void CPresenceCacheClientNotification::RunL()
    {    
    TInt origStatus = iStatus.Int();
    
    MPresCacheReadHandler* client = ClientNotifyHandler();    
    
    if ( origStatus )
        {
        iNotificationSubscribed = EFalse;        
        TRAP_IGNORE( client->HandlePresenceNotificationL( origStatus, NULL ));
        }
    else
        {
        StartNotifySubscription();        
        PreseneceFromServerToClientL( *client, iSizePckg() );
        }              
    }
// ----------------------------------------------------------
// CPresenceCacheClientNotification::RunError
// ----------------------------------------------------------
//
TInt CPresenceCacheClientNotification::RunError( TInt aError )
    {         
    if ( KErrServerTerminated == aError )
        {
        // When server is terminated we notify the clienrt about that
        iNotificationSubscribed = EFalse;         
        MPresCacheReadHandler* client = ClientNotifyHandler();
        TRAP_IGNORE( client->HandlePresenceNotificationL( aError, NULL ));        
        }
    else
        {
        // StartNotifySubscription is already called.
        }
    return KErrNone;    
    } 

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::DoCancel
// ---------------------------------------------------------------------------
//
void CPresenceCacheClientNotification::DoCancel()
    {
    // TODO: consider when this is safe
    iClient.SendReceive( NRequest::ECancelWaitingForNotification );
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::SubscribePresenceBuddyChangeL
// ---------------------------------------------------------------------------
//    
TInt CPresenceCacheClientNotification::SubscribePresenceBuddyChangeL(
	const MXIMPIdentity& aIdentity)
    {       
    if ( !iNotificationSubscribed )
        {
        StartNotifySubscription();
        }
    
    HBufC8* myIdPack = iClient.PackIdentityLC(aIdentity);                
                      
    // Package message arguments before sending to the server
    TIpcArgs args;
    args.Set(1, myIdPack);  
        
    TInt err = iClient.SendReceive( NRequest::ESubscribeBuddyPresenceChange, args ); 
    CleanupStack::PopAndDestroy( myIdPack );    
        
    return err;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::UnsubscribePresenceBuddyChangeL
// ---------------------------------------------------------------------------
//    
void CPresenceCacheClientNotification::UnsubscribePresenceBuddyChangeL(
	const MXIMPIdentity& aIdentity)
    {           
    HBufC8* myIdPack = iClient.PackIdentityLC(aIdentity);                
                      
    // Package message arguments before sending to the server
    TIpcArgs args;
    args.Set(1, myIdPack);  
        
    TInt err = iClient.SendReceive( NRequest::EUnSubscribeBuddyPresenceChange, args );        
    CleanupStack::PopAndDestroy( myIdPack );
    User::LeaveIfError( err );
    }
 
// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::StartNotifySubscriptionL
// ---------------------------------------------------------------------------
//    
void CPresenceCacheClientNotification::StartNotifySubscription()
    {                                    
    // Package message arguments before sending to the server
    SetActive();
    TIpcArgs args(&iSizePckg);            
    iClient.SendReceive( NRequest::EWaitingForNotification, args, iStatus );
    iNotificationSubscribed = ETrue;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::ClientNotifyHandler
// ---------------------------------------------------------------------------
//    
MPresCacheReadHandler* CPresenceCacheClientNotification::ClientNotifyHandler()
    {
	return iClient.iNotifyClient;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClientNotification::PreseneceFromServerToClientL
// ---------------------------------------------------------------------------
//    
void CPresenceCacheClientNotification::PreseneceFromServerToClientL( 
    MPresCacheReadHandler& aClient, TInt aSize )
    {       
    CPresenceBuddyInfoImp* buddyInfo(NULL);
    buddyInfo = CPresenceBuddyInfoImp::NewLC();
    HBufC8* presInfoDes = HBufC8::NewLC( aSize );
    TPtr8 ptrBuf( presInfoDes->Des() );
    TInt err = iClient.SendReceive( NRequest::EGetLastNotifiedtPacket, TIpcArgs( &ptrBuf ));    
    if ( !err )
        {
        TXIMPObjectPacker< CPresenceBuddyInfoImp >::UnPackL( *buddyInfo, *presInfoDes );
        CleanupStack::PopAndDestroy( presInfoDes );                          
        aClient.HandlePresenceNotificationL( KErrNone, buddyInfo );
        CleanupStack::Pop( buddyInfo );         
        }
    else
        {
        CleanupStack::PopAndDestroy( presInfoDes );         
        CleanupStack::PopAndDestroy( buddyInfo );          
        }
    }

// end of file
