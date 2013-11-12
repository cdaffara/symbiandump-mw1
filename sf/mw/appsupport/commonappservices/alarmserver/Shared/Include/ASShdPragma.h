// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __ASSHDPRAGMA_H__
#define __ASSHDPRAGMA_H__

// MSDEV "Build Window" friendly helper
#define _QUOTEME(x) # x
#define DO_QUOTE(x) _QUOTEME(x)
#define __MSDEV__ __FILE__ "(" DO_QUOTE(__LINE__) ") : "

#endif


