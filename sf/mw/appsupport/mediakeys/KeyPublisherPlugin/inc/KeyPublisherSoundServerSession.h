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
*       Declaration of class CKeyPublisherSoundServerSession.
*       
*
*/

#ifndef __KEYPUBLISHERSOUNDSERVERSESSION_H__
#define __KEYPUBLISHERSOUNDSERVERSESSION_H__

#include <w32adll.h>
class RKeyPublisherSoundServerSession: public RSessionBase
	{
public:
	TInt Connect();
	void KeyPressed(TInt aKey, TBool aRepeat = EFalse);
	};

#endif //__KEYPUBLISHERSOUNDSERVERSESSION_H__
