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
#include <eikstart.h>
#include "t_oomtestappDocument.h"
#include "t_oomtestappApplication.h"
#include "t_oomtestapp.hrh"

// ============================ MEMBER FUNCTIONS ===============================

Ct_oomtestappApplication::Ct_oomtestappApplication()
	{
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* Ct_oomtestappApplication::CreateDocumentL()
	{
	return Ct_oomtestappDocument::NewL(*this);
	}

Ct_oomtestappApplication::~Ct_oomtestappApplication()
	{
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid Ct_oomtestappApplication::AppDllUid() const
	{
	// Return the UID for the t_oomtestapp application
	return TUid::Uid( T_OOM_TESTAPP_UID );
	}

EXPORT_C CApaApplication* NewApplication()
    {
	return new Ct_oomtestappApplication;
    }

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

// End of File
