/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngDrmCommonWrapper.
*
*/



// INCLUDE FILES
#include    "CProEngDrmCommonWrapper.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngDrmCommonWrapper::CProEngDrmCommonWrapper
// -----------------------------------------------------------------------------
//
CProEngDrmCommonWrapper::CProEngDrmCommonWrapper()
    {
    }

// -----------------------------------------------------------------------------
// CProEngDrmCommonWrapper::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngDrmCommonWrapper::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CProEngDrmCommonWrapper::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngDrmCommonWrapper* CProEngDrmCommonWrapper::NewL()
    {
    CProEngDrmCommonWrapper* self =
        new ( ELeave ) CProEngDrmCommonWrapper();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CProEngDrmCommonWrapper::~CProEngDrmCommonWrapper()
    {
    }

// -----------------------------------------------------------------------------
// CProEngDrmCommonWrapper::IsProtectedFile
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TInt CProEngDrmCommonWrapper::IsProtected(
    const TDesC& /* aFileName */ )
    {
    return KErrNone;
    }
    
//  End of File




