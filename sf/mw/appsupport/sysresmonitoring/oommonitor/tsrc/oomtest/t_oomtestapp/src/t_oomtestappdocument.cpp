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
* Description:  
*
*/




// INCLUDE FILES
#include "t_oomtestappAppUi.h"
#include "t_oomtestappDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ct_oomtestappDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomtestappDocument* Ct_oomtestappDocument::NewL(CEikApplication& aApp)
	{
	Ct_oomtestappDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomtestappDocument* Ct_oomtestappDocument::NewLC(CEikApplication& aApp)
	{
	Ct_oomtestappDocument* self = new (ELeave) Ct_oomtestappDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ct_oomtestappDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappDocument::Ct_oomtestappDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
Ct_oomtestappDocument::Ct_oomtestappDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// Ct_oomtestappDocument::~Ct_oomtestappDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
Ct_oomtestappDocument::~Ct_oomtestappDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// Ct_oomtestappDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* Ct_oomtestappDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) Ct_oomtestappAppUi;
	}

// End of File
