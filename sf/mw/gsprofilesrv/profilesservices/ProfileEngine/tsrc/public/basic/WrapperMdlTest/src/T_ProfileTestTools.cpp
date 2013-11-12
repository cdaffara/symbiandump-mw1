/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of the T_ProfileTestTools.
*
*/
////////////////////////////////////////////////////////////////////////
// T_ProfileTestTools.cpp
//

// INCLUDES
#include "T_ProfileTestTools.h"

#include <F32FILE.H>
#include <bautils.h>
#include <CNTITEM.H>
#include "TProfileToneSettings.h"
#include "MProfilesNamesArray.h"
#include "MProfileName.h"
#include "MProfile.h"
#include "MProfileTones.h"
#include "MProfileExtraTones.h"
#include <CPbkContactEngine.h>
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>
#include <HWRMVibraSDKCRKeys.h>
#include "ProfileEnginePrivateCRKeys.h"
#include <CPbkContactItem.h>
#include <MProEngProfile.h>
#include <MProEngProfileName.h>
#include <MProEngProfileNameArray.h>
#include <MProEngTones.h>
#include <MProEngToneSettings.h>
#include <EUnitMacros.h>

TBool T_ProfileTestTools::CompareArrays( const TArray<TContactItemId>& aArray1, const TArray<TContactItemId>& aArray2 )
    {
    TInt count = aArray1.Count();
    if( count != aArray2.Count() )
        {
        return EFalse;
        }
    for( TInt i = 0 ; i < count ; ++i )
        {
        if( aArray1[i] != aArray2[i] ) return EFalse;
        }
    return ETrue;
    }

