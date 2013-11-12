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
* Description: 
*       Animation plugin implementation file
*       
*
*/


#include <w32adll.h>
#include <eikkeysound.h>
#include "KeyPublisherSoundServerSession.h"

// -----------------------------------------------------------------------
// RKeyPublisherSoundServerSession::Connect
// Description: 
// -----------------------------------------------------------------------
//
TInt RKeyPublisherSoundServerSession::Connect()
	{
	// Create a session with zero message slots (since we have no asycronous calls)
	TInt ret=CreateSession(__KEYSOUND_SERVER_NAME,TVersion(KKeySoundServMajorVN,KKeySoundServMinorVN,KKeySoundServBuildVN),0);
	return ret;  
	}

// -----------------------------------------------------------------------
// RKeyPublisherSoundServerSession::KeyPressed
// Description: 
// -----------------------------------------------------------------------
//
void RKeyPublisherSoundServerSession::KeyPressed(TInt aKey, TBool aRepeat)
	{
	TIpcArgs args( aKey, aRepeat);	
	SendReceive(EKeySoundServerPlayKey,args);
	}

// End of File
