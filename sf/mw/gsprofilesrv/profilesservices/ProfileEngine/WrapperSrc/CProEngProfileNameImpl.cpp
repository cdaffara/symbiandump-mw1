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
* Description:  Implementation of CProEngProfileNameImpl.
*
*/



// INCLUDE FILES
#include    "CProEngProfileNameImpl.h"
#include    <MProfileName.h>
#include    <MProEngProfileNameArray.h>
#include    <MProfileSetName.h>
#include    "CProEngEngineImpl.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngProfileNameImpl::CProEngProfileNameImpl
// -----------------------------------------------------------------------------
//
CProEngProfileNameImpl::CProEngProfileNameImpl(
        const MProfileName& aProfileName,
        MProfileSetName& aProfileSetName,
        CProEngEngineImpl& aEngine,
        TBool aModifiable )
    : iProfileName( aProfileName ), iProfileSetName( aProfileSetName ),
      iEngine( aEngine), iModifiable( aModifiable )
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngProfileNameImpl* CProEngProfileNameImpl::NewL(
        const MProfileName& aProfileName,
        MProfileSetName& aProfileSetName,
        CProEngEngineImpl& aEngine,
        TBool aModifiable )
    {
    return new ( ELeave ) CProEngProfileNameImpl(
            aProfileName, aProfileSetName, aEngine, aModifiable );
    }

// Destructor
CProEngProfileNameImpl::~CProEngProfileNameImpl()
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameImpl::Id
// -----------------------------------------------------------------------------
//
TInt CProEngProfileNameImpl::Id() const
    {
    return iProfileName.Id();
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameImpl::Name
// -----------------------------------------------------------------------------
//
const TDesC& CProEngProfileNameImpl::Name() const
    {
    return iProfileName.Name();
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameImpl::SetNameL
// -----------------------------------------------------------------------------
//
TInt CProEngProfileNameImpl::SetNameL( const TDesC& aName )
    {
    if( !iModifiable )
        {
        return KErrAccessDenied;
        }

    if( AlreadyExistsL( aName ) )
        {
        return KErrAlreadyExists;
        }

    iProfileSetName.SetNameL( aName );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameImpl::AlreadyExistsL
// -----------------------------------------------------------------------------
//
TBool CProEngProfileNameImpl::AlreadyExistsL( const TDesC& aName )
    {
    MProEngProfileNameArray* existingNames( iEngine.ProfileNameArrayLC() );
    TInt result( existingNames->FindByName( aName ) );
    TInt resId( existingNames->ProfileId( result ) );
    CleanupStack::PopAndDestroy(); // existingNames

    return ( ( result != KErrNotFound ) && ( resId != Id() ) );
    }

//  End of File

