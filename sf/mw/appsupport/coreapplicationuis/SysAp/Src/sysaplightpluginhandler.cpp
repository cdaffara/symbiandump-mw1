/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  System Application light extension plugin handler
*
*/


#include <e32debug.h>
#include <sysaplightcontrolplugin.h>
#include "sysaplightpluginhandler.h"
#include "SysAp.hrh"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::CSysApLightPluginHandler
// ---------------------------------------------------------------------------
//
CSysApLightPluginHandler::CSysApLightPluginHandler() : iPlugin( NULL ),
                                                       iEventCompleted( ETrue )
    {
    }

// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApLightPluginHandler::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApLightPluginHandler::ConstructL") ) );
    
    TRAPD( err, iPlugin = CSysApLightControlPlugin::NewL() );
    
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print( _L("CSysApLightPluginHandler::ConstructL: loading plugin failed, err=%d"), err ) );
        iPlugin = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::CSysApLightPluginHandler
// ---------------------------------------------------------------------------
//
CSysApLightPluginHandler* CSysApLightPluginHandler::NewL()
    {
    TRACES( RDebug::Print( _L("CSysApLightPluginHandler::NewL") ) );
    CSysApLightPluginHandler* self = new (ELeave) CSysApLightPluginHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::~CSysApLightPluginHandler
// ---------------------------------------------------------------------------
//
CSysApLightPluginHandler::~CSysApLightPluginHandler()
    {
    TRACES( RDebug::Print( _L("CSysApLightPluginHandler::~CSysApLightPluginHandler") ) );
    delete iPlugin;
    }

// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::HandleEvent
// ---------------------------------------------------------------------------
//    
void CSysApLightPluginHandler::HandleEvent( SysApLightExtension::TLightEvent aLightEvent, const TDesC8& aData )
    {
    iEventCompleted = EFalse;
    
    if ( iPlugin )
        {
        TRACES( RDebug::Print( _L("CSysApLightPluginHandler::HandleEvent: aLightEvent=%d"), aLightEvent ) );

        TRAPD( err, iPlugin->LightEventL( aLightEvent, aData ) );
        
        if ( err != KErrNone )
            {
            TRACES( RDebug::Print( _L("CSysApLightPluginHandler::HandleEvent: err=%d"), err ) );
            }
        }
    }

// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::HandleEventNoAction
// ---------------------------------------------------------------------------
//    
void CSysApLightPluginHandler::HandleEventNoAction( SysApLightExtension::TLightEvent aLightEvent, const TDesC8& aData )
    {
    HandleEvent( aLightEvent, aData );
    HandleCommand( SysApLightExtension::ELightCommandNoAction );
    }


// ---------------------------------------------------------------------------
// CSysApLightPluginHandler::HandleCommand
// ---------------------------------------------------------------------------
//
TBool CSysApLightPluginHandler::HandleCommand( SysApLightExtension::TLightCommand aLightCommand, const TDesC8& aData )
    {
    if ( !iPlugin )
        {
        return EFalse; // light extension plugin not installed
        }
    
    iEventCompleted = ETrue;
    
    TRACES( RDebug::Print( _L("CSysApLightPluginHandler::HandleCommand: aLightCommand=%d"), aLightCommand ) );
        
    TInt response(SysApLightExtension::ECommandHandled);
    
    TRAPD( err, response = iPlugin->HandleLightCommandL( aLightCommand, aData ) );
    
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print( _L("CSysApLightPluginHandler::HandleLightCommand: err=%d"), err ) );
        response = SysApLightExtension::ECommandHandled; // don't apply default light control in case of a leave
        }
    
    TRACES( RDebug::Print( _L("CSysApLightPluginHandler::HandleLightCommand: response=%d"), response ) );
    
    return (response == SysApLightExtension::ECommandHandled ? ETrue : EFalse);
    }
