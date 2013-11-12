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
* Description:  CSysApKeyManagement implementation.
*
*/


// INCLUDE FILES

#include "sysapkeymanagement.h"
#include "SysAp.hrh"
#include "SysApAppUi.h"
#include <e32debug.h>
#include <ecom/implementationinformation.h> 
#include <sysapkeyhandlerplugininterface.h>
 
// CONSTANTS
const TInt KCurrentSupportedPluginVersion=2;
//text constant indicating that the plugin is non-resident, i.e. instantiated only when needed
_LIT8(KNonResidentPlugin,"NonResidentPlugin");

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
// CSysApPluginInfo::CSysApPluginInfo()
// -----------------------------------------------------------------------------
//
CSysApKeyManagement::CSysApPluginInfo::CSysApPluginInfo( TUid aPluginUid ) : iPluginUid(aPluginUid),
                                                                             iPtrPlugin(NULL),
                                                                             iIsKeyProvider(EFalse)
    {
    }

// ----------------------------------------------------------------------------
// CSysApPluginInfo::CSysApPluginInfo()
// -----------------------------------------------------------------------------
//
CSysApKeyManagement::CSysApPluginInfo::~CSysApPluginInfo()
    {
    delete iPtrPlugin;
    iKeys.Close();
    iKeyHandles.Close();
    iUpAndDownKeyHandles.Close();
    }    

// ----------------------------------------------------------------------------
// CSysApKeyManagement::CSysApKeyManagement()
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------

CSysApKeyManagement::CSysApKeyManagement( RWindowGroup& aRootWindowGroup, MSysapCallback& aSysApCallback ):
	iRootWindowGroup(aRootWindowGroup),
	iSysApCallback(aSysApCallback)
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::CSysApKeyManagement")) );	
    }

// ----------------------------------------------------------------------------
// CSysApKeyManagement::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//

void CSysApKeyManagement::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::ConstructL")) );	      
    GetPluginsL(); //gets info for all available plugins
    CaptureKeys(); //registers to capture all needed keys
    }


// ----------------------------------------------------------------------------
// CSysApKeyManagement::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//

CSysApKeyManagement* CSysApKeyManagement::NewL( RWindowGroup& aRootWindowGroup, MSysapCallback& aSysApCallback )
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::NewL" ) ) );
    CSysApKeyManagement* self = new ( ELeave ) CSysApKeyManagement( aRootWindowGroup, aSysApCallback );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self); //self
    return self;
    }


// ----------------------------------------------------------------------------
// CSysApKeyManagement::~CSysApKeyManagement()
// ----------------------------------------------------------------------------
//
CSysApKeyManagement::~CSysApKeyManagement()
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::~CSysApKeyManagement")) );	

    CancelCaptureKeys(); //first cancel the capturing of the keys

    iSysApPlugins.ResetAndDestroy();
    }


// ----------------------------------------------------------------------------
// CSysApKeyManagement::CancelCaptureKeys()
// ----------------------------------------------------------------------------
// 
void CSysApKeyManagement::CancelCaptureKeys()
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys")) );	

    // iterate through plugins and cancel key capture
    for ( TInt pluginCounter = iSysApPlugins.Count()-1; pluginCounter >= 0; pluginCounter-- )
        {
        CSysApPluginInfo* info = iSysApPlugins[pluginCounter];
        
        for ( TInt keyHandleCounter = info->iKeyHandles.Count()-1; keyHandleCounter >= 0; keyHandleCounter-- )
            {
            iRootWindowGroup.CancelCaptureKey( info->iKeyHandles[keyHandleCounter] );
            }
        
        for ( TInt upAndDownKeyHandleCounter = info->iUpAndDownKeyHandles.Count()-1; upAndDownKeyHandleCounter >= 0; upAndDownKeyHandleCounter-- )
            {
            iRootWindowGroup.CancelCaptureKeyUpAndDowns( info->iUpAndDownKeyHandles[upAndDownKeyHandleCounter] );
            }
        for ( TInt keyHandleCounter = info->iLongKeyPressHandle.Count()-1; keyHandleCounter >= 0; keyHandleCounter-- )
             {
             iRootWindowGroup.CancelCaptureLongKey( info->iLongKeyPressHandle[keyHandleCounter] );
             }
        }
    }


// ----------------------------------------------------------------------------
// CSysApKeyManagement::CaptureKeys()
// registers to window server for the keys we want. Callback is in SysappUi
// ----------------------------------------------------------------------------

