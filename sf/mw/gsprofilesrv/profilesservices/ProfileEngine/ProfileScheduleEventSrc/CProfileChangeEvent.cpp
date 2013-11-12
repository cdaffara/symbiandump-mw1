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
* Description:  Handles reactivation of the previous profile when the scheduler
*                runs.
*
*/


// INCLUDE FILES
#include    "CProfileChangeEvent.h"
#include    "ProfileEnginePrivateCRKeys.h"
#include    "ProfilesDebug.h"

#include    <e32svr.h>
#include    <MProfileEngine.h>
#include    <data_caging_path_literals.hrh>
#include    <centralrepository.h>

#include    <ProfileScheduleEvent.rsg>
#include    <ProfileEngineConstants.h>


// CONSTANTS

//namespace
//    {
//    _LIT( KProfileScheduleEventResourceDriveAndDir, "Z:ProfileScheduleEvent.rsc" );
//    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileChangeEvent::CProfileChangeEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileChangeEvent::CProfileChangeEvent()
    {
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    // Open/Create mutex
    TInt error( KErrNotFound );
    while( error == KErrNotFound )
        {
        error = iMutex.CreateGlobal( KProfileMutexName );
        if( error != KErrAlreadyExists )
            {
            break;
            }
        error = iMutex.OpenGlobal( KProfileMutexName );
        }
    User::LeaveIfError( error );
    iProfileEngine = CreateProfileEngineL( &iFs );
    ReadResourcesL();
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileChangeEvent* CProfileChangeEvent::NewL()
    {
    CProfileChangeEvent* self = CProfileChangeEvent::NewLC();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileChangeEvent* CProfileChangeEvent::NewLC()
    {
    CProfileChangeEvent* self = new( ELeave ) CProfileChangeEvent;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// Destructor
CProfileChangeEvent::~CProfileChangeEvent()
    {
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    iFs.Close();
    iMutex.Close();
    }


// -----------------------------------------------------------------------------
// CProfileChangeEvent::ExecuteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ExecuteL()
    {
	PRODEBUG( " CProfileChangeEvent:ExecuteL" );
    CProfileChangeEvent* profileChangeEvent = CProfileChangeEvent::NewLC();
	PRODEBUG( " CProfileChangeEvent:ExecuteL 2" );

    profileChangeEvent->DoProfileExpirationL();

    CleanupStack::PopAndDestroy(); // profileChangeEvent
	PRODEBUG( " CProfileChangeEvent:ExecuteL end" );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::DoProfileExpirationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::DoProfileExpirationL()
    {
    iMutex.Wait();
    TRAPD( err,
        if( ReadProfileIdL() )
            {
            ShowNoteL();
            ChangeProfileL();
            }
         );
    iMutex.Signal();
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ChangeProfileL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ChangeProfileL()
    {
	PRODEBUG( " CProfileChangeEvent:ChangeProfileL" );

    iProfileEngine->SetActiveProfileL( iPreviousId );

    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ReadProfileIdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileChangeEvent::ReadProfileIdL()
    {
	PRODEBUG( " CProfileChangeEvent:ReadProfileIdL" );
    CRepository* cenrep = CRepository::NewLC( KCRUidProfileEngine );
    User::LeaveIfError( cenrep->Get( KProEngPreviousActiveId, iPreviousId ) );
    CleanupStack::PopAndDestroy( cenrep );
    // If someone called SetActiveProfileL() right after scheduler started the
    // execution of this dll, the previous id would be resetted to KErrNotFound
    return ( iPreviousId > KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ShowNoteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ShowNoteL()
    {
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ReadResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ReadResourcesL()
    {
    }

//  End of File

