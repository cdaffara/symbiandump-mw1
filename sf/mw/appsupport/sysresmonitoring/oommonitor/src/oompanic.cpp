/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Panic codes for OOM monitor.
*
*/

#include "oompanic.h"

#include <e32std.h>

void OomMonitorPanic(TOomMonitorPanic aReason)
    {
    _LIT(KCat, "OomMonitor");
    User::Panic(KCat, aReason);
    }

void OomConfigParserPanic(TInt aReason)
    {
    _LIT(KParserCat, "OomParser");
    User::Panic(KParserCat, aReason);
    }
