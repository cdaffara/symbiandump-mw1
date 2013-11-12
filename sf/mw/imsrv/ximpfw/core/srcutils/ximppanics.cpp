/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XIMP Framework panic implementation
*
*/

// INCLUDE FILES
#include "ximppanics.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// NXIMPPrivPanic::Panic()
// -----------------------------------------------------------------------------
//
EXPORT_C void NXIMPPrivPanic::Panic( TReason aReason )
    {
    #ifdef __WINS__
    // User::SetJustInTime( ETrue );
    #endif // __WINS__
    
    User::Panic( NXIMPPrivPanic::KCategory, aReason );
    }

   

