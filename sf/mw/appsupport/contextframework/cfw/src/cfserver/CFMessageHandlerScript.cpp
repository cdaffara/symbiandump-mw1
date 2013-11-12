/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Message handler context class implementation.
*
*/


#include <s32mem.h>

#include "CFMessageHandlerScript.h"
#include "cfcommon.h"
#include "CFScriptInterface.h"
#include "CFActionSubscriptionImpl.h"
#include "CFActionIndicationImpl.h"
#include "CFServSession.h"
#include "cftrace.h"

CCFMessageHandlerScript* CCFMessageHandlerScript::NewL(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFMessageHandlerScript* self = CCFMessageHandlerScript::NewLC(
        aCFContext,
        aCFAction,
        aScriptInterface );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFMessageHandlerScript* CCFMessageHandlerScript::NewLC(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFMessageHandlerScript* self = new( ELeave ) CCFMessageHandlerScript(
        aCFContext,
        aCFAction,
        aScriptInterface );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFMessageHandlerScript::~CCFMessageHandlerScript()
    {
    FUNC_LOG;
    
    iCFScriptInterface.DeregisterScriptOwner( *this );
    if( !iMessage.IsNull() )
        {
        // Complete message
        iMessage.Complete( KErrCancel );
        }
    iScriptIds.Close();
    }

CCFMessageHandlerScript::CCFMessageHandlerScript(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aCFScriptInterface ):
    CCFMessageHandlerBase( aCFContext, aCFAction, aCFScriptInterface )
    {
    FUNC_LOG;
    }
    
void CCFMessageHandlerScript::ConstructL()
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFMessageHandlerScript::HandleMessageL
//-----------------------------------------------------------------------------
//
TBool CCFMessageHandlerScript::HandleMessageL( const RMessage2& aMessage )
    {
    FUNC_LOG;

    TBool handled = ETrue;
    switch( aMessage.Function() )
        {
		// Slot[0] = RFs handle
		// Slot[1] = RFile handle
		case ERegisterScriptByName:
			{
            RFile scriptFile;
            User::LeaveIfError( scriptFile.AdoptFromClient( aMessage, 0, 1 ) );
            CleanupClosePushL( scriptFile );
            
            HBufC8* script = LoadScriptFromFile( scriptFile );
            TPtrC8 scriptPtrC( *script );
            CleanupStack::PushL( script );
            
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            HBufC* name = HBufC::NewLC( KMaxFileName );
            TPtr namePtr( name->Des() );
            User::LeaveIfError( scriptFile.Name( namePtr ) );
            
            // Register script
            TInt scriptId = 0;
            TInt err = RegisterScript( client, namePtr, scriptPtrC, scriptId );

            // Clean up
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( script );
            CleanupStack::PopAndDestroy( &scriptFile );
            
            if( err == KErrNone )
                {
                err = scriptId;
                }
            aMessage.Complete( err );
            break;
			}

        // Slot[0] = TDesC8 - Script
        // Slot[1] = TDesC  - Script name
    	case ERegisterScriptByNameAndDesc:
            {
            HBufC8* script = HBufC8::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr8 scriptPtr( script->Des() );
            aMessage.ReadL( 0, scriptPtr );
            
            HBufC* name = HBufC::NewLC( aMessage.GetDesLengthL( 1 ) );
            TPtr namePtr( name->Des() );
            aMessage.ReadL( 1, namePtr );
            
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );

            // Register script
            TInt scriptId = 0;
            TInt err = RegisterScript( client, namePtr, scriptPtr, scriptId );

            // Clean up
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( script );
            
            if( err == KErrNone )
                {
                err = scriptId;
                }
            aMessage.Complete( err );
            break;
            }

        // Slot[0] = TInt - Script ID
        case EDeregisterScriptById:
            {
			TInt scriptId = aMessage.Int0();            
            
            // Get handle to client process thread
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Deregister script
            TInt err = iCFScriptInterface.DeregisterScript( scriptId, client );

            // Cleanup
            CleanupStack::PopAndDestroy( &client );

            // Complete message and return
            aMessage.Complete( err );
            break;
            }
            
        // Slot[0] = RFs handle
        // Slot[1] = RFile handle
        case ESaveScriptByName:
            {
            RFile scriptFile;
            User::LeaveIfError( scriptFile.AdoptFromClient( aMessage, 0, 1 ) );
            CleanupClosePushL( scriptFile );
            
            HBufC8* script = LoadScriptFromFile( scriptFile );
            TPtrC8 scriptPtrC( *script );
            CleanupStack::PushL( script );
            
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            HBufC* name = HBufC::NewLC( KMaxFileName );
            TPtr namePtr( name->Des() );
            User::LeaveIfError( scriptFile.Name( namePtr ) );
            
            // Register script
            TInt scriptId = 0;
            TInt err = RegisterScript( client, namePtr, scriptPtrC, scriptId );
            if( err == KErrNone )
                {
                // Script registered - save script
                err = iCFScriptInterface.SaveScript(
                    scriptPtrC, scriptId, client );
                if( err != KErrNone )
                    {
                    // Script save failed - deregister script
                    iCFScriptInterface.DeregisterScript( scriptId, client );
                    }
                }

            // Clean up
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( script );
            CleanupStack::PopAndDestroy( &scriptFile );
            
            if( err == KErrNone )
                {
                err = scriptId;
                }
            aMessage.Complete( err );
            break;
            }
            
        // Slot[0] = TDesC8 - Script
        // Slot[1] = TDesC  - Script name
        case ESaveScriptByNameAndDesc:
            {
            HBufC8* script = HBufC8::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr8 scriptPtr( script->Des() );
            aMessage.ReadL( 0, scriptPtr );
            
            HBufC* name = HBufC::NewLC( aMessage.GetDesLengthL( 1 ) );
            TPtr namePtr( name->Des() );
            aMessage.ReadL( 1, namePtr );
            
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );

            // Register script
            TInt scriptId = 0;
            TInt err = RegisterScript( client, namePtr, scriptPtr, scriptId );
            if( err == KErrNone )
                {
                // Register ok - save script
                err = iCFScriptInterface.SaveScript(
                    scriptPtr, scriptId, client );
                if( err != KErrNone )
                    {
                    // Script save failed - deregister script
                    iCFScriptInterface.DeregisterScript( scriptId, client );
                    }
                }

            // Clean up
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( script );
            
            if( err == KErrNone )
                {
                err = scriptId;
                }
            aMessage.Complete( err );
            break;
            }

            // Slot[0] = TDesC - Script name
        case EDeleteScriptByName:
            {
            HBufC* scriptName = HBufC::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr scriptNamePtr( scriptName->Des() );
            aMessage.ReadL( 0, scriptNamePtr );
            
            // Get handle to client process thread
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Delete script by name
            TInt err = iCFScriptInterface.DeleteScript( scriptNamePtr, client );
            if( err == KErrNone )
                {
                // cleanup script persistent data
                iCFScriptInterface.CleanupPersistentDataByName(
                    scriptNamePtr, client );
                
                // Delete ok - deregister script
                err = iCFScriptInterface.DeregisterScript(
                    scriptNamePtr, client );
                }

            // Cleanup
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( scriptName );

            // Complete message and return
            aMessage.Complete( err == KErrDeregisterNotNeeded ? KErrNone : err );
            break;
            }

            // No params
        case EDeleteScriptByUid:
            {
            // Get handle to client process thread
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Delete scripts from the client
            TInt err = iCFScriptInterface.DeleteScripts( client );
            if( err == KErrNone )
                {
                // cleanup script persistent data
                iCFScriptInterface.CleanupPersistentDataByUid( client );

                // Delete ok - deregister scripts
                err = iCFScriptInterface.DeregisterScripts( client );
                }

            // Cleanup
            CleanupStack::PopAndDestroy( &client );

            // Complete message and return
            aMessage.Complete( err == KErrDeregisterNotNeeded ? KErrNone : err );
            break;
            }
            
            // Slot[0] = RFs handle
            // Slot[1] = RFile handle
        case EUpgradeRomScriptByName:
            {
            RFile scriptFile;
            User::LeaveIfError( scriptFile.AdoptFromClient( aMessage, 0, 1 ) );
            CleanupClosePushL( scriptFile );
            
            HBufC8* script = LoadScriptFromFile( scriptFile );
            TPtrC8 scriptPtrC( *script );
            CleanupStack::PushL( script );

            HBufC* name = HBufC::NewLC( KMaxFileName );
            TPtr namePtr( name->Des() );
            User::LeaveIfError( scriptFile.Name( namePtr ) );
            
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Check if client is allowed to upgrade
            TInt err = iCFScriptInterface.IsUpgradeAllowed(
                namePtr, *script, client );
            if( err == KErrNone )
                {
                // Upgrade script
                err = UpgradeScript( namePtr, scriptPtrC );
                }

            // Clean up
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( script );
            CleanupStack::PopAndDestroy( &scriptFile );
            
            aMessage.Complete( err );
            break;
            }
            
            // Slot[0] = TDesC8 - Script
            // Slot[1] = TDesC  - Script name
        case EUpgradeRomScriptByNameAndDesc:
            {
            HBufC8* script = HBufC8::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr8 scriptPtr( script->Des() );
            aMessage.ReadL( 0, scriptPtr );
            
            HBufC* name = HBufC::NewLC( aMessage.GetDesLengthL( 1 ) );
            TPtr namePtr( name->Des() );
            aMessage.ReadL( 1, namePtr );
            
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Check if client is allowed to upgrade
            TInt err = iCFScriptInterface.IsUpgradeAllowed(
                namePtr, *script, client );
            if( err == KErrNone )
                {
                // Upgrade script
                err = UpgradeScript( namePtr, scriptPtr );
                }

            // Clean up
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( script );
            
            aMessage.Complete( err );
            break;
            }
            
            // Slot[0] = TDesC - Script name
        case ERestoreRomScript:
            {
            // Read file name
            HBufC* scriptName = HBufC::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr scriptNamePtr( scriptName->Des() );
            aMessage.ReadL( 0, scriptNamePtr );
            
            RThread mainThread;
            CleanupClosePushL( mainThread );

            // Check if the script exists
            TInt err = KErrNotFound;
            TInt scriptId = 0;
            TBool exists = iCFScriptInterface.AlreadyExists(
                scriptNamePtr, mainThread, scriptId );
            if( exists )
                {
                // Get handle to the client thread
                RThread client;
                aMessage.ClientL( client );
                CleanupClosePushL( client );

                // Restore script - check that client is allowed to restore
                err = iCFScriptInterface.RestoreRomScript(
                    scriptId, client );
                if( err == KErrNone )
                    {
                    // Delete ram script
                    err = iCFScriptInterface.DeleteScript(
                        scriptNamePtr, mainThread );
                    }
                CleanupStack::PopAndDestroy( &client );
                }
            
            // Clean up
            CleanupStack::PopAndDestroy( &mainThread );
            CleanupStack::PopAndDestroy( scriptName );
            
            // Complete message
            aMessage.Complete( err );
            break;
            }
            
            // Slot[0] = TDes
            // Slot[1] = TInt
        case EScriptNotificationStart:
            {
            // Check if we already have message listening active
            // Client has messed up
            if( !iMessage.IsNull() )
                {
                // Panic client
                iMessage.Panic( KPanicCatClient,
                    EAlreadyReceivingScriptNotification );
                }
            else
                {
                // Store message and check if there are removed scripts notified
                iMessage = aMessage;
                NotifyRemovedScriptsL( iMessage );
                }            
            break;
            }
            
            // No args
        case EScriptNotificationCancel:
            {
            if( !iMessage.IsNull() )
                {
                // Complete message
                iMessage.Complete( KErrCancel );
                }
            aMessage.Complete( KErrNone );
            break;
            }
            
            // Slot[0] = TDes
        case EGetScriptNotification:
            {
            // Message will be completed during the function call
            NotifyRemovedScriptsL( aMessage );
            break;
            }
            
        default:
            {
            // Message not handled
            handled = EFalse;
            break;
            }
        }
    
    return handled;
    }

