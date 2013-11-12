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
* Description:  CSysApKeySndHandler implementation.
*
*/


// INCLUDE FILES
#include "SysApKeySndHandler.h"   // This
#include "SysApKeySndAnim.h"
#include "SysAp.hrh"
#include <data_caging_path_literals.hrh> 
#include <coedef.h>

//CONSTANTS
_LIT( KKeyPublisherPluginDllFileName, "z:KeyPublisherPlugin.dll" );

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApKeySndHandler::NewL( RWsSession& aWsSession)
// ----------------------------------------------------------------------------

CSysApKeySndHandler* CSysApKeySndHandler::NewL( RWsSession& aWsSession )
    {
    TRACES( RDebug::Print( _L( "CSysApKeySndHandler::NewL" ) ) );
    CSysApKeySndHandler* self = new( ELeave ) CSysApKeySndHandler();
    CleanupStack::PushL( self );
    self->ConstructL( aWsSession );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApKeySndHandler::CSysApKeySndHandler()
// ----------------------------------------------------------------------------

CSysApKeySndHandler::CSysApKeySndHandler()
    {
    // Nothing to do
    }

// ----------------------------------------------------------------------------
// CSysApKeySndHandler::ConstructL( RWsSession& aWsSession ).
// ----------------------------------------------------------------------------

void CSysApKeySndHandler::ConstructL( RWsSession& aWsSession )
    {
    TRACES( RDebug::Print(_L("CSysApKeySndHandler::ConstructL" ) ) );

    //   Launching KeyPublisher Plugin

    iOffScreenGroupWin = RWindowGroup(aWsSession);
    User::LeaveIfError(iOffScreenGroupWin.Construct(
                                            (TUint32)&iOffScreenGroupWin, 
                                             EFalse)); 
                                             // EFalse disables key events
    iOffScreenGroupWin.SetOrdinalPosition(0,ECoeWinPriorityHigh);
    
    iAnimDll = new( ELeave ) RAnimDll( aWsSession );

    TParse* fp = new (ELeave) TParse();
    CleanupStack::PushL(fp);
    fp->Set( KKeyPublisherPluginDllFileName, &KDC_SHARED_LIB_DIR, NULL );    
    TRACES( RDebug::Print(_L("CSysApKeySndHandler::ConstructL: key publisher plugin: %S" ), &(fp->FullName())) );
    User::LeaveIfError( iAnimDll->Load( fp->FullName() ) );
    CleanupStack::PopAndDestroy(fp);
    TRACES( RDebug::Print( _L("CSysApKeySndHandler::ConstructL : key publisher plugin loaded" ) ) );

    iKeyPublisher = new (ELeave) RSysApKeySndAnim(*iAnimDll);
    iKeyPublisher->ConstructL(&iOffScreenGroupWin);     
    
    }

// ----------------------------------------------------------------------------
// CSysApKeySndHandler::~CSysApKeySndHandler()
// ----------------------------------------------------------------------------

CSysApKeySndHandler::~CSysApKeySndHandler()
    {
    TRACES( RDebug::Print(_L("CSysApKeySndHandler::~CSysApKeySndHandler") ) );
    if( iKeyPublisher )
        {
        iKeyPublisher->Close();
        delete iKeyPublisher;
        }
    
    if( iAnimDll )
        {
        iAnimDll->Close();
        delete iAnimDll;
        }
        
    iOffScreenGroupWin.Close();
    }


//  End of File  
