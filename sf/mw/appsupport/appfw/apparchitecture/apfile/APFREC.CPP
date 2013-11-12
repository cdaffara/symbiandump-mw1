// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apfrec.cpp
//

#include <apfrec.h>
#include <e32uid.h>
#include <f32file.h>
#include <apadef.h>
#include <ecom/ecom.h>

// CleanupEComArray function is used for cleanup support 
// of locally declared arrays.
void CleanUpEComInfoArray(TAny* aInfoArray)
	{
	RImplInfoPtrArray* infoArray = (static_cast<RImplInfoPtrArray*>(aInfoArray));
	infoArray->ResetAndDestroy();
	infoArray->Close();
	}

