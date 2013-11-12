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
* Description:  Implementation of the CProfileEngineImpl.
*
*/



// INCLUDE FILES
#include "CProfileEngineImpl.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileEngineImpl::DeleteProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileEngineImpl::DeleteProfileL( TInt /*aId*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CreateProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileExtended* CProfileEngineImpl::CreateProfileL()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadNewProfileNameFromResourceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadNewProfileNameFromResourceL( TDes& /*aProfileName*/ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadDynamicProfilesNamesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadDynamicProfilesNamesL(
        CProfilesNamesArrayImpl* /* aProfilesNames */,
        CProfileNameImpl& /* aNameTemplate */ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadNewProfileNameFromResourceLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileNameImpl* CProfileEngineImpl::ReadNewProfileNameFromResourceLC()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetLocalizedNameForDynamicProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetLocalizedNameForDynamicProfileL(
        CProfileImpl& aProfile )
    {
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
