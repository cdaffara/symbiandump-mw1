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

// INCLUDE FILES
#include "widgetscannerutils.h"
#include "cainnerentry.h"
#include "cadef.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void WidgetScannerUtils::CurrentMmcId( const RFs& aFs, TDes& aMmcId )
    {
    TInt mmcDrive;
    if ( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRemovableMassStorage,
            mmcDrive ) == KErrNone )
        {
        TVolumeInfo volumeInfo;
        if ( aFs.Volume( volumeInfo, mmcDrive ) == KErrNone )
            {
            aMmcId.Zero();
            aMmcId.AppendNum( volumeInfo.iUniqueID );
            }
        }
    }

//  End of File
