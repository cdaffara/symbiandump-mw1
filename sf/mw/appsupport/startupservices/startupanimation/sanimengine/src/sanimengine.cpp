/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSAnimEngine class
*
*/


#include "sanimengine.h"
#include "sanimengineimpl.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimEngine::NewL
//
// ---------------------------------------------------------------------------
//
EXPORT_C CSAnimEngine* CSAnimEngine::NewL( RFs& aFs, MSAnimObserver& aObserver )
    {
    FUNC_LOG;

    return new ( ELeave ) CSAnimEngineImpl( aFs, aObserver );
    }


// ---------------------------------------------------------------------------
// CSAnimEngine::~CSAnimEngine
//
// ---------------------------------------------------------------------------
//
CSAnimEngine::~CSAnimEngine()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimEngine::CSAnimEngine
//
// ---------------------------------------------------------------------------
//
CSAnimEngine::CSAnimEngine()
    {
    FUNC_LOG;
    }
