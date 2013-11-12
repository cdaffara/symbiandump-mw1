/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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


// SYSTEM INCLUDES
#include <uiklaf/private/lafshut.h>
#include <w32std.h>

// USER INCLUDES
#include "outofdiskmonitor.h"
#include "OodTraces.h"

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C CLafShutdownManagerBase* LafShutdown::CreateShutdownManager(
                MShutdownEventObserver& aObserver)
    { // static
    TRACES("LafShutdown::CreateShutdownManager");
    CLafShutdownManagerBase* outOfDiskMonitor(NULL);
    TRAP_IGNORE( outOfDiskMonitor = COutOfDiskMonitor::NewL(aObserver) );
    return outOfDiskMonitor;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C MSaveObserver::TSaveType LafShutdown::SaveFromViewSwitch()
    { // static
    TRACES("LafShutdown::SaveFromViewSwitch");
    return MSaveObserver::ESaveNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C MSaveObserver::TSaveType LafShutdown::SaveFromAppSwitch()
    { // static
    TRACES("LafShutdown::SaveFromAppSwitch");
    return MSaveObserver::ESaveNone;
    }

// End of file.
