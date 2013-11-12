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
* Description:  RSysApKeySndAnim implementation.
*
*/


// INCLUDE FILES
#include "SysApKeySndAnim.h"   // This
#include "SysApAnimKeySndControl.h"                   
#include <keypublisherdll.h>


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// RSysApKeySndAnim::RSysApKeySndAnim( RAnimDll& aAnimDll )
// ----------------------------------------------------------------------------

RSysApKeySndAnim::RSysApKeySndAnim( RAnimDll& aAnimDll )
    :RAnim( aAnimDll )
	{
	}

// ----------------------------------------------------------------------------
// RSysApKeySndAnim::ConstructL( const RWindowBase& aDevice )
// ----------------------------------------------------------------------------

void RSysApKeySndAnim::ConstructL( RWindowGroup* aParent )
	{
	iKeySoundControl = CSysApAnimKeySndControl::NewL(aParent);
	RAnim::Construct(*(iKeySoundControl->DrawableWindow()), 
	                   ESVKAnimKeySound, TPtrC8());
	}

// ----------------------------------------------------------------------------
// RSysApKeySndAnim::Close()
// ----------------------------------------------------------------------------
void RSysApKeySndAnim::Close()
	{
	RAnim::Close();
	delete iKeySoundControl;
	iKeySoundControl = NULL;
	}

//  End of File  








