/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApNspsHandler implementation.
*
*/


// INCLUDE FILES
#include "SysApNspsHandler.h"   // This
#include "SysApNspsAnim.h"
#include "SysAp.hrh"
#include <data_caging_path_literals.hrh> 

//CONSTANTS

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApNspsHandler::NewL( RWsSession& aWsSession, const RWindowBase& aDevice )
// ----------------------------------------------------------------------------

CSysApNspsHandler* CSysApNspsHandler::NewL( RWsSession& aWsSession, const RWindowBase& aDevice )
    {
    TRACES( RDebug::Print( _L( "CSysApNspsHandler::NewL" ) ) );
    CSysApNspsHandler* self = new( ELeave ) CSysApNspsHandler();
    CleanupStack::PushL( self );
    self->ConstructL( aWsSession, aDevice );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::ConstructL( RWsSession& aWsSession, const RWindowBase& aDevice ).
// ----------------------------------------------------------------------------

void CSysApNspsHandler::ConstructL( RWsSession& aWsSession, const RWindowBase& aDevice )
    {
    TRACES( RDebug::Print(_L("CSysApNspsHandler::ConstructL" ) ) );
    iAnimDll = new( ELeave ) RAnimDll( aWsSession );
#ifdef __WINS__
    _LIT( KNspsWsPluginDllFileName, "nspswsplugin.dll" );
    User::LeaveIfError( iAnimDll->Load( KNspsWsPluginDllFileName ) );
#else
    _LIT( KNspsWsPluginDllFileName, "z:nspswsplugin.dll" );
    TParse* fp = new (ELeave) TParse();
    CleanupStack::PushL(fp);
    fp->Set( KNspsWsPluginDllFileName, &KDC_SHARED_LIB_DIR, NULL );    
    TRACES( RDebug::Print(_L("CSysApNspsHandler::ConstructL: plugin: %S" ), &(fp->FullName())) );
    User::LeaveIfError( iAnimDll->Load( fp->FullName() ) );
    CleanupStack::PopAndDestroy(fp);
#endif
    iSysApAnim = new( ELeave ) RSysApNspsAnim( *iAnimDll );
    iSysApAnim->ConstructL( aDevice );
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::~CSysApNspsHandler()
// ----------------------------------------------------------------------------

CSysApNspsHandler::~CSysApNspsHandler()
    {
    TRACES( RDebug::Print(_L("CSysApNspsHandler::~CSysApNspsHandler") ) );
    if( iSysApAnim )
        {
        iSysApAnim->Close();
        delete iSysApAnim;
        }
    
    if( iAnimDll )
        {
        iAnimDll->Close();
        delete iAnimDll;
        }
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::CSysApNspsHandler()
// ----------------------------------------------------------------------------

CSysApNspsHandler::CSysApNspsHandler()
    {
    }


// ----------------------------------------------------------------------------
// CSysApNspsHandler::SetNspsOn()
// ----------------------------------------------------------------------------

void CSysApNspsHandler::SetNspsOn()
    {
    iSysApAnim->SetNspsOn();
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::SetNspsOff()
// ----------------------------------------------------------------------------

void CSysApNspsHandler::SetNspsOff()
    {
    iSysApAnim->SetNspsOff();
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::SetMessageTonePlaying()
// ----------------------------------------------------------------------------

void CSysApNspsHandler::SetMessageTonePlaying()
    {
    iSysApAnim->SetMessageTonePlaying();
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::SetMessageToneNotPlaying()
// ----------------------------------------------------------------------------

void CSysApNspsHandler::SetMessageToneNotPlaying()
    {
    iSysApAnim->SetMessageToneNotPlaying();
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::ActivateKeyeventForwardingForLights()
// ----------------------------------------------------------------------------

void CSysApNspsHandler::ActivateKeyeventForwardingForLights()
    {
    iSysApAnim->ActivateKeyeventForwardingForLights();
    }

// ----------------------------------------------------------------------------
// CSysApNspsHandler::DeActivateKeyeventForwardingForLights()
// ----------------------------------------------------------------------------

void CSysApNspsHandler::DeActivateKeyeventForwardingForLights()
    {
    iSysApAnim->DeActivateKeyeventForwardingForLights();
    }

//  End of File  
