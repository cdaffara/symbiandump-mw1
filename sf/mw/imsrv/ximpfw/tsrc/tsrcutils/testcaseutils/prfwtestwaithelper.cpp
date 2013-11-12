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
* Description:  XIMP Framework TestGeneralWaiter 
*
*/



#include <e32std.h>

#include "prfwtestwaithelper.h"

// ======== LOCAL FUNCTIONS ========


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestWaitHelper* CXIMPTestWaitHelper::NewL()
    {
    CXIMPTestWaitHelper* self = new ( ELeave ) CXIMPTestWaitHelper;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CXIMPTestWaitHelper::ConstructL()
    {
    }
    
CXIMPTestWaitHelper::~CXIMPTestWaitHelper()
    {
    }
        
EXPORT_C void CXIMPTestWaitHelper::WaitForL( TInt aSeconds )
    {
    User::After( aSeconds * 1000000 );
    }
    
// End of file
