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
* Description:  Document class of the application.
*
*/


// INCLUDE FILES
#include <apgwgnam.h>

#include "StartupDocument.h"
#include "StartupAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CStartupDocument::~CStartupDocument()
// ---------------------------------------------------------
CStartupDocument::~CStartupDocument()
    {
    }

// ---------------------------------------------------------
// CStartupDocument::ConstructL()
// ---------------------------------------------------------
void CStartupDocument::ConstructL()
    {
    }

// ---------------------------------------------------------
// CStartupDocument::NewL()
// ---------------------------------------------------------
CStartupDocument* CStartupDocument::NewL(
        CEikApplication& aApp)     // CStartupApp reference
    {
    CStartupDocument* self = new (ELeave) CStartupDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }
    
// ----------------------------------------------------
// CStartupDocument::CreateAppUiL()
// ----------------------------------------------------
CEikAppUi* CStartupDocument::CreateAppUiL()
    {
    return new(ELeave) CStartupAppUi;
    }

// ----------------------------------------------------
// CStartupDocument::UpdateTaskNameL()
// Makes Startup-application hidden in menu shell and fastswap window
// ----------------------------------------------------
void CStartupDocument::UpdateTaskNameL( CApaWindowGroupName* aWgName )
    {
    CEikDocument::UpdateTaskNameL( aWgName );
    aWgName->SetHidden( ETrue );
    }

// End of file
