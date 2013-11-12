/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CAknAnimDll method definitions
*
*/


// INCLUDE FILES
#include "NspsWsPluginDllStd.h"
#include "NspsWsPluginDll.h"       // this
#include "NspsWsPlugin.h"     // functionality

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknAnimDll::CreateInstanceL(TInt aType)
// -----------------------------------------------------------------------------

CAnim* CAknAnimDll::CreateInstanceL(TInt aType)
    {
    switch (aType)
        {
        case EForwardRawKeyeventsForNsps:
            return new(ELeave)CNspsWsPlugin();
            // no break as there is return
        default:
            return NULL;
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CreateCAnimDllL()
// -----------------------------------------------------------------------------

EXPORT_C CAnimDll* CreateCAnimDllL()
    {
    return new(ELeave)CAknAnimDll();
    }


// End of File
