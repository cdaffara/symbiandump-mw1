/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SysAp drive list implementation
 *
*/



// INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <badesca.h>
#include <StringLoader.h>
#include <driveinfo.h>
#include "sysapdrivelist.h"
#include "SysAp.hrh"

// CONSTANTS
_LIT( KMemoryCardFSName, "Fat" );
const TInt KNameFormatArrayLen = 2;
#ifdef __WINS__
const TInt KMountDelay = 1000000; // 1s
#endif

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// CSysApDriveList::NewL
// ---------------------------------------------------------------------------
//
CSysApDriveList* CSysApDriveList::NewL( RFs& aFs )
    {
    CSysApDriveList* self = new ( ELeave ) CSysApDriveList( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApDriveList::ConstructL()
    {
    // Get the default memory card. If memory card is unavailable,
    // then get the next best suitable drive.
    
    TInt err( DriveInfo::GetDefaultDrive(
        DriveInfo::EDefaultRemovableMassStorage, iDefaultMemoryCard ) );
    if ( err != KErrNone )
        {
        err = DriveInfo::GetDefaultDrive(
            DriveInfo::EDefaultMassStorage, iDefaultMemoryCard );
        }
    if ( err != KErrNone )
        {
        err = DriveInfo::GetDefaultDrive(
            DriveInfo::EDefaultPhoneMemory, iDefaultMemoryCard );
        }
    if ( err != KErrNone )
        {
        err = DriveInfo::GetDefaultDrive(
            DriveInfo::EDefaultSystem, iDefaultMemoryCard );
        }

    TRACES( RDebug::Print(
        _L( "CSysApDriveList::ConstructL: iDefaultMemoryCard: %d, err: %d" ),
        iDefaultMemoryCard, err ) );

    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::CSysApDriveList
// ---------------------------------------------------------------------------
//
CSysApDriveList::CSysApDriveList( RFs& aFs ) :
        iFs( aFs )
    {
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::~CSysApDriveList
// ---------------------------------------------------------------------------
//
CSysApDriveList::~CSysApDriveList()
    {
    TRACES( RDebug::Print( _L( "CSysApDriveList::~CSysApDriveList" ) ) );
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::MountDrive
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::MountDrive( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::MountDrive: drive: %d" ), aDrive ) );

#ifdef __WINS__ // Let's sleep a second in WINS
    User::After( KMountDelay );
#endif
    TInt ret( iFs.MountFileSystem( KMemoryCardFSName, aDrive ) );
    if ( ret != KErrNone )
        {
        TRACES( RDebug::Print(
            _L( "CSysApDriveList::MountDrive: drive: %d, ret: %d" ),
            aDrive, ret ) );
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::DismountDrive
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::DismountDrive( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::DismountDrive: drive: %d" ), aDrive ) );

#ifdef __WINS__ // Let's sleep a second in WINS
    User::After( KMountDelay );
#endif
	TRequestStatus status;

	TRACES( RDebug::Print( _L("CSysApDriveList::Start DismountDrive: RFs::NotifyDismount()") ) );

	iFs.NotifyDismount( aDrive, status, EFsDismountForceDismount );

	User::WaitForRequest( status );

	TRACES( RDebug::Print( _L("CSysApDriveList::DismountDrive: RFs::NotifyDismount() returned: %d"), status.Int() ) );

    return status.Int();
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::MemoryCardStatus
// ---------------------------------------------------------------------------
//
TSysApMemoryCardStatus CSysApDriveList::MemoryCardStatus(
        TInt aDrive )
    {
    UpdateDrive( aDrive, EFalse );
    return iStatusCache[ aDrive ].iStatus;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::DefaultMemoryCard
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::DefaultMemoryCard() const
    {
    return iDefaultMemoryCard;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::GetMemoryCardsL
// ---------------------------------------------------------------------------
//
void CSysApDriveList::GetMemoryCardsL(
        RArray< TMemoryCardEntry >& aMemoryCardArray,
        TUint aInclusionMask )
    {
    aMemoryCardArray.Reset();
    TMemoryCardEntry entry;
    for ( TInt i( 0 ); i < KMaxDrives; ++i )
        {
        UpdateDrive( i, EFalse );
        TSysApMemoryCardStatus memoryCardStatus( iStatusCache[ i ].iStatus );
        if ( memoryCardStatus != ESysApMemoryCardStatusNotKnown )
            {
            entry.iDrive = i;
            entry.iStatus = memoryCardStatus;
            entry.iDriveStatus = iStatusCache[ i ].iDriveStatus;
            TBool append( EFalse );
            if ( aInclusionMask == EIncludeAll )
                {
                append = ETrue;
                }
            else if ( ( aInclusionMask & EIncludeInserted ) &&
                entry.iStatus == ESysApMemoryCardInserted ||
                entry.iStatus == ESysApMemoryCardLocked )
                {
                append = ETrue;
                }
            else if ( ( aInclusionMask & EIncludeNonInserted ) &&
                entry.iStatus == ESysApMemoryCardNotInserted )
                {
                append = ETrue;
                }
            else if ( ( aInclusionMask & EIncludeLocked ) &&
                entry.iStatus == ESysApMemoryCardLocked )
                {
                append = ETrue;
                }
            if ( append )
                {
                aMemoryCardArray.AppendL( entry );
                }
            }
        }

    TRACES( RDebug::Print(
        _L( "CSysApDriveList::GetMemoryCardsL: count: %d, inclusion: 0x%x" ),
        aMemoryCardArray.Count(), aInclusionMask ) );
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::GetFormattedDriveNameLC
// ---------------------------------------------------------------------------
//
HBufC* CSysApDriveList::GetFormattedDriveNameLC(
        const TInt aDrive,
        const TInt aTextIdForDefaultName,
        const TInt aTextIdForName ) const
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::GetFormattedDriveNameLC: drive: %d" ),
        aDrive ) );

    TDriveName drvLetter( TDriveUnit( aDrive ).Name() );
    TPtrC drvName;
    TVolumeInfo drvVolumeInfo;
    if ( iFs.Volume( drvVolumeInfo, aDrive ) == KErrNone )
        {
        drvName.Set( drvVolumeInfo.iName );
        }
    HBufC* ret = NULL;
    if ( aTextIdForName && ( drvName.Length() || !aTextIdForDefaultName ) )
        {
        CDesCArray* array = new( ELeave ) CDesCArrayFlat(
            KNameFormatArrayLen );
        CleanupStack::PushL( array );
        array->AppendL( drvLetter );
        array->AppendL( drvName );
        ret = StringLoader::LoadL( aTextIdForName, *array );
        CleanupStack::PopAndDestroy( array );
        }
    else if ( aTextIdForDefaultName )
        {
        ret = StringLoader::LoadL( aTextIdForDefaultName, drvLetter );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    CleanupStack::PushL( ret );
    return ret;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::UpdateDrive
// ---------------------------------------------------------------------------
//
void CSysApDriveList::UpdateDrive( TInt aDrive, TBool aForceUpdate )
    {
    TDriveStateEntry& entry( iStatusCache[ aDrive ] );
    if ( !aForceUpdate && ( entry.iState & EStateStatusUpdated ) )
        {
        return;
        }

    TUint drvStatus( 0 );
    DriveInfo::GetDriveStatus( iFs, aDrive, drvStatus );

    TSysApMemoryCardStatus memoryCardStatus( ESysApMemoryCardStatusNotKnown );
    if ( drvStatus & DriveInfo::EDriveRemovable )
        {
        // Set memory card status for physically 
        // removable memory cards only
        if ( drvStatus & DriveInfo::EDriveLocked )
            {
            memoryCardStatus = ESysApMemoryCardLocked;
            }
        else if ( drvStatus & DriveInfo::EDrivePresent )
            {
            memoryCardStatus = ESysApMemoryCardInserted;
            }
        else
            {
            memoryCardStatus = ESysApMemoryCardNotInserted;
            }
        }

    TRACES( RDebug::Print(
        _L( "CSysApDriveList::UpdateDrive: drive: %d, drvStatus: 0x%x, memoryCardStatus: %d" ),
        aDrive, drvStatus, memoryCardStatus ) );

    entry.iStatus = memoryCardStatus;
    entry.iState |= EStateStatusUpdated;
    entry.iDriveStatus = drvStatus;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::Find
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::Find(
        const RArray< TMemoryCardEntry >& aMemoryCardArray,
        TInt aDrive )
    {
    return aMemoryCardArray.Find( aDrive, TMemoryCardEntry::CompareFind );
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::MarkDriveToEject
// ---------------------------------------------------------------------------
//
void CSysApDriveList::MarkDriveToEject(
        TInt aDrive, TDriveEjectType aEjectUsed )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::MarkDriveToEject: drive: %d, ejectType:%d" ),
        aDrive, aEjectUsed ) );

    ResetDriveToEject( aDrive );
    switch ( aEjectUsed )
        {
        case EEjectFromMenu:
            {
            iStatusCache[ aDrive ].iState |= EStateEjectedFromMenu;
            break;
            }
        case EEjectRemovedWithoutEject:
            {
            iStatusCache[ aDrive ].iState |= EStateRemovedWithoutEject;
            break;
            }
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::ResetDriveToEject
// ---------------------------------------------------------------------------
//
void CSysApDriveList::ResetDriveToEject( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::ResetDriveToEject: drive: %d" ), aDrive ) );

    iStatusCache[ aDrive ].iState &= ~EStateAllEjected;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::ResetDrivesToEject
// ---------------------------------------------------------------------------
//
void CSysApDriveList::ResetDrivesToEject()
    {
    for ( TInt i( 0 ); i < KMaxDrives; ++i )
        {
        ResetDriveToEject( i );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::DriveToEject
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::DriveToEject( TDriveEjectType& aEjectUsed )
    {
    aEjectUsed = EEjectNone;
    TInt ret( KErrNotFound );
    for ( TInt i( 0 ); i < KMaxDrives; ++i )
        {
        TUint state( iStatusCache[ i ].iState );
        if ( state & EStateAllEjected )
            {
            // Store current drive to eject
            ret = i;
            if ( state & EStateEjectedFromMenu )
                {
                aEjectUsed = EEjectFromMenu;
                }
            else if ( state & EStateRemovedWithoutEject )
                {
                aEjectUsed = EEjectRemovedWithoutEject;
                }
            break;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::IsDriveToEject
// ---------------------------------------------------------------------------
//
TBool CSysApDriveList::IsDriveToEject( TInt aDrive )
    {
    if ( iStatusCache[ aDrive ].iState & EStateAllEjected )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::MarkDrivesInsertBeepIgnored
// ---------------------------------------------------------------------------
//
void CSysApDriveList::MarkDrivesInsertBeepIgnored(
      const RArray< TMemoryCardEntry >& aArray )
    {
    TInt count( aArray.Count() );
    for( TInt i( 0 ); i < count; ++i )
        {
        MarkDriveInsertBeepIgnored( aArray[ i ].iDrive );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::MarkDriveInsertBeepIgnored
// ---------------------------------------------------------------------------
//
void CSysApDriveList::MarkDriveInsertBeepIgnored( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::MarkDriveInsertBeepIgnored: drive: %d" ),
        aDrive ) );

    iStatusCache[ aDrive ].iState |= EStateBeepIgnored;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::ResetDriveInsertBeepIgnored
// ---------------------------------------------------------------------------
//
void CSysApDriveList::ResetDriveInsertBeepIgnored( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::ResetDriveInsertBeepIgnored: drive: %d" ),
        aDrive ) );

    iStatusCache[ aDrive ].iState &= ~EStateBeepIgnored;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::ResetDrivesInsertBeepIgnored
// ---------------------------------------------------------------------------
//
void CSysApDriveList::ResetDrivesInsertBeepIgnored()
    {
    for ( TInt i( 0 ); i < KMaxDrives; ++i )
        {
        ResetDriveInsertBeepIgnored( i );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::IsDriveInsertBeepIgnored
// ---------------------------------------------------------------------------
//
TBool CSysApDriveList::IsDriveInsertBeepIgnored( TInt aDrive )
    {
    if ( iStatusCache[ aDrive ].iState & EStateBeepIgnored )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::MarkDriveUnlockQueryShown
// ---------------------------------------------------------------------------
//
void CSysApDriveList::MarkDriveUnlockQueryShown( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::MarkDriveUnlockQueryShown: drive: %d" ),
        aDrive ) );

    iStatusCache[ aDrive ].iState |= EStateUnlockQueryShown;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::ResetDriveUnlockQueryShown
// ---------------------------------------------------------------------------
//
void CSysApDriveList::ResetDriveUnlockQueryShown( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveList::ResetDriveUnlockQueryShown: drive: %d" ),
        aDrive ) );

    iStatusCache[ aDrive ].iState &= ~EStateUnlockQueryShown;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::DriveToUnlock
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::DriveToUnlock()
    {
    TInt ret( KErrNotFound );
    for ( TInt i( 0 ); i < KMaxDrives; ++i )
        {
        TDriveStateEntry& entry( iStatusCache[ i ] );
        if ( entry.iStatus == ESysApMemoryCardLocked &&
            !( entry.iState & EStateUnlockQueryShown ) )
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::TMemoryCardEntry::CompareFind
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::TMemoryCardEntry::CompareFind(
        const TInt* aDrive, const TMemoryCardEntry& aEntry )
    {
    return *aDrive == aEntry.iDrive;
    }

// ---------------------------------------------------------------------------
// CSysApDriveList::NonUsbDriveCount
// ---------------------------------------------------------------------------
//
TInt CSysApDriveList::NonUsbDriveCount( const RArray< TMemoryCardEntry >& aDriveArray )
    {
    TInt ret( 0 );
    TInt count( aDriveArray.Count() );
    for( TInt i( 0 ); i < count; ++i )
        {
        if ( !( aDriveArray[ i ].iDriveStatus & DriveInfo::EDriveUsbMemory ) )
            {
            ++ret;
            }
        }
    TRACES( RDebug::Print( _L( "CSysApDriveList::NonUsbDriveCount: %d" ), ret ) );
    return ret;
    }

// End of File