void CSysApKeyManagement::CaptureKeys()
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys")) );	

    for ( TInt pluginCounter = iSysApPlugins.Count()-1; pluginCounter >= 0; pluginCounter-- )
        {
        CSysApPluginInfo* info = iSysApPlugins[pluginCounter];
        
        for ( TInt keyCounter = info->iKeys.Count()-1; keyCounter >= 0; keyCounter-- )
            {
            TUint captureKey = info->iKeys[keyCounter].iCode;

            // Special handling for flip key simulation
			if ( captureKey == EKeyFlipOpen || captureKey == EKeyFlipClose )
			    {
			    // don't capture flip, it is simulated to plugins from P&S
			    continue;
			    }
			
			TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys: capturing key 0x%x for plugin 0x%x"), 
                                       captureKey, info->iPluginUid ) );
			if (info->iIsKeyProvider)
			    {
			    TUint repeatValue = 0;
			    TInt captureUpDownKey = info->iKeys[keyCounter].iScanCode;
			    TUint modifiers = info->iKeys[keyCounter].iModifiers;
			    repeatValue = info->iKeys[keyCounter].iRepeats; 
			    
                TInt32 keyHandle = iRootWindowGroup.CaptureKey( captureKey, modifiers, modifiers );
                
                if ( keyHandle < KErrNone )
                    {
                    TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys: error %d"), keyHandle ) );
                    continue; // other keys may be possible to capture, so continue
                    }
                
                if ( ( keyHandle = info->iKeyHandles.Append( keyHandle ) ) != KErrNone ) //store handle for CancelCaptureKey
                    {
                    //error of Append ignored, key will not be cancelled. We can live with that.
                    TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys: Append failed, error %d"), keyHandle ) );
                    }
                
                TInt32 upAndDownKeyHandle = iRootWindowGroup.CaptureKeyUpAndDowns( captureUpDownKey, modifiers, modifiers );
                
                if ( upAndDownKeyHandle < KErrNone )
                    {
                    TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys UpAndDowns: error %d"), upAndDownKeyHandle ) );
                    continue; // other keys may be possible to capture, so continue
                    }
                
                if ( ( upAndDownKeyHandle = info->iUpAndDownKeyHandles.Append( upAndDownKeyHandle ) ) != KErrNone ) //store handle for CancelCaptureKeyUpAndDowns
                    {
                    //error of Append ignored, key will not be cancelled. We can live with that.
                    TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys UpAndDowns: Append failed, error %d"), upAndDownKeyHandle ) );
                    }
                if( repeatValue == 1 )
                    {
                    TInt32 longKeyPressHandle = iRootWindowGroup.CaptureLongKey( captureKey, captureKey,0, modifiers,0, ELongCaptureWaitShort);
                    if ( longKeyPressHandle < KErrNone )
                        {
                       TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureLongKey : error %d"), longKeyPressHandle ) );
                       continue; // other keys may be possible to capture, so continue
                        }
                    if ( ( longKeyPressHandle = info->iLongKeyPressHandle.Append( longKeyPressHandle ) ) != KErrNone ) //store handle for CancelCaptureKeyUpAndDowns
                         {
                          TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureLongKey : Append failed, error %d"), longKeyPressHandle ) );
                         }                               
                    }
			    }
			else
			    {
	            TInt32 handle = iRootWindowGroup.CaptureKey( captureKey, 0, 0 );
	            
	            if ( handle < KErrNone )
	                {
	                TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys: error %d"), handle ) );
	                continue; // other keys may be possible to capture, so continue
	                }
	            
	            if ( ( handle = info->iKeyHandles.Append( handle ) ) != KErrNone ) //store handle for CancelCaptureKey
	                {
	                //error of Append ignored, key will not be cancelled. We can live with that.
	                TRACES( RDebug::Print( _L("CSysApKeyManagement::CaptureKeys: Append failed, error %d"), handle ) );
	                }			    
			    }
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApKeyManagement::GetPluginsL()
// Gets plugins and extracts key and plugin information form them
// ----------------------------------------------------------------------------