//------------------------------------------------------------------------------
// CCFMessageHandlerScript::HandleScriptsRemoved
//------------------------------------------------------------------------------
//
void CCFMessageHandlerScript::HandleScriptsRemoved(
    const RArray<TInt>& aScriptIds )
    {
    FUNC_LOG;
    
    // Add ids in to list
    for( TInt i = 0; i < aScriptIds.Count(); i++ )
        {
        // Ignore errors since this can occur only due to OOM
        iScriptIds.Append( aScriptIds[i] );
        }

    // Check if there is a message pending
    TRAP_IGNORE( NotifyRemovedScriptsL( iMessage ) );
    }

// -----------------------------------------------------------------------------
// CCFMessageHandlerScript::LoadScriptFromFile
// -----------------------------------------------------------------------------
//
HBufC8* CCFMessageHandlerScript::LoadScriptFromFile( RFile& aFile )
    {
    FUNC_LOG;
    
    HBufC8* script = NULL;
    TInt size = 0;
    TInt err = aFile.Size( size );
    if( err == KErrNone )
        {
        script = HBufC8::New( size );
        if( script )
            {
            TPtr8 scriptPtr = script->Des();
            err = aFile.Read( scriptPtr );
            if( err == KErrNone )
                {
                // Strip all unnecassary data from script
                TInt pos = scriptPtr.FindF( KScriptStartTag );
                if( pos != KErrNotFound )
                    {
                    scriptPtr.Copy( scriptPtr.MidTPtr( pos ) );
                    }
                else
                    {
                    // Incorrect script
                    delete script;
                    script = NULL;
                    }
                }
            else
                {
                delete script;
                script = NULL;
                }
            }
        }
    
    ERROR( err, "Failed loading script" );
    
    return script;
    }

