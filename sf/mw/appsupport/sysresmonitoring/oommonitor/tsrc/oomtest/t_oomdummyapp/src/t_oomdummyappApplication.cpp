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
#include "t_oomdummyapp.hrh"
#include "t_oomdummyappDocument.h"
#include "t_oomdummyappApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

Ct_oomdummyappApplication::Ct_oomdummyappApplication(TUid aUid, TUint aAlloc) : iUID(aUid), iAlloc(aAlloc)
	{
	
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* Ct_oomdummyappApplication::CreateDocumentL()
	{
	if(iAlloc > 0) iMemory = User::AllocL(iAlloc);
	// Create an t_oomdummyapp document, and return a pointer to it
	return Ct_oomdummyappDocument::NewL(*this);
	}

Ct_oomdummyappApplication::~Ct_oomdummyappApplication()
	{
	delete iMemory;
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid Ct_oomdummyappApplication::AppDllUid() const
	{
	// Return the UID for the t_oomdummyapp application
	return iUID;
	}

// End of File