void CSysApKeyManagement::GetPluginsL()
    {
    TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL")) );
    //lists all plugins implementing interface identified with SysapPluginUID

    RImplInfoPtrArray pluginInfoArray;

    CleanupClosePushL( pluginInfoArray );

    REComSession::ListImplementationsL( KSysapKeyHandlerPluginInterfaceUid, 
    									pluginInfoArray );

    TInt count = pluginInfoArray.Count();

    for ( TInt index = 0; index < count; ++index)
        {
    	//extract key from data of plugin
        CImplementationInformation* info = pluginInfoArray[index];
    	
        TInt version = info->Version();
    	
        if ( version > KCurrentSupportedPluginVersion )
            {
            TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: plugin 0x%x load failure, version conflict: version=%d"), info->ImplementationUid(), version ) );
            continue;	
            }
    	
        CSysApPluginInfo* pluginInfo = new (ELeave) CSysApPluginInfo( info->ImplementationUid() );
        CleanupStack::PushL( pluginInfo );
        
        if ( info->OpaqueData().Compare( KNonResidentPlugin ) == 0 ) //plugin tells itself in opaquedata that if it shall be resident
            {
            // do nothing for non-resident plugins
            TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: plugin 0x%x is non-resident."), pluginInfo->iPluginUid ) );
            }
        else
            {
            TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: loading resident plugin 0x%x"), pluginInfo->iPluginUid ) );
            pluginInfo->iPtrPlugin = CSysapKeyHandlerPlugin::NewL( pluginInfo->iPluginUid, &iSysApCallback );            
            if ( version > 1 )
                {
                //plugin is a key provider plugin (plugin version 2)
                pluginInfo->iIsKeyProvider = ETrue;
                }
            }
        
        TInt err = KErrNone;
    	if (pluginInfo->iIsKeyProvider)
    	    {
    	    TRAP(err, pluginInfo->iPtrPlugin->ProvideKeyEventsL(pluginInfo->iKeys));
    	    if (err == KErrNone)
    	        {
    	        err = CheckKeys(pluginInfo->iKeys);
    	        }    	    
    	    }
    	else
    	    {
            const TDesC8& data = info->DataType();  
            TLex8 lex(data); //convert data to TUint. We suppose that there is one TUint in the data
            err = GetKeys( lex, pluginInfo->iKeys );    	    
    	    }
        
        if ( err != KErrNone ) //plugin has no valid keys
    	    {
    	 	if( err == KErrAbort ) //this means that there was some error in the key definitions, which makes the plugin useless,
    	 	    {
    	 	  	TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: plugin 0x%x load failure, err=%d. Probably error in key definitons."), pluginInfo->iPluginUid, err  ) );
    	 	  	CleanupStack::PopAndDestroy( pluginInfo );
    	 	  	continue; //just ignore this plugin
    	 	  	}
            else 
                {
                TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: plugin 0x%x load failure, err=%d.Leaving..."), pluginInfo->iPluginUid, err  ) );
                User::Leave( err );
                }
            }
        
        //append to array of plugins
        err = iSysApPlugins.Append( pluginInfo );
                    		    		
        if ( err != KErrNone )	 //error in append
            {
            TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: resident plugin 0x%x load failure, err=%d"), pluginInfo->iPluginUid, err  ) );
            CleanupStack::PopAndDestroy( pluginInfo ); //this deletes also ptrPlugin
            break;	//probably no more plugins can be read
            }
    		
        TRACES( RDebug::Print( _L("CSysApKeyManagement::GetPluginsL: plugin 0x%x succesfully loaded."), pluginInfo->iPluginUid ) ); 	
            
        CleanupStack::Pop( pluginInfo ); 
    	}
    
    pluginInfoArray.Close();
    	
    CleanupStack::Pop( &pluginInfoArray ); 
    }

// ----------------------------------------------------------------------------
// CSysApKeyManagement::HandleKeyEventL()
// Handles key events. SysAppUi calls this function
// ----------------------------------------------------------------------------

TKeyResponse CSysApKeyManagement::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType )
    {
	TRACES( RDebug::Print( _L("CSysApKeyManagement::HandleKeyEventL")) );
	
    TKeyResponse response(EKeyWasNotConsumed); //not consumed by default
    
    for ( TInt pluginCounter = iSysApPlugins.Count()-1; pluginCounter >= 0; pluginCounter-- )
        {
        CSysApPluginInfo* info = iSysApPlugins[pluginCounter];
        
        for ( TInt keyCounter = info->iKeys.Count()-1; keyCounter >= 0; keyCounter-- )
            {
            if (aType == EEventKey)
                {
                if ( info->iKeys[keyCounter].iCode == aKeyEvent.iCode )
                    {
                    //  a plug-in, don't continue
                    TRACES( RDebug::Print( _L("CSysApKeyManagement::HandleKeyEventL: offering key=%d to plugin 0x%x"), 
                                                  aKeyEvent.iCode, info->iPluginUid ) );
                    
                    if ( info->iPtrPlugin )
                        {
                        response = info->iPtrPlugin->HandleKeyEventL( aKeyEvent, aType );
                        }
                    else // non-resident plug-in, needs to be loaded
                        {
                        TUid pluginuid(info->iPluginUid); //gets plugin info for this key
                        CSysapKeyHandlerPlugin* plugin = CSysapKeyHandlerPlugin::NewL( pluginuid, &iSysApCallback ); //instantiates plugin
                        CleanupStack::PushL( plugin );
                        response = plugin->HandleKeyEventL( aKeyEvent, aType );
                        CleanupStack::PopAndDestroy( plugin );
                        }

                    // only one plugin is allowed to capture a key event, so there is no sense to continue looping regardless
                    // whether key has been consumed or not
                    return response;                   
                    }
                }
            else if (info->iIsKeyProvider && (aType == EEventKeyUp || aType == EEventKeyDown))
                {
                if ( info->iKeys[keyCounter].iScanCode == aKeyEvent.iScanCode )
                    {
                    //  a plug-in, don't continue
                    TRACES( RDebug::Print( _L("CSysApKeyManagement::HandleKeyEventL: offering up or down key scancode =%d to plugin 0x%x"), 
                                                  aKeyEvent.iScanCode, info->iPluginUid ) );
                    //key provider plugins are always resident
                    return info->iPtrPlugin->HandleKeyEventL( aKeyEvent, aType );
                    }
                }
            else
                {
                continue;
                }
            }
        }
    return response;        
    }

