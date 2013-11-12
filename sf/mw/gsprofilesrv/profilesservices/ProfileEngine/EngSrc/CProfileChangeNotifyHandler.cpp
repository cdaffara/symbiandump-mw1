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
* Description:  Implementation of the CProfileChangeNotifyHandler
*
*/



// INCLUDE FILES
#include "ProfileEngPanic.h"
#include <CProfileChangeNotifyHandler.h>
#include <MProfileChangeObserver.h>
#include <ProfileEngineSDKCRKeys.h>
#include <centralrepository.h>
#include <ProfileEnginePrivatePSKeys.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::CProfileChangeNotifyHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileChangeNotifyHandler::CProfileChangeNotifyHandler(
    MProfileChangeObserver* aProfileChangeObserver )
    : iProfileChangeObserver( aProfileChangeObserver )
    {

    }

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileChangeNotifyHandler::ConstructL()
    {
    iPSObserver = new ( ELeave ) CProfilePSObserver(
            KProEngActiveProfileModified );
    iActiveProfileObserver = new ( ELeave ) CProfilePSObserver(
            KProEngActiveProfileChanged );
    iPSObserver->iProfileChangeObserver = iProfileChangeObserver;
    iActiveProfileObserver->iProfileChangeObserver = iProfileChangeObserver;
    iRepository = CRepository::NewL( KCRUidProfileEngine );
    iActiveProfileObserver->iCenRep = iRepository;
    iPSObserver->iCenRep = iRepository;
    iActiveProfileObserver->RequestNotificationL(); 
    iPSObserver->RequestNotificationL(); 
    }


// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileChangeNotifyHandler* CProfileChangeNotifyHandler::NewL(
    MProfileChangeObserver* aProfileChangeObserver )
    {
    __ASSERT_ALWAYS( aProfileChangeObserver,
        ProfileEngPanic::Panic( EProfileEngPanicNullPointer ) );

    CProfileChangeNotifyHandler* self =
        new ( ELeave ) CProfileChangeNotifyHandler(aProfileChangeObserver);

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// Destructor
CProfileChangeNotifyHandler::~CProfileChangeNotifyHandler()
    {
    delete iActiveProfileObserver;
    delete iPSObserver;
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::CProfilePSObserver::CProfilePSObserver
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileChangeNotifyHandler::CProfilePSObserver::CProfilePSObserver(
    TUint32 aPSKey )
    : CActive( EPriorityNormal ), iPSKey( aPSKey )
    {
    CActiveScheduler::Add( this );
    iAttached = EFalse;
    }

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::CProfilePSObserver::~CProfilePSObserver
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileChangeNotifyHandler::CProfilePSObserver::~CProfilePSObserver()
    {
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::CProfilePSObserver::RequestNotificationL()
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeNotifyHandler::CProfilePSObserver::RequestNotificationL()
    {
    if ( !iAttached )
        {
        User::LeaveIfError( iProperty.Attach( KPSUidProfileEngine, iPSKey ) );
        iAttached = ETrue;
        }

    iProperty.Subscribe( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::CProfilePSObserver::RunL()
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeNotifyHandler::CProfilePSObserver::RunL()
    {
    RequestNotificationL();
    // The id of the current active profile is always passed to the observer:
    TInt profileId( 0 );
    User::LeaveIfError( iCenRep->Get( KProEngActiveProfile, profileId ) );
    TProfileEvent evt( iPSKey == KProEngActiveProfileModified ?
                       EProfileActiveProfileModified :
                       EProfileNewActiveProfile );
    iProfileChangeObserver->HandleActiveProfileEventL( evt, profileId );
    }

// -----------------------------------------------------------------------------
// CProfileChangeNotifyHandler::CProfilePSObserver::DoCancel()
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeNotifyHandler::CProfilePSObserver::DoCancel()
    {
    iProperty.Cancel();
    }

//  End of File


