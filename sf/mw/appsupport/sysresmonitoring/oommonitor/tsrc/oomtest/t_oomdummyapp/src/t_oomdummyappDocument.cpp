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
#include "t_oomdummyappAppUi.h"
#include "t_oomdummyappDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ct_oomdummyappDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappDocument* Ct_oomdummyappDocument::NewL(CEikApplication& aApp)
	{
	Ct_oomdummyappDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappDocument* Ct_oomdummyappDocument::NewLC(CEikApplication& aApp)
	{
	Ct_oomdummyappDocument* self = new (ELeave) Ct_oomdummyappDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappDocument::Ct_oomdummyappDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappDocument::Ct_oomdummyappDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// Ct_oomdummyappDocument::~Ct_oomdummyappDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
Ct_oomdummyappDocument::~Ct_oomdummyappDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// Ct_oomdummyappDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* Ct_oomdummyappDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) Ct_oomdummyappAppUi;
	}

// End of File