// ----------------------------------------------------------------------------
// CSysApKeyManagement::FindKeyCode()
// Helper function for finding a key in plugins.
// ----------------------------------------------------------------------------

TInt CSysApKeyManagement::FindKeyCode( TUint aKeycode )
    {
    for ( TInt pluginCounter = iSysApPlugins.Count()-1; pluginCounter >= 0; pluginCounter-- )
        {
        CSysApPluginInfo* info = iSysApPlugins[pluginCounter];
        
        for ( TInt keyCounter = info->iKeys.Count()-1; keyCounter >= 0; keyCounter-- )
            {
            if ( info->iKeys[keyCounter].iCode == aKeycode )
				{
				return keyCounter;	
				}
            }
        }
    
    return KErrNotFound;
    }

// ----------------------------------------------------------------------------
// CSysApKeyManagement::FindScanCode()
// Helper function for finding a key scancode in plugins.
// ----------------------------------------------------------------------------

TInt CSysApKeyManagement::FindScanCode( TInt aScanCode )
    {
    for ( TInt pluginCounter = iSysApPlugins.Count()-1; pluginCounter >= 0; pluginCounter-- )
        {
        CSysApPluginInfo* info = iSysApPlugins[pluginCounter];
        
        for ( TInt keyCounter = info->iKeys.Count()-1; keyCounter >= 0; keyCounter-- )
            {
            if ( info->iKeys[keyCounter].iScanCode == aScanCode )
                {
                return keyCounter;  
                }
            }
        }
    
    return KErrNotFound;
    }


// ----------------------------------------------------------------------------
// CSysApKeyManagement::GetKeys()
// Helper function for getting the keys supported by plugins
// ----------------------------------------------------------------------------

TInt CSysApKeyManagement::GetKeys( TLex8& aLex, RArray<TKeyEvent>& aKeys )
	{
	aLex.SkipSpace();
	
	TUint thiskey;
	TInt err = aLex.Val( thiskey );
	
	if ( err != KErrNone ) //there must be at least one number
		{
		return KErrAbort;		
		}
	
	while ( err == KErrNone ) //while we have a new key
		{
		if ( FindKeyCode( thiskey ) == KErrNotFound ) // keys not to be duplicated 
			{
			TKeyEvent event;
			//other values in event not initialized, but
			//they are not used anyway
			event.iCode = thiskey;
			err = aKeys.Append( event ); //append to keys.
			
			if ( err != KErrNone )
				{
				return err;
				}
			}
		
		aLex.SkipSpace();
		
		err = aLex.Val( thiskey ); //get next new key
		}
		
	if ( aKeys.Count() == 0 ) //no valid keys		
		{
		return KErrAbort;		
		}
	
	return KErrNone;
	}

// ----------------------------------------------------------------------------
// CSysApKeyManagement::CheckKeys()
// Helper function for checking plugin keys
// ----------------------------------------------------------------------------

TInt CSysApKeyManagement::CheckKeys(RArray<TKeyEvent>& aKeys )
    {
    for ( TInt keyCounter = aKeys.Count()-1;  keyCounter >= 0; keyCounter-- )
        {
        if ( FindKeyCode(aKeys[keyCounter].iCode) != KErrNotFound )
            {
            aKeys.Remove(keyCounter);
            }
        }
    //check scancodes also just to be sure
    //no duplicates should be found is TKeyEvents are properly constructed
    for ( TInt keyCounter = aKeys.Count()-1;  keyCounter >= 0; keyCounter-- )
        {
        if ( FindScanCode(aKeys[keyCounter].iScanCode) != KErrNotFound )
            {
            aKeys.Remove(keyCounter);
            }
        }
    if ( aKeys.Count() )
        {
        return KErrNone;
        }
    return KErrAbort;
    }
	
//  End of File

