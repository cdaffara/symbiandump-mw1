// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <fbs.h>

// User includes
#include "crectestengine.h"

// Lits
_LIT(KTxtFailed, "Failed to complete tests");

GLDEF_C void TestL()
	{
	// start FB server
	FbsStartup(); 
	RFbsSession::Connect();
	
	// Create the test engine
	CRecTestEngine* engine = CRecTestEngine::NewL();
	CleanupStack::PushL(engine);
	
	engine->StartTestsL();
	
	// Testing completed, destroy engine
	CleanupStack::PopAndDestroy(engine); 

	// disconnect from FB server
	RFbsSession::Disconnect();
	}

GLDEF_C TInt E32Main()
// Main function
	{
	__UHEAP_MARK;
	CTrapCleanup* tc=CTrapCleanup::New();
	TRAPD(err,TestL());
	if (err!=KErrNone)
		User::Panic(KTxtFailed, err);
	delete tc;
	REComSession::FinalClose();
	__UHEAP_MARKEND;
	return KErrNone;
	}
