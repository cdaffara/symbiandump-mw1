/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApApp implementation.
*
*/


// INCLUDE FILES
#include "SysApApp.h"
#include "SysApDocument.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CSysApApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------------------------

TUid CSysApApp::AppDllUid() const
    {
    return KUidSysAp;
    }

// ---------------------------------------------------------------------------
// CSysApApp::CreateDocumentL()
// Creates CSysApDocument object
// ---------------------------------------------------------------------------

CApaDocument* CSysApApp::CreateDocumentL()
    {
    return new( ELeave ) CSysApDocument( *this );
    
    // return CSysApDocument::NewL(*this);
    }

// ===================== OTHER EXPORTED FUNCTIONS ============================

#include <eikstart.h>

LOCAL_C CApaApplication* NewApplication()
    {
    return new CSysApApp;
    }

GLDEF_C TInt E32Main()
    {
    // Change the priority to high during initial application loading.
    // It will be lowered back in app ui constructor.
    // If priority < high, app framework will lower it to background since
    // SysAp is started up as a background application.
    RThread().SetProcessPriority( EPriorityHigh );
    return EikStart::RunApplication(NewApplication);
    }


// End of File

