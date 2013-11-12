/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CGSAccProfileList class
*
*/


#include <gsaccessoryplugin.rsg>
#include <MProfileEngine.h>
#include <MProfileName.h>
#include <MProfilesNamesArray.h>
#include <StringLoader.h>

#include "gsaccprofilelist.h"
#include "trace.h"

// Use an id unlikely to be allocated to any real profile.
const TInt KActiveProfileId( 0 );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccProfileList::NewL
// ---------------------------------------------------------------------------
//
CGSAccProfileList* CGSAccProfileList::NewL()
    {
    CGSAccProfileList* self = new ( ELeave ) CGSAccProfileList;
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::~CGSAccProfileList
// ---------------------------------------------------------------------------
//
CGSAccProfileList::~CGSAccProfileList()
    {
    FUNC_LOG;

    delete iProfileNames;
    iProfileIds.Close();
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::NameListL
// ---------------------------------------------------------------------------
//
const MDesCArray& CGSAccProfileList::NameListL( CCoeEnv* aCoeEnv )
    {
    FUNC_LOG;

    LoadProfileListL( aCoeEnv );

    return *iProfileNames;
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::IdByIndexL
// ---------------------------------------------------------------------------
//
TInt CGSAccProfileList::IdByIndexL( const TInt aIndex, CCoeEnv* aCoeEnv )
    {
    FUNC_LOG;

    LoadProfileListL( aCoeEnv );

    return iProfileIds[ ValidateIndex( aIndex ) ];
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::IndexByIdL
// ---------------------------------------------------------------------------
//
TInt CGSAccProfileList::IndexByIdL( const TInt aProfileId, CCoeEnv* aCoeEnv )
    {
    FUNC_LOG;

    LoadProfileListL( aCoeEnv );

    TInt idx = ValidateIndex( iProfileIds.Find( aProfileId ) );
    INFO_2( "Index by id %d => %d", aProfileId, idx );
    return idx;
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::NameByIdL
// ---------------------------------------------------------------------------
//
TPtrC CGSAccProfileList::NameByIdL(
    const TInt aProfileId,
    CCoeEnv* aCoeEnv )
    {
    FUNC_LOG;

    LoadProfileListL( aCoeEnv );

    return ( *iProfileNames )[ ValidateIndex( iProfileIds.Find( aProfileId ) ) ];
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::CGSAccProfileList
// ---------------------------------------------------------------------------
//
CGSAccProfileList::CGSAccProfileList()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::LoadProfileListL
// ---------------------------------------------------------------------------
//
void CGSAccProfileList::LoadProfileListL( CCoeEnv* aCoeEnv )
    {
    FUNC_LOG;
    ASSERT_TRACE( aCoeEnv );

    if ( iProfileNames )
        {	
        delete iProfileNames;
        iProfileIds.Close();
        }  

    // Get list of profiles
    MProfileEngine* profileEngine =
        CreateProfileEngineL( &( aCoeEnv->FsSession() ) );
    CleanupReleasePushL( *profileEngine );
    MProfilesNamesArray* items = profileEngine->ProfilesNamesArrayLC();
    TInt count = items->MdcaCount();

    iProfileNames = new ( ELeave ) CDesCArrayFlat( count + 1 );
    iProfileIds.ReserveL( count + 1 );

    // Add active profile
    HBufC* activeProfile =
        StringLoader::LoadLC( R_ACC_ACTIVE_PROFILE, aCoeEnv );

    iProfileNames->AppendL( *activeProfile ); // First item
    iProfileIds.AppendL( KActiveProfileId );
    CleanupStack::PopAndDestroy( activeProfile );

    // Add other profiles
    for ( TInt i = 0; i < count; i++ )
        {
        TInt profileId = items->ProfileName( i )->Id();
        if ( profileId != EProfileOffLineId ) // Offline profile is not valid for accessories
            {
            iProfileNames->AppendL( items->MdcaPoint( i ) );
            // For some reason, profile IDs stored in CenRep are not the same ProfileEngine uses.
            iProfileIds.AppendL( profileId + 1 );
            }
        }

    CleanupStack::PopAndDestroy( 2 ); // items, profileEngine, pushed as cleanup items
    }


// ---------------------------------------------------------------------------
// CGSAccProfileList::ValidateIndex
// ---------------------------------------------------------------------------
//
TInt CGSAccProfileList::ValidateIndex( TInt aIndex ) const
    {
    ASSERT_TRACE( iProfileNames && iProfileNames->Count() > 0 );

    if ( aIndex >= 0 && aIndex < iProfileNames->Count() )
        {
        return aIndex;
        }

    return 0; // There is always at least one item in the array, the 'active profile'.
    }
