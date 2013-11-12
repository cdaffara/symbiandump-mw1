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
* Description:  Implemention of the ProfileEngUtils.
*
*/



// INCLUDE FILES
#include "ProfileEngUtils.h"
#include "CProfileEngineImpl.h"
#include <ProfileEngineConstants.h>
#include <rsssettings.h>
#include <centralrepository.h>
#include <bldvariant.hrh>

#include <psmsrvdomaincrkeys.h>
#include <psmsettings.h>
#include <e32property.h>
#include <ProfileEnginePrivatePSKeys.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ProfileEngUtils::GetAlternateService
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void ProfileEngUtils::GetAlternateLineService( TInt& aValue )
    {
    aValue = ESSSettingsAlsNotSupported;
    RSSSettings settings;

    if( settings.Open() == KErrNone )
        {
        // Get Alternate Line Service setting. Ignore errors because
        // iAlternateLineService value ESSSettingsAlsNotSupported is also valid.
        settings.Get( ESSSettingsAls, aValue );
        settings.Close();
        }
    }

// -----------------------------------------------------------------------------
// ProfileEngUtils::IsAlternateLineSelected
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool ProfileEngUtils::IsAlternateLineSelected()
    {
    TInt als( ESSSettingsAlsNotSupported );
    GetAlternateLineService( als );
    return ( als == ESSSettingsAlsAlternate );
    }

// -----------------------------------------------------------------------------
// ProfileEngUtils::ResolveKey
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint32 ProfileEngUtils::ResolveKey( TUint aPartialKey, TInt aProfileId )
    {
    return ( ( aProfileId << KProfileIdShift ) | aPartialKey );
    }

// -----------------------------------------------------------------------------
// ProfileEngUtils::GetBinarySettingL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void ProfileEngUtils::GetBinarySettingL( CRepository& aCenRep,
                                         TUint32 aId,
                                         TDes8& aDes )
    {
    aDes.Zero();
    User::LeaveIfError( aCenRep.Get( aId, aDes ) );
    const TInt KProEngEmptyBinSettingLength( 2 );
    const TInt KProEngZero( 0 );

    if( ( aDes.Length() == KProEngEmptyBinSettingLength ) &&
        ( aDes[0] == KProEngZero && aDes[1] == KProEngZero ) )
        {
            aDes.Zero();
        }
    }

// -----------------------------------------------------------------------------
// ProfileEngUtils::FindValueFromBinaryKey
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt ProfileEngUtils::FindValueFromBinaryKeyL( CRepository& aCenRep,
        TUint32 aCenRepKey,
        TUint8 aValue )
    {
    HBufC8* buf = HBufC8::NewLC(
            NCentralRepositoryConstants::KMaxBinaryLength );
    TPtr8 ptr = buf->Des();
    ProfileEngUtils::GetBinarySettingL( aCenRep, aCenRepKey, ptr );

    TInt result( KErrNotFound );
    TInt len( buf->Length() );
    for( TInt i( 0 ); i<len; ++i )
        {
        if( ptr[ i ] == aValue )
            {
            result = i;
            }
        }
    CleanupStack::PopAndDestroy(); // buf
    return result;
    }

// -----------------------------------------------------------------------------
// ProfileEngUtils::GetUint32Presentation
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void ProfileEngUtils::GetUint32Presentation( TUint32& aResult,
                                             const TDesC8& aBuf,
                                             TInt aOffset )
    {
    aResult = 0;
    const TInt KProEngByteLength( 8 );
    const TInt length( aBuf.Length() );
    const TInt maxBitShift( KProEngByteLength * ( length - 1 ) );

    for( TInt i( 0 ); i<length; ++i )
        {
        aResult |= ( aBuf[aOffset + i] <<
                   ( maxBitShift - ( KProEngByteLength * i ) ) );
        }
    }

// -----------------------------------------------------------------------------
// ProfileEngUtils::PowerSaveMode
//
// -----------------------------------------------------------------------------
//
TBool ProfileEngUtils::PowerSaveMode()
    {
    TBool psm = EFalse;
    
    TRAPD( err, psm = PowerSaveModeL() );
    
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    return psm;
    }


// -----------------------------------------------------------------------------
// ProfileEngUtils::PowerSaveModeL
//
// -----------------------------------------------------------------------------
//
TBool ProfileEngUtils::PowerSaveModeL()
    {

#ifdef FF_POWER_SAVE
    TInt psm = EPsmsrvModeNormal;
    
    // KProEngPsmChanged PubSub key is set by GSProfilesPlugin when power save
    // mode changes. If the key cannot be read (doesn't exist), PSM hasn't
    // changed since the last boot and the mode is read from PSM CenRep.
    if( RProperty::Get( KPSUidProfileEngine, KProEngPsmChanged, psm ) != KErrNone )
        {
        CRepository* cenrep = CRepository::NewLC( KCRUidPowerSaveMode );
        User::LeaveIfError( cenrep->Get( KPsmCurrentMode, psm ) );
        CleanupStack::PopAndDestroy( cenrep );
        }

    if ( psm == EPsmsrvModePowerSave )
        {
        return ETrue;
        }
#endif

    return EFalse;
    }


//  End of File