TBool T_ProfileTestTools::CompareNameArrays(
        const MProEngProfileNameArray& aArray1,
        const MProfilesNamesArray& aArray2 )
    {
    TInt count1( aArray1.MdcaCount() );
    TInt count2( aArray2.MdcaCount() );
    if( count1 != count2 )
        {
        return EFalse;
        }
    for( TInt i(0); i<count1; ++i )
        {
        if( aArray1.MdcaPoint( i ).Compare( aArray2.MdcaPoint( i ) ) != 0 )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

TBool T_ProfileTestTools::CompareProfiles( MProEngProfile& ap1, MProfile& ap2 )
    {
    EUNIT_PRINT( _L( "CompareProfiles 1" ) );
    MProEngProfileName& pn1( ap1.ProfileName() );
    const MProfileName& pn2( ap2.ProfileName() );
    EUNIT_PRINT( _L( "CompareProfiles id1=%d, id2=%d" ), pn1.Id(), pn2.Id() );
    if( pn1.Id() != pn2.Id() )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 2" ) );
    if( pn1.Name().Compare( pn2.Name() ) != 0 )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 3" ) );
    MProEngTones& pt1( ap1.ProfileTones() );
    MProEngToneSettings& pts1( ap1.ToneSettings() );
    const MProfileTones& pt2( ap2.ProfileTones() );
    const MProfileExtraTones& pet2( ap2.ProfileExtraTones() );
    const TProfileToneSettings& pts2( pt2.ToneSettings() );

    if( pt1.RingingTone1().Compare( pt2.RingingTone1() ) != 0 )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 4" ) );
    if( pt1.RingingTone2().Compare( pt2.RingingTone2() ) != 0 )
        {
        return EFalse;
        }
    const TDesC& msgTone1( pt1.MessageAlertTone() );
    const TDesC& msgTone2( pt2.MessageAlertTone() );
    EUNIT_PRINT( _L( "CompareProfiles 5 msgt1=%S, msgt2=%S" ), &msgTone1, &msgTone2 );
    if( pt1.MessageAlertTone().Compare( pt2.MessageAlertTone() ) != 0 )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 6" ) );
    if( pt1.EmailAlertTone().Compare( pet2.EmailAlertTone() ) != 0 )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 7" ) );
    if( pt1.VideoCallRingingTone().Compare( pet2.VideoCallRingingTone() ) != 0 )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 8" ) );
    if( pts1.RingingType() != pts2.iRingingType )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 9" ) );
    if( pts1.KeypadVolume() != pts2.iKeypadVolume )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 10" ) );
    if( pts1.RingingVolume() != pts2.iRingingVolume )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 11" ) );
    if( pts1.VibratingAlert() != pts2.iVibratingAlert )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 12" ) );
    if( pts1.WarningAndGameTones() != pts2.iWarningAndGameTones )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 13" ) );
    if( pts1.TextToSpeech() != pts2.iTextToSpeech )
        {
        return EFalse;
        }
    EUNIT_PRINT( _L( "CompareProfiles 14" ) );
    const TArray<TContactItemId> cids1( ap1.AlertForL() );
    const TArray<TContactItemId> cids2( ap2.AlertForL() );
    TBool result( CompareArrays( cids1, cids2 ) );
    EUNIT_PRINT( _L( "CompareProfiles 15 result=%d" ), result );
    return result;
    }

TBool T_ProfileTestTools::CheckArray( const MProfilesNamesArray& aArray )
    {
    TInt count = aArray.MdcaCount();
    TInt correctCount( 5 );
#ifdef __OFFLINE_MODE
    ++correctCount;
#endif
#ifdef __DRIVE_MODE
    ++correctCount;
#endif
    if( count != correctCount )
        {
        return EFalse;
        }
    for( TInt i = 0 ; i < count ; ++i )
        {
        const MProfileName* pn = aArray.ProfileName( i );
        if( !pn )
            {
            return EFalse;
            }
        if( pn->Name().Compare( T_ProfileTestTools::ProfileName( i ) ) != 0 ||
            pn->Id() != i )
            {
            return EFalse;
            }
        }
    return ETrue;
    }


const TDesC& T_ProfileTestTools::ProfileName( TInt aId )
    {
    switch( aId )
        {
        case 2:
            {
            return KProfileMeetingName;
            }
        case 1:
            {
            return KProfileSilentName;
            }
        case 3:
            {
            return KProfileOutdoorName;
            }
        case 4:
            {
            return KProfilePagerName;
            }
#ifdef __OFFLINE_MODE
        case 5:
            {
            return KProfileOfflineName;
            }
#endif
#ifdef __DRIVE_MODE
        case 6:
            {
            return KProfileDriveName;
            }
#endif
        case 0:
        default:
            {
            return KProfileGeneralName;
            }
        }
    }

const TDesC& T_ProfileTestTools::ProfileMessageAlertTone( TInt aId )
    {
    switch( aId )
        {
        case 2:
            {
            return KProfileBeepTwice;
            }
        case 1:
            {
            return KProfileNoSound;
            }
        case 3:
        case 4:
        case 0:
        default:
            {
            return KProfileMessageTone;
            }
        }
    }

const TDesC& T_ProfileTestTools::ProfileRingingTone( TInt aId )
    {
    switch( aId )
        {
        case 2:
            {
            return KProfileBeep;
            }
        case 1:
        case 3:
        case 4:
        case 0:
        default:
            {
            return KProfileNokiaTune;
            }
        }
    }

const TDesC& T_ProfileTestTools::ProfileIMMessageAlertTone( TInt aId )
    {
    switch( aId )
        {
        case 1:
            {
            return KProfileNoSound;
            }
        case 2:
            {
            return KProfileBeepTwice;
            }
        case 3:
        case 4:
        case 0:
        default:
            {
            return KProfileIMMessageTone;
            }
        }
    }

const TDesC& T_ProfileTestTools::ProfileEmailAlertTone( TInt aId )
    {
    switch( aId )
        {
        case 1:
            {
            return KProfileNoSound;
            }
        case 2:
            {
            return KProfileBeepTwice;
            }
        case 3:
        case 4:
        case 0:
        default:
            {
            return KProfileMessageTone;
            }
        }
    }

const TDesC& T_ProfileTestTools::ProfileVideoCallRingingTone( TInt /* aId */ )
    {
    return KProfileNokiaTune;
    }

TBool T_ProfileTestTools::CheckToneSettings( TInt aId, const TProfileToneSettings&  aSettings )
    {
    switch( aId )
        {
        case 1:
            {
            if ( aSettings.iRingingType == EProfileRingingTypeSilent &&
                 aSettings.iKeypadVolume == EProfileKeypadVolumeOff &&
                 aSettings.iRingingVolume == 1 &&
                 !( aSettings.iVibratingAlert ) &&
                 !( aSettings.iWarningAndGameTones ) )
                {
                return ETrue;
                }
            else
                return EFalse;
            }
        case 2:
            {
            if ( aSettings.iRingingType == EProfileRingingTypeBeepOnce &&
                 aSettings.iKeypadVolume == EProfileKeypadVolumeOff &&
                 aSettings.iRingingVolume == 1 &&
                 !( aSettings.iVibratingAlert ) &&
                 !( aSettings.iWarningAndGameTones ) )
                {
                return ETrue;
                }
            else
                return EFalse;
            }
        case 3:
            {
            if ( aSettings.iRingingType == EProfileRingingTypeRinging &&
                 aSettings.iKeypadVolume == EProfileKeypadVolumeLevel3 &&
                 aSettings.iRingingVolume == 10 &&
                 aSettings.iVibratingAlert &&
                 aSettings.iWarningAndGameTones )
                {
                return ETrue;
                }
            else
                return EFalse;
            }
        case 4:
            {
            if ( aSettings.iRingingType == EProfileRingingTypeSilent &&
                 aSettings.iKeypadVolume == EProfileKeypadVolumeLevel2 &&
                 aSettings.iRingingVolume == 7 &&
                 aSettings.iVibratingAlert &&
                 aSettings.iWarningAndGameTones )
                {
                return ETrue;
                }
            else
                return EFalse;
            }
        case 0:
        default:
            {
            if ( aSettings.iRingingType == EProfileRingingTypeRinging &&
                 aSettings.iKeypadVolume == EProfileKeypadVolumeLevel2 &&
                 aSettings.iRingingVolume == 7 &&
                 !( aSettings.iVibratingAlert ) &&
                 aSettings.iWarningAndGameTones )
                {
                return ETrue;
                }
            else
                return EFalse;
            }
        }
    }

void T_ProfileTestTools::CreateContactGroupsL( TInt aNumber, RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine )
    {
    for( TInt i = 0 ; i < aNumber ; ++i )
        {
        CContactGroup* tmp = aContactEngine.CreateContactGroupL( _L( "ProfileTestGroup" ) );
        CleanupStack::PushL( tmp );
        User::LeaveIfError( aArray.Append( tmp->Id() ) );
        CleanupStack::PopAndDestroy();  // tmp;
        }
    }

void T_ProfileTestTools::DeleteContactGroups( RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine )
    {
    TInt count = aArray.Count();
    for( TInt i = 0 ; i < count ; ++i )
        {
        TRAP_IGNORE( aContactEngine.DeleteContactGroupL( aArray[i] ) );
        }
    }

void T_ProfileTestTools::CreateContactsL( TInt aNumber, RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine )
    {
    for( TInt i = 0 ; i < aNumber ; ++i )
        {
        CPbkContactItem* tmp = aContactEngine.CreateEmptyContactL();
        CleanupStack::PushL( tmp );
        aContactEngine.AddNewContactL( *tmp );
        User::LeaveIfError( aArray.Append( tmp->Id() ) );
        CleanupStack::PopAndDestroy();  // tmp;
        }
    }

void T_ProfileTestTools::DeleteContacts( RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine )
    {
    TInt count = aArray.Count();
    for( TInt i = 0 ; i < count ; ++i )
        {
        TRAP_IGNORE( aContactEngine.DeleteContactL( aArray[i] ) );
        }
    }

void T_ProfileTestTools::WriteAlertForIdsToCenrepL( TInt aProfileId, RArray<TContactItemId>& aArray )
    {
    CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
    CleanupStack::PushL( cenrep );
    TBuf8< 40 > alertForBuf; // max 10 ids
    TInt count( aArray.Count() );
    for( TInt i( 0 ) ; i < count ; ++i )
        {
        TContactItemId id( aArray[i] );
        TUint8 pieceOfId( ( aArray[i] >> 24 ) & 0x000000FF );
        alertForBuf.Append( pieceOfId );
        pieceOfId = ( aArray[i] >> 16 & 0x000000FF );
        alertForBuf.Append( pieceOfId );
        pieceOfId = ( aArray[i] >> 8 & 0x000000FF );
        alertForBuf.Append( pieceOfId );
        pieceOfId = ( aArray[i] & 0x000000FF );
        alertForBuf.Append( pieceOfId );
        }
    User::LeaveIfError(
            cenrep->Set( ( aProfileId << 24 ) | KProEngAlertForGroups,
                         alertForBuf ) );

    CleanupStack::PopAndDestroy(); // cenrep
    }

void T_ProfileTestTools::DeleteAlertForIdsFromCenrep( TInt aProfileId )
    {
    TRAP_IGNORE(
           CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
           TBuf8< 1 > alertForBuf;
           alertForBuf.Zero();

           cenrep->Set( ( aProfileId << 24 ) | KProEngAlertForGroups,
                        alertForBuf );

           delete cenrep;
         );
    }

void T_ProfileTestTools::ResetRepository()
    {
    TRAP_IGNORE(
        CRepository* cr = CRepository::NewL( KCRUidProfileEngine );
        cr->Reset();
        delete cr;
        );
    }

TBool T_ProfileTestTools::IsCenrepFeatureEnabledL( TUid aUid, TUint32 aKey )
    {
    CRepository* cenrep = CRepository::NewLC( aUid );
    TInt value( 0 );
    User::LeaveIfError( cenrep->Get( aKey, value ) );
    CleanupStack::PopAndDestroy( cenrep );
    return value;
    }

//  End of File
