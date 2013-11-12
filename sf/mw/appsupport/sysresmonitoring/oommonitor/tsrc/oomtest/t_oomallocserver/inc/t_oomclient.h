/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/




#ifndef __T_CLIENT_H__
#define __T_CLIENT_H__

#include <e32std.h>

class ROOMAllocSession : public RSessionBase
	{
public:
	IMPORT_C TInt Connect();
	IMPORT_C TInt Reset();
	IMPORT_C TInt StartAllocating();
	IMPORT_C TInt StopAllocating();
	IMPORT_C TInt Configure(TUid aPlugin, TUint aAllocRate, TUint aAllocInitial, TUint aAllocLimit);
	IMPORT_C TInt MemoryLow(TUid aPlugin);
	IMPORT_C TInt MemoryGood(TUid aPlugin);
	};

#endif
