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
* Description:  Implementation of observer class for remote control keys
*
*/


// INCLUDE FILES
#include <remconcoreapitarget.h>
#include <remconinterfaceselector.h>
//#include <aknconsts.h>
#include "sysapremconobserver.h"
#include "SysApAppUi.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSysApRemConObserver::CSysApRemConObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSysApRemConObserver::CSysApRemConObserver( CSysApAppUi& aSysApAppUi ) 
 : iInterfaceSelector( NULL ),
   iCoreTarget( NULL ),
   iSysApAppUi( aSysApAppUi )
    {
    // Nothing
    }

// -----------------------------------------------------------------------------
// CSysApRemConObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSysApRemConObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApRemConObserver::ConstructL") ) );  
    
    iInterfaceSelector = CRemConInterfaceSelector::NewL();
    
    iCoreTarget = CRemConCoreApiTarget::NewL( *iInterfaceSelector, *this );
    
    iInterfaceSelector->OpenTargetL();
    }

// -----------------------------------------------------------------------------
// CSysApRemConObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSysApRemConObserver* CSysApRemConObserver::NewL( CSysApAppUi& aSysApAppUi )
    {   
    TRACES( RDebug::Print( _L("CSysApRemConObserver::NewL") ) );
    
    CSysApRemConObserver* self = new( ELeave ) CSysApRemConObserver( aSysApAppUi );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CSysApRemConObserver::~CSysApRemConObserver
// Destructor
// -----------------------------------------------------------------------------
//
CSysApRemConObserver::~CSysApRemConObserver()
    {
    TRACES( RDebug::Print( _L("CSysApRemConObserver::~CSysApRemConObserver") ) );
    delete iInterfaceSelector; // Internally deletes iCoreTarget 
    }
    
// -----------------------------------------------------------------------------
// CSysApRemConObserver::MrccatoCommand
// Handles remote key event
// -----------------------------------------------------------------------------
//
void CSysApRemConObserver::MrccatoCommand( TRemConCoreApiOperationId aOperationId, 
                                        TRemConCoreApiButtonAction aButtonAct )
	{
    TRACES( RDebug::Print( _L("CSysApRemConObserver::MrccatoCommand: aOperationId=%d, aButtonAct=%d"),
                           aOperationId, aButtonAct ) );
	if (  aButtonAct == ERemConCoreApiButtonPress ||
	      aButtonAct == ERemConCoreApiButtonClick )
	    {
	    if ( aOperationId == ERemConCoreApiVolumeUp 
	        || aOperationId == ERemConCoreApiVolumeDown
	        || aOperationId == ERemConCoreApiMute )
	        {
            // A volume key is pressed down
            TRAPD( err, iSysApAppUi.ShowUiNoteL( EFmTxVolumeDisabledNote ) );
            
            if ( err != KErrNone )
                {
                TRACES( RDebug::Print( _L("CSysApRemConObserver::MrccatoCommand: err=%d"), err ) );
                }
	        }
	    }
	}

//  End of File 
