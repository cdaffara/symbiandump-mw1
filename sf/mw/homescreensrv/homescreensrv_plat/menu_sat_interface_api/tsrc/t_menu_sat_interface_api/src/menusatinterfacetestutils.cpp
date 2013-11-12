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
* Description:
*
*/
#include <ecom/ecom.h>
#include "menusatinterfacetest.h"
#include "waitactive.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CClientServiceTester::CClientServiceTester
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//


// -----------------------------------------------------------------------------
// CClientServiceTester::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMenuSatInterfaceTest::CMenuSatInterfaceTest( )
    {
    RProperty::Get( KCRUidCa, KCaSatUIIconId, iIconId );
    }

    
// Destructor
CMenuSatInterfaceTest::~CMenuSatInterfaceTest()
    { 
    ///load preconditions
    RProperty::Set( KCRUidCa, KCaSatUIIconId, iIconId );
    //load preconditions
	WaitL(20000);
    }



void CMenuSatInterfaceTest::WaitL(TInt aMicroSec)
	{
	CWaitActive* wait = CWaitActive::NewL();
    wait->Wait(aMicroSec);
    delete wait;
	}

