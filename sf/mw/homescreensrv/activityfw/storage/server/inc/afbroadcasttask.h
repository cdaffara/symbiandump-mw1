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
#ifndef AFBROADCASTTASK_H
#define AFBROADCASTTASK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "aftaskstorage.h"
#include "aftask.h"

class AfBroadcastTask
{
public:
    static void ExecuteL(MAfTaskStorage& storage, const RMessage2& msg);
};

#endif // AFBROADCASTTASK_H
