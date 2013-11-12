/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  TUidOrientationPair class implementation.
 *
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "uidorientationpair.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
TUidOrientationPair::TUidOrientationPair( const TUid& aUid, TInt aOrientation ):
    iUid( aUid ),
    iOrientation( aOrientation )
    {
    FUNC_LOG;
    }

// End of file
