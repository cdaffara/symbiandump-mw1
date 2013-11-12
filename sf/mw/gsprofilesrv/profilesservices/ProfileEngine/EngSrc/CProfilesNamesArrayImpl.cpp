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
* Description:  Implementation of the CProfilesNamesArrayImpl
*
*/



// INCLUDE FILES
#include "CProfilesNamesArrayImpl.h"
#include "CProfileNameImpl.h"
#include "ProfileEngPanic.h"
#include <collate.h>
#include <MProfilesNamesArray.h>
#include <ProfileEng.hrh>
#include <ProfileEngineConstants.h>

// LOCAL CONSTANTS AND MACROS
const TInt KProfileNamesGranularity( 5 );
const TInt KMaxCollationLevel( 3 );

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// SortOrder sort profile names.
// Returns: zero: if the two objects are equal
//          negative value: if the first object is less than the second.
//          positive value: if the first object is greater than the second.
// -----------------------------------------------------------------------------
//
TInt SortOrder(
    const CProfileNameImpl& aFirst,
    const CProfileNameImpl& aSecond )
    {
    TInt firstId( aFirst.Id() );
    TInt secondId( aSecond.Id() );
    if( firstId < KProfileBaseDynamicProfileId || 
        secondId < KProfileBaseDynamicProfileId ) 
        {
        return firstId - secondId;
        }
    else
        {
        TCollationMethod m = *Mem::CollationMethodByIndex( 0 ); // get the standard method
        m.iFlags |= TCollationMethod::EIgnoreNone;
        return aFirst.Name().CompareC( aSecond.Name(), KMaxCollationLevel, &m  );
        }
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::CProfilesNamesArrayImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfilesNamesArrayImpl::CProfilesNamesArrayImpl()
    : iProfilesNames( KProfileNamesGranularity ), iNullName( KNullDesC )
    {
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfilesNamesArrayImpl* CProfilesNamesArrayImpl::NewL()
    {
    return new( ELeave ) CProfilesNamesArrayImpl;
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfilesNamesArrayImpl* CProfilesNamesArrayImpl::NewLC()
    {
    CProfilesNamesArrayImpl* self = new( ELeave ) CProfilesNamesArrayImpl;

    CleanupStack::PushL( self );

    return self;
    }


// Destructor
CProfilesNamesArrayImpl::~CProfilesNamesArrayImpl()
    {
    iProfilesNames.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::InsertInOrder
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfilesNamesArrayImpl::InsertInOrder(
    const CProfileNameImpl* aProfileName )
    {
    __ASSERT_DEBUG( aProfileName, 
        ProfileEngPanic::Panic( EProfileEngPanicNullPointer ) );
    
    return iProfilesNames.InsertInOrder(
            aProfileName, TLinearOrder<CProfileNameImpl>( SortOrder ) );
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::Append
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfilesNamesArrayImpl::Append(
    const CProfileNameImpl* aProfileName )
    {
    __ASSERT_DEBUG( aProfileName,
        ProfileEngPanic::Panic( EProfileEngPanicNullPointer ) );

    return iProfilesNames.Append( aProfileName );
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::FindNameById
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfilesNamesArrayImpl::FindNameById(
    TInt aId )
    {
    const MProfileName* profileName = ProfileName( FindById( aId ) );
    if( profileName )
        {
        return profileName->Name();
        }
    return iNullName;
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::ProfileName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileName* CProfilesNamesArrayImpl::ProfileName(
    TInt aIndex ) const
    {
    TInt count( iProfilesNames.Count() );
    if( aIndex < 0 ||
        aIndex >= count )
        {
        return NULL;
        }
    return iProfilesNames[aIndex];
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::FindById
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfilesNamesArrayImpl::FindById(
    TInt aId ) const
    {
    TInt count( iProfilesNames.Count() );
    for( TInt i( 0 ) ; i < count ; ++i )
        {
        if( iProfilesNames[i]->Id() == aId )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::FindByName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfilesNamesArrayImpl::FindByName(
    const TDesC& aProfileName ) const
    {
    TInt count( iProfilesNames.Count() );
    for( TInt i( 0 ) ; i < count ; ++i )
        {
        if( aProfileName.Compare( iProfilesNames[i]->Name() ) == 0 )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::MdcaCount
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfilesNamesArrayImpl::MdcaCount() const
    {
    return iProfilesNames.Count();
    }

// -----------------------------------------------------------------------------
// CProfilesNamesArrayImpl::MdcaPoint
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CProfilesNamesArrayImpl::MdcaPoint(
    TInt aIndex ) const
    {
    TInt count( iProfilesNames.Count() );
    if( aIndex < 0 ||
        aIndex >= count )
        {
        return iNullName;
        }
    return iProfilesNames[aIndex]->Name();
    }

//  End of File
