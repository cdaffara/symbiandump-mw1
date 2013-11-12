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
* Description:  Implementation of the ProfileEngPanic.
*
*/



// INCLUDE FILES
#include "ProfileEngPanic.h"

// CONSTANTS
_LIT( KProfileEngPanicText, "ProfileEng");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ProfileEngPanic::Panic
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void ProfileEngPanic::Panic( TInt aReason )
    {
    User::Panic( KProfileEngPanicText, aReason );
    }

//  End of File
