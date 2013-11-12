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
* Description:  Implementation of CProEngProfileNameArrayImpl.
*
*/



// INCLUDE FILES
#include    "CProEngProfileNameArrayImpl.h"
#include    <MProfilesNamesArray.h>
#include    <MProfileName.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::CProEngProfileNameArrayImpl
// -----------------------------------------------------------------------------
//
CProEngProfileNameArrayImpl::CProEngProfileNameArrayImpl(
        MProfilesNamesArray* aProfilesNamesArray )
        : iProfilesNamesArray( aProfilesNamesArray )
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngProfileNameArrayImpl* CProEngProfileNameArrayImpl::NewL(
        MProfilesNamesArray* aProfilesNamesArray )
    {
    return new ( ELeave ) CProEngProfileNameArrayImpl( aProfilesNamesArray );
    }

// Destructor
CProEngProfileNameArrayImpl::~CProEngProfileNameArrayImpl()
    {
    delete iProfilesNamesArray;
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::MdcaCount
// -----------------------------------------------------------------------------
//
TInt CProEngProfileNameArrayImpl::MdcaCount() const
    {
    return iProfilesNamesArray->MdcaCount();
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::MdcaPoint
// -----------------------------------------------------------------------------
//
TPtrC CProEngProfileNameArrayImpl::MdcaPoint( TInt aIndex ) const
    {
    const MProfileName* profileName =
        iProfilesNamesArray->ProfileName( aIndex );

    if( profileName )
        {
        return profileName->Name();
        }

    return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::ProfileId
// -----------------------------------------------------------------------------
//
TInt CProEngProfileNameArrayImpl::ProfileId( TInt aIndex ) const
    {
    const MProfileName* profileName =
        iProfilesNamesArray->ProfileName( aIndex );

    if( profileName )
        {
        return profileName->Id();
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::FindById
// -----------------------------------------------------------------------------
//
TInt CProEngProfileNameArrayImpl::FindById( TInt aId ) const
    {
    return iProfilesNamesArray->FindById( aId );
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayImpl::FindByName
// -----------------------------------------------------------------------------
//
TInt CProEngProfileNameArrayImpl::FindByName( const TDesC& aProfileName ) const
    {
    return iProfilesNamesArray->FindByName( aProfileName );
    }

//  End of File

