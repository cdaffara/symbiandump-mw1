/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApAnimKeySndControl implementation.
*
*/


// INCLUDE FILES
#include "SysApAnimKeySndControl.h"   // This


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApAnimKeySndControl::NewL( RWsSession& aWsSession)
// ----------------------------------------------------------------------------

CSysApAnimKeySndControl* CSysApAnimKeySndControl::NewL( RWindowGroup* aParent )
    {
    TRACES( RDebug::Print( _L( "CSysApAnimKeySndControl::NewL" ) ) );
    CSysApAnimKeySndControl* self = new( ELeave ) CSysApAnimKeySndControl();
    CleanupStack::PushL( self );
    self->ConstructL( aParent );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApAnimKeySndControl::CSysApAnimKeySndControl()
// ----------------------------------------------------------------------------
CSysApAnimKeySndControl::CSysApAnimKeySndControl()
	{
	// Nothing to do
	}

// ----------------------------------------------------------------------------
// CSysApAnimKeySndControl::ConstructL( RWsSession& aWsSession ).
// ----------------------------------------------------------------------------

void CSysApAnimKeySndControl::ConstructL( RWindowGroup* aParent )
    {
	CreateWindowL(aParent);
	SetExtent(TPoint(0,0),TSize(0,0));
	Window().SetShadowDisabled(ETrue);
	Window().Activate();
    }

// ----------------------------------------------------------------------------
// CSysApAnimKeySndControl::~CSysApAnimKeySndControl()
// ----------------------------------------------------------------------------

CSysApAnimKeySndControl::~CSysApAnimKeySndControl()
    {
    // Nothing to do?
    }


//  End of File  
