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
//

#include "PREVSTD.H"


GLDEF_C void Panic(TPrnPreviewPanic aPanic)
//
// Panic the process with PRINT as the category.
//
	{
	_LIT(KPrnPreviewPanic,"PRINT_PREVIEW");
	User::Panic(KPrnPreviewPanic,aPanic);
	}


