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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/


#include "oomaction.h"
#include "oomactionlist.h"
#include "OomTraces.h"

COomAction::~COomAction()
    {
    FUNC_LOG;
    }

// To be called by the derived class after the memory has been freed (or if it fails)
void COomAction::MemoryFreed(TInt)
    {
    FUNC_LOG;
    
    iState = EOomIdle;
    iStateChangeObserver.StateChanged();
    }

COomAction::COomAction(MOomActionObserver& aStateChangeObserver) : iStateChangeObserver(aStateChangeObserver)
    {
    FUNC_LOG;
    }
