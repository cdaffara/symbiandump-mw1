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
 * Description:
 *
 */

#ifndef WIDGETSCANNERUTILS_H
#define WIDGETSCANNERUTILS_H

//  INCLUDES
#include <e32base.h>
#include <driveinfo.h>
#include "cawidgetscannerdef.h"

// FORWARD DECLARATIONS
class CCaInnerEntry;
/**
 *  Widget Scanner Utils
 *
 *  @since Series Series60 5.x
 */
class WidgetScannerUtils
    {
public:

    /**
     * Returns current MMC id.
     * @param aFs file server session reference.
     * @param aMmcId mmc id reference.
     */
    static void CurrentMmcId( const RFs& aFs, TDes& aMmcId );

    };

#endif      // WIDGETSCANNERUTILS_H
// End of File
