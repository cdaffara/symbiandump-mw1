/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The API supports requesting raw bitmaps
 *
*/


#ifndef __CAMENUICONUTILITY_H__
#define __CAMENUICONUTILITY_H__

#include "caclient_global.h"

class CaEntry;
class HbIcon;

class CACLIENT_EXPORT CaMenuIconUtility 
{
public:
    static HbIcon getApplicationIcon(int uid,
            const QSizeF &size = QSizeF(50,80));
    
    static HbIcon getEntryIcon(const CaEntry& entry,
            const QSizeF &size = QSizeF(50,80));
};

#endif // __CAMENUICONUTILITY_H__
