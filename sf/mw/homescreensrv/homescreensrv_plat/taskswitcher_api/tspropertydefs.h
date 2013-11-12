/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef TSPROPERTYDEFS_H
#define TSPROPERTYDEFS_H

namespace TsProperty {

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
    const TUid KCategory = {0x20022fc5};
    const TUint KVisibilityKey = 0x2002677F;
    const TUint KDismissRequestKey = 0x20026781;
#endif    
    const char KTsPath[] = "/TaskSwitcher";
    const char KVisibilityPath[] = "Visibility";
    const char KDismissRequestPath[] = "DismissRequest";

}

#endif // TSPROPERTYDEFS_H
    