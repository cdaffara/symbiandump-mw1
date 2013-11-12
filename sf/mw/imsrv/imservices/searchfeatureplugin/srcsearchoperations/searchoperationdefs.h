/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XIMP framework SEARCH operation definitions.
 *
*/

#ifndef SEARCHOPERATIONDEFS_H
#define SEARCHOPERATIONDEFS_H

#include <e32std.h>



namespace NSearchOps
    {
    enum TSearchOpTypes
        {
        
        ESearch = 300,                          
        EHandleSearchResults,
        ESearchCleaner,
		EGetSearchKeys, 
		EHandleSearchKeys
        };
    }

#endif  //SEARCHOPERATIONDEFS_H