// -----------------------------------------------------------------------------
// CCFMessageHandlerScript::RegisterScript
// -----------------------------------------------------------------------------
//
TInt CCFMessageHandlerScript::RegisterScript( const RThread& aClient,
    const TDesC& aName,
    const TDesC8& aScript,
    TInt& aScriptId )
    {
    FUNC_LOG;

    TInt err = KErrNone;

    // Check if the client already has a script with the same name
    TBool exists = iCFScriptInterface.AlreadyExists(
        aName, aClient, aScriptId );
    if( exists )
        {
        // Update the script
        err = iCFScriptInterface.UpdateScript(
            aScriptId, aClient, aScript, *this );
        }
    else
        {
        // Register a new script
        err = iCFScriptInterface.RegisterScript(
            aName, aScript, aScriptId, aClient, *this );    
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFMessageHandlerScript::UpgradeScript
// -----------------------------------------------------------------------------
//
TInt CCFMessageHandlerScript::UpgradeScript( const TDesC& aName,
    const TDesC8& aScript )
    {
    FUNC_LOG;

    RThread mainThread;
    TInt err = KErrNotFound;
    TInt scriptId = 0;
    TBool found = iCFScriptInterface.AlreadyExists(
        aName, mainThread, scriptId );
    if( found )
        {
        // Update script
        err = iCFScriptInterface.UpdateScript( scriptId, mainThread, aScript, *this );
        if( err == KErrNone )
            {
            // Script updated - save script
            err = iCFScriptInterface.SaveScript(
                aScript, scriptId, mainThread );
            }
        }
    
    // Clean up
    mainThread.Close();
    
    return err;
    }

//------------------------------------------------------------------------------
// CCFMessageHandlerScript::NotifyRemovedScriptsL
//------------------------------------------------------------------------------
//
void CCFMessageHandlerScript::NotifyRemovedScriptsL(
    const RMessage2& aMessage )
    {
    FUNC_LOG;
    
    // Double check that the message is not already completed and there are
    // script ids which have not yet been notified
    if( !aMessage.IsNull() && iScriptIds.Count() )
        {
        TInt err = KErrNone;
        
        // Check if the client side buffer is large enough
        TInt size = 0;
        
        // Count
        size += sizeof( TInt );
        // Ids
        for( TInt i = 0; i < iScriptIds.Count(); i++ )
            {
            size += sizeof( TInt );
            }
        // Client buffer length
        TInt clientBufLen = aMessage.GetDesMaxLengthL( 0 );
        if( clientBufLen >= size )
            {
            // Buffer large enough
            HBufC8* buf = HBufC8::NewLC( size );
            TPtr8 bufPtr( buf->Des() );
            RDesWriteStream write( bufPtr );
            write.PushL();
            
            // Write content in to the stream and then to client side buffer
            write.WriteInt16L( iScriptIds.Count() );
            for( TInt i = 0; i < iScriptIds.Count(); i++ )
                {
                write.WriteInt16L( iScriptIds[i] );
                }
            write.CommitL();
            aMessage.WriteL( 0, bufPtr );
            
            // Clean up
            CleanupStack::PopAndDestroy( &write );
            CleanupStack::PopAndDestroy( buf );
            
            // Reset script ids
            iScriptIds.Reset();
            }
        else
            {
            // Buffer too small
            err = KErrTooBig;
            TPckg<TInt> bufferSize( size );
            aMessage.WriteL( 1, bufferSize );
            }
        
        // Complete message
        aMessage.Complete( err );
        }
    }

// End of file
