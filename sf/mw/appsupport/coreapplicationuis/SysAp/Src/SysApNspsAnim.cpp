/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  RSysApNspsAnim implementation.
*
*/


// INCLUDE FILES
#include "SysApNspsAnim.h"              // This
#include <coecntrl.h>                   // CCoeControl
#include "nspswsplugindllstd.h"         // Command

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// RSysApNspsAnim::RSysApNspsAnim( RAnimDll& aAnimDll )
// ----------------------------------------------------------------------------

RSysApNspsAnim::RSysApNspsAnim( RAnimDll& aAnimDll )
    :RAnim( aAnimDll )
	{
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::ConstructL( const RWindowBase& aDevice )
// ----------------------------------------------------------------------------

void RSysApNspsAnim::ConstructL( const RWindowBase& aDevice )
	{
	// Creates a server side anim class
	RAnim::Construct( aDevice, EForwardRawKeyeventsForNsps, TPtrC8() );
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::SetNspsOn()
// ----------------------------------------------------------------------------

void RSysApNspsAnim::SetNspsOn()
	{
	Command( EActivateNspsKeyeventForwarding );
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::SetNspsOff()
// ----------------------------------------------------------------------------

void RSysApNspsAnim::SetNspsOff()
	{
	Command( EDeActivateNspsKeyeventForwarding );
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::SetNspsOff()
// ----------------------------------------------------------------------------

void RSysApNspsAnim::SetMessageTonePlaying()
	{
	Command( EActivateKeyeventForwardingForNcn );
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::SetNspsOff()
// ----------------------------------------------------------------------------

void RSysApNspsAnim::SetMessageToneNotPlaying()
	{
	Command( EDeActivateKeyeventForwardingForNcn );
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::ActivateKeyeventForwardingForLights()
// ----------------------------------------------------------------------------

void RSysApNspsAnim::ActivateKeyeventForwardingForLights()
	{
	Command( EActivateKeyeventForwardingForLights );
	}

// ----------------------------------------------------------------------------
// RSysApNspsAnim::DeActivateKeyeventForwardingForLights()
// ----------------------------------------------------------------------------

void RSysApNspsAnim::DeActivateKeyeventForwardingForLights()
	{
	Command( EDeActivateKeyeventForwardingForLights );
	}

//  End of File  








