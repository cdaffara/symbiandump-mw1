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
// The main startup of the AppArc server
// 
// apsmain.cpp
//

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APA_INTERNAL_H__)
#include "apainternal.h"
#endif
#else
#include "APASVST.H"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS

//
//	EXE entry point
//
TInt E32Main()
	{
	return ApaServThreadStart();
	}
	
