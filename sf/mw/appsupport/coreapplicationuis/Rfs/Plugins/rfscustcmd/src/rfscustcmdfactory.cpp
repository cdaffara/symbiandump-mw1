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
* Implementation of RfsCustCmdFactory class.
*
*/

#include "rfscustcmdfactory.h"
#include "rfscustcmd.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// RfsCustCmdFactory::RfsCustCmdFirstBootNewL
// ---------------------------------------------------------------------------
//
EXPORT_C MSsmCustomCommand* RfsCustCmdFactory::RfsCustCmdFirstBootNewL()
    {
    FUNC_LOG;

    return CRfsCustCmd::NewL( CRfsCustCmd::ERfsCustCmdFirstBoot );
    }


// ---------------------------------------------------------------------------
// RfsCustCmdFactory::RfsCustCmdDeepNewL
// ---------------------------------------------------------------------------
//
EXPORT_C MSsmCustomCommand* RfsCustCmdFactory::RfsCustCmdDeepNewL()
    {
    FUNC_LOG;

    return CRfsCustCmd::NewL( CRfsCustCmd::ERfsCustCmdDeep );
    }


// ---------------------------------------------------------------------------
// RfsCustCmdFactory::RfsCustCmdPreUiNewL
// ---------------------------------------------------------------------------
//
EXPORT_C MSsmCustomCommand* RfsCustCmdFactory::RfsCustCmdPreUiNewL()
    {
    FUNC_LOG;

    return CRfsCustCmd::NewL( CRfsCustCmd::ERfsCustCmdPreUi );
    }


// ---------------------------------------------------------------------------
// RfsCustCmdFactory::RfsCustCmdPostUiNewL
// ---------------------------------------------------------------------------
//
EXPORT_C MSsmCustomCommand* RfsCustCmdFactory::RfsCustCmdPostUiNewL()
    {
    FUNC_LOG;

    return CRfsCustCmd::NewL( CRfsCustCmd::ERfsCustCmdPostUi );
    }
