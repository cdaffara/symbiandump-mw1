/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Default key event handling of System Application
*
*/


#include <e32property.h>
#include <eikon.hrh>
#include <coemain.h>
//#include <aknkeylock.h>
//#include <AknTaskList.h>
#include "SysApTaskList.h"
#include <apgtask.h>
#include <apgcli.h>
#include <apgwgnam.h>
#include <apacmdln.h>
#include <AiwPoCParameters.h>
#include <AiwServiceHandler.h>
#include <featmgr.h>
#include <coreapplicationuisdomainpskeys.h>
#include <PSVariables.h>
#include <startupdomainpskeys.h>
#include <hwrmdomainpskeys.h>
#include <u32hal.h>
//#include <SysAp.rsg>
#include "sysapdefaultkeyhandler.h"
#include "sysapcallback.h"
#include "SysAp.hrh"
#include <AknSgcc.h>
//#include "sysapappui.h"
#include <w32std.h>


const TInt KModifierMask( 0 );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApDefaultKeyHandler::CSysApDefaultKeyHandler
// ---------------------------------------------------------------------------
//
CSysApDefaultKeyHandler::CSysApDefaultKeyHandler( MSysapCallback& aCallback ):                                                      
                                                     iCallback(aCallback),
                                                     iCameraSupported(EFalse),
                                                     iCoverDisplaySupported(EFalse),
//                                                     iKeylock(NULL),
                                                     iCapturedEKeyCamera(0),
                                                     iCapturedEKeyTwistOpen(0),
                                                     iCapturedEKeyTwistClose(0),
                                                     iCapturedEKeyGripOpen(0),
                                                     iCapturedEKeyGripClose(0),
                                                     iCapturedEKeyPoC(0)
//                                                     iAknUiServerConnected(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// CSysApDefaultKeyHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApDefaultKeyHandler::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::ConstructL()") ) );
    
    RWindowGroup groupWin = CCoeEnv::Static()->RootWin();
    User::LeaveIfError ( iCapturedEKeyCamera = groupWin.CaptureKey( EKeyCamera, KModifierMask, KModifierMask ) );
    
    FeatureManager::InitializeLibL();
    iCameraSupported = FeatureManager::FeatureSupported( KFeatureIdCamera );
    iCoverDisplaySupported = FeatureManager::FeatureSupported( KFeatureIdCoverDisplay );
    FeatureManager::UnInitializeLib();
 
    }

// ---------------------------------------------------------------------------
// CSysApDefaultKeyHandler::NewL
// ---------------------------------------------------------------------------
//
CSysApDefaultKeyHandler* CSysApDefaultKeyHandler::NewL(MSysapCallback& aCallback )
    {
    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::NewL()") ) );
    
    CSysApDefaultKeyHandler* self = new( ELeave ) CSysApDefaultKeyHandler(aCallback) ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSysApDefaultKeyHandler::~CSysApDefaultKeyHandler
// ---------------------------------------------------------------------------
//
CSysApDefaultKeyHandler::~CSysApDefaultKeyHandler()
    {
    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::~CSysApDefaultKeyHandler()") ) );
    RWindowGroup groupWin = CCoeEnv::Static()->RootWin();
    groupWin.CancelCaptureKey( iCapturedEKeyCamera );    
    }
    

// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::HandleKeyEventL()
// ----------------------------------------------------------------------------
//
TKeyResponse CSysApDefaultKeyHandler::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
{
    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::HandleKeyEventL: aKeyEvent.iCode=%d, aKeyEvent.iScanCode=%d, aType=%d"),
                                aKeyEvent.iCode, aKeyEvent.iScanCode, aType ) );
    
    TKeyResponse response = EKeyWasNotConsumed;
    
    __ASSERT_DEBUG( aType == EEventKey, User::Invariant() ); // other than event keys should not provided
        
    if( aType == EEventKey )
        {
        response = EKeyWasConsumed; // set again in default case if not consumed
        
        switch ( aKeyEvent.iCode )
            {                              
            case EKeyCamera:
                TRACES( RDebug::Print(_L("CSysApDefaultKeyHandler::HandleKeyEventL: EKeyCamera") ) );
                if ( iCameraSupported && !IsDeviceLocked() && !DoShowKeysLockedNote() )
                    {
                    ActivateApplicationL( KSysApCamcorderUid );
                    }
                break;
            default:
                response = EKeyWasNotConsumed;
                TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::CSysApDefaultKeyHandler: key was not consumed.") ) ); 
                break;               
            }
        }
    
    return response;
}  
    
  

