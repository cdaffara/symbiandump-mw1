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
* Description:  Entry point for RfsTestApp.
*
*/


#include <eikstart.h>

#include "rfstestapp.h"
#include "trace.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Create application object.
// ---------------------------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication()
    {
    FUNC_LOG;

    return new CRfsTestApp;
    }

// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Entry point for the application.
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    FUNC_LOG;

    return EikStart::RunApplication( NewApplication );
    }

