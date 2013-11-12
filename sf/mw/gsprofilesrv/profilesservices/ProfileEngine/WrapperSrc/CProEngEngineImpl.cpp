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
* Description:  Implementation of CProEngEngineImpl.
*
*/



// INCLUDE FILES
#include    "CProEngEngineImpl.h"
#include    "CProEngProfileImpl.h"
#include    "CProEngToneHandler.h"
#include    "CProEngProfileNameArrayImpl.h"
#include    <f32file.h>
#include    <MProEngProfile.h>
#include    <MProEngProfileNameArray.h>
#include    <MProfileEngineExtended.h>
#include    <ProfileEngineConstants.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngEngineImpl::CProEngEngineImpl
// -----------------------------------------------------------------------------
//
CProEngEngineImpl::CProEngEngineImpl()
    {
    }


// -----------------------------------------------------------------------------
// CProEngEngineImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngEngineImpl::ConstructL()
    {
    iProfileEngine = CreateProfileEngineExtendedL();
    iToneHandler = CProEngToneHandler::NewL();
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngEngineImpl::ConstructL( RFs& aFs )
    {
    iProfileEngine = CreateProfileEngineExtendedL( &aFs );
    iToneHandler = CProEngToneHandler::NewL();
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngEngineImpl* CProEngEngineImpl::NewL()
    {
    CProEngEngineImpl* self = CProEngEngineImpl::NewLC();

    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngEngineImpl* CProEngEngineImpl::NewLC()
    {
    CProEngEngineImpl* self = new ( ELeave ) CProEngEngineImpl;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngEngineImpl* CProEngEngineImpl::NewL( RFs& aFs )
    {
    CProEngEngineImpl* self = CProEngEngineImpl::NewLC( aFs );

    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngEngineImpl* CProEngEngineImpl::NewLC( RFs& aFs )
    {
    CProEngEngineImpl* self = new ( ELeave ) CProEngEngineImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aFs );

    return self;
    }

// Destructor
CProEngEngineImpl::~CProEngEngineImpl()
    {
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    delete iToneHandler;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::Release
// -----------------------------------------------------------------------------
//
void CProEngEngineImpl::Release()
    {
    delete this;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ActiveProfileLC
// -----------------------------------------------------------------------------
//
MProEngProfile* CProEngEngineImpl::ActiveProfileLC()
    {
    return ProfileLC( ActiveProfileId() );
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ActiveProfileL
// -----------------------------------------------------------------------------
//
MProEngProfile* CProEngEngineImpl::ActiveProfileL()
    {
    MProEngProfile* profileWrapper = ActiveProfileLC();

    CleanupStack::Pop(); // profileWrapper
    
    return profileWrapper;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ActiveProfileId
// -----------------------------------------------------------------------------
//
TInt CProEngEngineImpl::ActiveProfileId()
    {
    return iProfileEngine->ActiveProfileId();
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ProfileNameArrayLC
// -----------------------------------------------------------------------------
//
MProEngProfileNameArray* CProEngEngineImpl::ProfileNameArrayLC()
    {
    MProfilesNamesArray* nameArray = iProfileEngine->ProfilesNamesArrayLC();

    CProEngProfileNameArrayImpl* nameArrayWrapper =
        CProEngProfileNameArrayImpl::NewL( nameArray ); // takes ownership

    CleanupStack::Pop();  // namearray
    CleanupStack::PushL( nameArrayWrapper );

    return nameArrayWrapper;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::SetActiveProfileL
// -----------------------------------------------------------------------------
//
void CProEngEngineImpl::SetActiveProfileL( TInt aId )
    {
    iProfileEngine->SetActiveProfileL( aId );
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ProfileLC
// -----------------------------------------------------------------------------
//
MProEngProfile* CProEngEngineImpl::ProfileLC( TInt aId )
    {
    return ProfilePrivateLC( aId );
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ProfileL
// -----------------------------------------------------------------------------
//
MProEngProfile* CProEngEngineImpl::ProfileL( TInt aId )
    {
    MProEngProfile* profileWrapper = ProfileLC( aId );

    CleanupStack::Pop();  // profileWrapper

    return profileWrapper;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ProfilePrivateLC
// -----------------------------------------------------------------------------
//
CProEngProfileImpl* CProEngEngineImpl::ProfilePrivateLC( TInt aId )
    {
    // This check is needed if the given id belongs to a variated profile which
    // is not supported by the platform (unfortunately the settings for this
    // kind of profile always exist in Central Repository):
    if( !IsDynamicProfileId( aId ) && !iProfileEngine->IsDefaultProfile( aId ) )
        {
        User::Leave( KErrNotFound );
        }

    MProfileExtended* profile = iProfileEngine->ProfileLC( aId );

    CProEngProfileImpl* profileWrapper =
        CProEngProfileImpl::NewL( *this, profile );

    CleanupStack::Pop(); // profile
    CleanupStack::PushL( profileWrapper );

    return profileWrapper;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ProfileEngineExtended
// -----------------------------------------------------------------------------
//
MProfileEngineExtended& CProEngEngineImpl::ProfileEngineExtended()
    {
    return *iProfileEngine;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::ToneHandler
// -----------------------------------------------------------------------------
//
CProEngToneHandler& CProEngEngineImpl::ToneHandler()
    {
    return *iToneHandler;
    }

// -----------------------------------------------------------------------------
// CProEngEngineImpl::IsDynamicProfileId
// -----------------------------------------------------------------------------
//
TBool CProEngEngineImpl::IsDynamicProfileId( TInt aId )
    {
    return ( ( aId >= KProfileBaseDynamicProfileId ) &&
             ( aId < ( KProfileBaseDynamicProfileId +
                       KProfilesMaxNumberOfDynamicProfiles ) ) );
    }

//  End of File


