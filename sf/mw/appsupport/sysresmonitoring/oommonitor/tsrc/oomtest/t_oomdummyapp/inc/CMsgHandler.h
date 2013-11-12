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




#ifndef CMSGHANDLER_H
#define CMSGHANDLER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32msgqueue.h>
#include "t_oomdummyappAppUi.h"

class CMsgHandler : public CActive
	{
public:
	// Cancel and destroy
	~CMsgHandler();

	// Two-phased constructor.
	static CMsgHandler* NewL(Ct_oomdummyappAppUi& aOwner);

	// Two-phased constructor.
	static CMsgHandler* NewLC(Ct_oomdummyappAppUi& aOwner);

public:
	// New functions
	// Function for making the initial request
	void StartL();

private:
	// C++ constructor
	CMsgHandler(Ct_oomdummyappAppUi& aOwner);

	// Second-phase constructor
	void ConstructL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);

private:
	TInt iState; // State of the active object
	RMsgQueue<TInt> iMsgQueue; // messages from the harness
	Ct_oomdummyappAppUi& iOwner;

	};

#endif // CMSGHANDLER_H
