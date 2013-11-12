/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApCenRepController implementation.
*
*/


// INCLUDE FILES
#include "SysApCenRepController.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"
#include "coreapplicationuisprivatecrkeys.h"
#include "accessoriescrkeys.h"


// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepController* CSysApCenRepController::NewL()
// ----------------------------------------------------------------------------

CSysApCenRepController* CSysApCenRepController::NewL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepController::NewL" ) ) );
    CSysApCenRepController* self = new ( ELeave ) CSysApCenRepController();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepController::CSysApCenRepController()
// ----------------------------------------------------------------------------

CSysApCenRepController::CSysApCenRepController()
    {
    }

// ----------------------------------------------------------------------------
// CSysApCenRepController::~CSysApCenRepController()
// ----------------------------------------------------------------------------

CSysApCenRepController::~CSysApCenRepController()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepController") ) );
    delete iSysApRepository;
    delete iProfileSettingsRepository;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepController::ConstructL()
// ----------------------------------------------------------------------------

void CSysApCenRepController::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepController::ConstructL start" ) ) );
    TRAPD( sysapErr, iSysApRepository = CRepository::NewL( KCRUidCoreApplicationUIsSysAp ) );     // 0x101F8765
    TRACES( RDebug::Print( _L("CSysApCenRepController::ConstructL sysapErr=%d" ), sysapErr ) );
    User::LeaveIfError( sysapErr );
    TRAPD( profileErr, iProfileSettingsRepository = CRepository::NewL( KCRUidProfileSettings ) ); // 0x101F877D
    TRACES( RDebug::Print( _L("CSysApCenRepController::ConstructL profileErr=%d" ), profileErr ) );
    User::LeaveIfError( profileErr );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepController::GetInt()
// ----------------------------------------------------------------------------

TInt CSysApCenRepController::GetInt( const TUid aRepository, const TUint32 aKey, TBool* aErrorReturn )
    {
    TInt returnValue( 0 );
    TInt err( KErrNone );
    TBool errorReturn = EFalse;

    if ( aRepository == KCRUidCoreApplicationUIsSysAp )
        {
        err = iSysApRepository->Get( aKey, returnValue );
        if ( err )
            {
            TRACES( RDebug::Print( _L("CSysApCenRepController::GetInt: err=%d (key)" ), err ) );
            returnValue = err;
            errorReturn = ETrue;
            }
        }
    else
        {
        CRepository* repository = NULL;
        TRAP( err, repository = CRepository::NewL( aRepository ) );
        if ( err )
            {
            TRACES( RDebug::Print( _L("CSysApCenRepController::GetInt: err=%d (repository)" ), err ) );
            returnValue = err;
            errorReturn = ETrue;
            }
        else
            {
            err = repository->Get( aKey, returnValue );
            if ( err )
                {
                TRACES( RDebug::Print( _L("CSysApCenRepController::GetInt: err=%d (key)" ), err ) );
                returnValue = err;
                errorReturn = ETrue;
                }
            }
        delete repository;
        }

    if ( aErrorReturn != NULL ) // caller of this function wants to make distinction between real and error value
        {
        *aErrorReturn = errorReturn;
        }

    return returnValue;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepController::SetInt()
// ----------------------------------------------------------------------------

void CSysApCenRepController::SetInt( const TUid aRepository, const TUint32 aKey, const TInt aNewValue )
    {
    TInt err( KErrNone );
    if ( aRepository == KCRUidCoreApplicationUIsSysAp )
        {
        err = iSysApRepository->Set( aKey, aNewValue );
        }
    else
        {
        CRepository* repository = NULL;
        TRAP( err, repository= CRepository::NewL( aRepository ) );
        if ( !err )
            {
            err = repository->Set( aKey, aNewValue );
            }
        delete repository;
        }
    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepController::SetInt: err=%d" ), err ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepController::ProfileIdForSelectedAccessory
// ----------------------------------------------------------------------------

TInt CSysApCenRepController::ProfileIdForSelectedAccessory( TSysApAccessoryType aAccessoryType )
    {
    TInt err( 0 );
    TInt accessoryProfileId( 0 );
    switch( aAccessoryType )
        {
        case ESysApHeadset:
            err = iProfileSettingsRepository->Get( KSettingsHeadsetDefaultProfile ,accessoryProfileId );
            break;
        case ESysApLoopset:
            err = iProfileSettingsRepository->Get( KSettingsLoopsetDefaultProfile ,accessoryProfileId );
            break;
        case ESysApTty:
            err = iProfileSettingsRepository->Get( KSettingsTTYDefaultProfile ,accessoryProfileId );
            break;
        case ESysApWirelessCarkit:
            err = iProfileSettingsRepository->Get( KSettingsWirelessCarkitDefaultProfile ,accessoryProfileId );
            break;
        case ESysApCarkit:
            err = iProfileSettingsRepository->Get( KSettingsCarKitDefaultProfile ,accessoryProfileId );
            break;
        case ESysApMusicStand:
            err = iProfileSettingsRepository->Get( KSettingsMusicStandDefaultProfile ,accessoryProfileId );
            break;
        case ESysApTvOut:
            err = iProfileSettingsRepository->Get( KSettingsTvOutDefaultProfile ,accessoryProfileId );
            break;
        case ESysApHeadphones:
            err = iProfileSettingsRepository->Get( KSettingsHeadphonesDefaultProfile ,accessoryProfileId );
            break;
        default:
            break;
        }

    if( err )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepController::ProfileIdForSelectedAccessory: err=%d" ), err ) );
        return 0; // General profile
        }
    else
        {
        return accessoryProfileId;
        }
    }

// End of File

