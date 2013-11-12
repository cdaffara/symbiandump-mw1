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
* Description:  Declarations of OOM Monitor.
*
*/


#ifndef OOMMONITOR_H
#define OOMMONITOR_H

#include <w32std.h>

/**
 * Creates thread for OOM watchers.
 * Called by the server in which the OOM Monitor runs.
 */
IMPORT_C void CreateOOMWatcherThreadL();

#endif // OOMMONITOR_H