#ifndef SYSAP_USE_STARTUP_UI_PHASE    
// -----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::IsStateNormal()
// -----------------------------------------------------------------------------
//
TBool CSysApDefaultKeyHandler::IsStateNormal() const
    {
    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::IsStateNormal")) );

    TInt state;
    TInt errorCode = RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state );
    
    if ( errorCode == KErrNone )
        {
        return state == ESwStateNormalRfOn ||
               state == ESwStateNormalRfOff ||
               state == ESwStateNormalBTSap;
        }
    else
        {
        TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::IsStateNormal: Failed to read global system state, error code %d." ), errorCode ) );
        return EFalse;
        }
    }
#endif // SYSAP_USE_STARTUP_UI_PHASE

// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::UiReady()
// 
// ----------------------------------------------------------------------------

TBool CSysApDefaultKeyHandler::UiReady() const
    {
#ifdef SYSAP_USE_STARTUP_UI_PHASE    
    TInt state( 0 );
    
    TInt err = RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state );
    
    if ( err != KErrNone || state == ESwStateEmergencyCallsOnly ) // emergency calls only state must block UI operations
        {
        TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::UiReady: KPSGlobalSystemState=%d, err=%d." ), state, err ) );
        return EFalse;
        }
    
    err = RProperty::Get( KPSUidStartup, KPSStartupUiPhase, state );
    
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::UiReady: Failed to read KPSStartupUiPhase, err=%d." ), err ) );
        return EFalse;
        }
    return state == EStartupUiPhaseAllDone;
#else // SYSAP_USE_STARTUP_UI_PHASE
    // if KPSSStartupUiPhase is not used, global system state normal is handled as UI idle state
    return IsStateNormal();
#endif // SYSAP_USE_STARTUP_UI_PHASE        
    }
    
// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::ActivateApplicationL()
// ----------------------------------------------------------------------------
//
void CSysApDefaultKeyHandler::ActivateApplicationL( const TUid aUid ) const
    {
    
    TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::ActivateApplicationL") ) );
    
    // Applications must not be activated before reaching normal system operation state
    if ( UiReady() )
        {
        CSysApTaskList* apaTaskList = CSysApTaskList::NewLC( CCoeEnv::Static()->WsSession() );
        TApaTask apaTask = apaTaskList->FindRootApp( aUid ); // only root application should not be activated
        CleanupStack::PopAndDestroy( apaTaskList );
                
        if ( apaTask.Exists() )
            {
            TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::ActivateApplicationL: application brought to foreground") ) );
#ifdef _DEBUG
            // camery latency measurement environment instrumentation, don't remove
            if ( aUid == KSysApCamcorderUid )
                {
                TRACES( RDebug::Print( _L("e_CAM_PRI_OFF_TO_ON 1") ) );
                }
#endif // _DEBUG                        
            apaTask.BringToForeground();
            }
        else
            {
            TInt err( KErrNone );
            TApaAppInfo appInfo;
            RApaLsSession apaLsSession;
            User::LeaveIfError( apaLsSession.Connect() );
            CleanupClosePushL( apaLsSession );
            err = apaLsSession.GetAppInfo( appInfo, aUid );
            if ( !err )
                {
                TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::ActivateApplicationL: starting application") ) );
                CApaCommandLine* apaCommandLine = CApaCommandLine::NewLC();
                apaCommandLine->SetExecutableNameL( appInfo.iFullName );
                apaCommandLine->SetCommandL( EApaCommandRun );
#ifdef _DEBUG
                // camery latency measurement environment instrumentation, don't remove
                if ( aUid == KSysApCamcorderUid )
                    {
                    TRACES( RDebug::Print( _L("e_CAM_PRI_OFF_TO_ON 1") ) );
                    }
#endif // _DEBUG                   
                apaLsSession.StartApp( *apaCommandLine );
                CleanupStack::PopAndDestroy( apaCommandLine );
                }
            else
                {
                TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::ActivateApplicationL err=%d"), err ) );
                }                
            // pop and destroy will also close this session
            CleanupStack::PopAndDestroy( &apaLsSession );
            }
        }
    }
    
// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::LaunchPocL()
// ----------------------------------------------------------------------------
//
void CSysApDefaultKeyHandler::LaunchPocL()
    {
    TRACES( RDebug::Print(_L("CSysApDefaultKeyHandler::LaunchPocL") ) );
    // not supported
  }
    
// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::IsDeviceLocked()
// ----------------------------------------------------------------------------
TBool CSysApDefaultKeyHandler::IsDeviceLocked() const    
    {
    TInt value(0);
    TBool retval(EFalse);

    RProperty::Get( KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, value );
    if ( value > EAutolockOff )
        {
        retval = ETrue;
        }
    
    TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::IsDeviceLocked()=%d" ), retval ) );
    
    return retval;
    }


// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::SetDisplayState
// ----------------------------------------------------------------------------
void CSysApDefaultKeyHandler::SetDisplayState( TBool aFlipOpen ) const
    {
    TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::SetDisplayState: aFlipOpen=%d" ), aFlipOpen ) );
    
    if ( iCoverDisplaySupported )
        {
        // Turn on/off the main display 
        TInt err = UserSvr::HalFunction(EHalGroupDisplay, EDisplayHalSetState, (TAny*)aFlipOpen, NULL);
    
        if ( err == KErrNone )
            {
            TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::SetDisplayState: Main display state set: %d" ), aFlipOpen ) );
            }
        else
            {
            TRACES( RDebug::Print( _L( "CSysApDefaultKeyHandler::SetDisplayState: Main display state set failed: %d." ), err ) );
            }    
        }

    }

// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::DoShowKeysLockedNote
// ----------------------------------------------------------------------------
//
TBool CSysApDefaultKeyHandler::DoShowKeysLockedNote()
    {
    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::DoShowKeysLockedNote: iAknUiServerConnected=%d"), iAknUiServerConnected ) );
    
//    TInt err(KErrNone);
    
    if ( !iAknUiServerConnected )
        {
/*        err = iAknUiServer.Connect();
        
        if ( err != KErrNone )
            {
 //           TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::DoShowKeysLockedNote: RAknUiServer::Connect failed, err=%d"), err ) );
            return ETrue; // assume that keypad/device is locked
            }
        iAknUiServerConnected = ETrue;            
  */      }
    
    TBool keysLocked = EFalse;
/*    err = iAknUiServer.ShowKeysLockedNote( keysLocked );
    
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::DoShowKeysLockedNote: RAknUiServer::ShowKeysLockedNote failed, err=%d"), err ) );
        keysLocked = ETrue; // assume that keypad/device is locked
        }
 */       
    return keysLocked;        
    }

// ----------------------------------------------------------------------------
// CSysApDefaultKeyHandler::IsDeviceModeKey
// ----------------------------------------------------------------------------
//
TBool CSysApDefaultKeyHandler::IsDeviceModeKey( const TKeyEvent& aKeyEvent )
    {
    TBool response( EFalse );
    
    switch ( aKeyEvent.iCode )
        {
        case EKeyGripOpen:
        case EKeyGripClose:
        case EKeyFlipOpen:
        case EKeyFlipClose:
        case EKeyTwistOpen:
        case EKeyTwistClose:
            response = ETrue;
            break;
            
        default:
            break; // not a device mode key            
        }

    TRACES( RDebug::Print( _L("CSysApDefaultKeyHandler::IsDeviceModeKey: aKeyEvent.iCode=%d, response=%d"), aKeyEvent.iCode, response ) );
    
    return response;        
    }



