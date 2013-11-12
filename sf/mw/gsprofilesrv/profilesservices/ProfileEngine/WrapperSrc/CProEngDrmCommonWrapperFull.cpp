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
#include    <DRMCommon.h>

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
    iDrmCommon = DRMCommon::NewL();
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
    delete iDrmCommon;
    }

// -----------------------------------------------------------------------------
// CProEngDrmCommonWrapper::IsProtected
// -----------------------------------------------------------------------------
//
TInt CProEngDrmCommonWrapper::IsProtected( const TDesC& aFileName )
    {
    TBool result = EFalse;
    TRAP_IGNORE( result = IsProtectedL( aFileName ) );
    return result;
    }

// -----------------------------------------------------------------------------
// CProEngDrmCommonWrapper::IsProtectedL
// -----------------------------------------------------------------------------
//
TInt CProEngDrmCommonWrapper::IsProtectedL( const TDesC& aFileName )
    {
    // ignore error, default is "unprotected":
    TInt isProtected( EFalse );
   	ContentAccess::TVirtualPathPtr path( aFileName,
   						ContentAccess::KDefaultContentObject );
   	CData* data = CData::NewLC( path, EContentShareReadWrite );
   	TInt result = data->GetAttribute( EIsProtected, isProtected );
   	CleanupStack::PopAndDestroy(); // data

    return( ( result == DRMCommon::EOk ) && isProtected );
    }
    
//  End of File

