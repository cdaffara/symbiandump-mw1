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
* Description: Implementation of the CProEngToneHandler.
*
*/



// INCLUDE FILES
#include    "CProEngToneHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngToneHandler::DrmConstructL
// 2nd phase construct for DRM objects. Empty implementation.
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::DrmConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::ReleaseDrm
// Destructor for DRM objects. Empty implementation.
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::ReleaseDrm()
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::CheckProtectedFileL
// Empty implementation.
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::CheckProtectedFileL( const TDesC& /*aFileName*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::CanSetAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
TBool CProEngToneHandler::CanSetAutomated( const TDesC& /*aFileName*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::AskAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::AskAutomated( const TDesC& /*aFileName*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::SetAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::SetAutomated( TProfileSettingId /* aSettingId */,
                                       const TDesC& /*aFileName*/ )
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::RemoveAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::RemoveAutomated( const TDesC& /*aFileName*/ )
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::IsProtected
// Empty implementation.
// -----------------------------------------------------------------------------
//
TBool CProEngToneHandler::IsProtected( const TDesC& /*aFileName*/ ) const
    {
    return EFalse;
    }

//  End of File
