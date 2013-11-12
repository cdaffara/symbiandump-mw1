/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngTonesImpl.
*
*/



// INCLUDE FILES
#include    "CProEngTonesImpl.h"
#include    "CProEngToneHandler.h"
#include    <MProfileTones.h>
#include    <MProfileExtraTones.h>
#include    <MProfileSetTones.h>
#include    <MProfileSetExtraTones.h>
#include    <MProfileExtended.h>
#include    <ProfileEng.hrh> // TProfileSettingId
#include    <pathinfo.h>
#include    <ProfileEngineDomainConstants.h>

namespace
    {
    // CONSTANTS
    // This flag is only used internally by this class (actually 0x0002 is
    // defined in MProfileExtended.h as EProfileFlagRingingType) for indicating
    // whether ringing tone 2 has been modified or not:
    const TInt KProEngFlagRingingTone2 = 0x0002;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngTonesImpl::CProEngTonesImpl
// -----------------------------------------------------------------------------
//
CProEngTonesImpl::CProEngTonesImpl(
        const MProfileTones& aProfileTones,
        MProfileSetTones& aProfileSetTones,
        const MProfileExtraTones& aProfileExtraTones,
        MProfileSetExtraTones& aProfileSetExtraTones,
        TUint32 aModifiableFlags )
    : iProfileTones( aProfileTones ), iProfileSetTones( aProfileSetTones ),
      iProfileExtraTones( aProfileExtraTones ),
      iProfileSetExtraTones( aProfileSetExtraTones ),
      iModifiableFlags( aModifiableFlags ),
      iNullTone( KNullDesC )
    {
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngTonesImpl::ConstructL()
    {
    // Read the file to be used when KNullDesC is set as an alert tone:
    RBuf toneBuf;
    toneBuf.CreateL( KMaxFileName );
    CleanupClosePushL( toneBuf );
    toneBuf.Copy( TParsePtrC( PathInfo::RomRootPath() ).Drive() );
    toneBuf.Append( KProfileNoSoundPath );

    iEmptyTone = toneBuf.AllocL();
    CleanupStack::PopAndDestroy(); // toneBuf
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngTonesImpl* CProEngTonesImpl::NewL(
        const MProfileTones& aProfileTones,
        MProfileSetTones& aProfileSetTones,
        const MProfileExtraTones& aProfileExtraTones,
        MProfileSetExtraTones& aProfileSetExtraTones,
        TUint32 aModifiableFlags )
    {
    CProEngTonesImpl* self = new ( ELeave ) CProEngTonesImpl(
            aProfileTones, aProfileSetTones, aProfileExtraTones,
            aProfileSetExtraTones, aModifiableFlags );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CProEngTonesImpl::~CProEngTonesImpl()
    {
    delete iEmptyTone;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::RingingTone1
// -----------------------------------------------------------------------------
//
const TDesC& CProEngTonesImpl::RingingTone1() const
    {
    return iProfileTones.RingingTone1();
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::RingingTone2
// -----------------------------------------------------------------------------
//
const TDesC& CProEngTonesImpl::RingingTone2() const
    {
    return iProfileTones.RingingTone2();
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::MessageAlertTone
// -----------------------------------------------------------------------------
//
const TDesC& CProEngTonesImpl::MessageAlertTone() const
    {
    return iProfileTones.MessageAlertTone();
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::EmailAlertTone
// -----------------------------------------------------------------------------
//
const TDesC& CProEngTonesImpl::EmailAlertTone() const
    {
    return iProfileExtraTones.EmailAlertTone();
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::VideoCallRingingTone
// -----------------------------------------------------------------------------
//
const TDesC& CProEngTonesImpl::VideoCallRingingTone() const
    {
    return iProfileExtraTones.VideoCallRingingTone();
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::SetRingingTone1L
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::SetRingingTone1L( const TDesC& aRingingTone )
    {
    if( iModifiableFlags & EProfileFlagRingingTone )
        {
        TPtrC tonePtr( ConvertKNullToEmptyTone( aRingingTone ) );
        iProfileSetTones.SetRingingTone1L( tonePtr );
        iModifiedFlags |= EProfileFlagRingingTone;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::SetRingingTone2L
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::SetRingingTone2L( const TDesC& aRingingTone )
    {
    if( iModifiableFlags & EProfileFlagRingingTone )
        {
        TPtrC tonePtr( ConvertKNullToEmptyTone( aRingingTone ) );
        iProfileSetTones.SetRingingTone2L( tonePtr );
        iModifiedFlags |= KProEngFlagRingingTone2;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::SetMessageAlertToneL
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::SetMessageAlertToneL( const TDesC& aMessageAlertTone )
    {
    if( iModifiableFlags & EProfileFlagMessageAlertTone )
        {
        TPtrC tonePtr( ConvertKNullToEmptyTone( aMessageAlertTone ) );
        iProfileSetTones.SetMessageAlertToneL( tonePtr );
        iModifiedFlags |= EProfileFlagMessageAlertTone;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::SetEmailAlertToneL
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::SetEmailAlertToneL( const TDesC& aEmailAlertTone )
    {
    if( iModifiableFlags & EProfileFlagEmailAlertTone )
        {
        TPtrC tonePtr( ConvertKNullToEmptyTone( aEmailAlertTone ) );
        iProfileSetExtraTones.SetEmailAlertToneL( tonePtr );
        iModifiedFlags |= EProfileFlagEmailAlertTone;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::SetVideoCallRingingToneL
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::SetVideoCallRingingToneL( const TDesC& aRingingTone )
    {
    if( iModifiableFlags & EProfileFlagVideoCallRingingTone )
        {
        TPtrC tonePtr( ConvertKNullToEmptyTone( aRingingTone ) );
        iProfileSetExtraTones.SetVideoCallRingingToneL( tonePtr );
        iModifiedFlags |= EProfileFlagVideoCallRingingTone;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::CheckTonesL
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::CheckTonesL( CProEngToneHandler& aToneHandler )
    {
    TInt result( KErrNone );
    if( iModifiedFlags & EProfileFlagRingingTone )
        {
        result = aToneHandler.CheckToneFileL( iProfileTones.RingingTone1(), 
            EProfileSettingIdRingingTone );
        if( result != KErrNone )
            {
            return result;
            }
        }
    if( iModifiedFlags & KProEngFlagRingingTone2 )
        {
        result = aToneHandler.CheckToneFileL( iProfileTones.RingingTone2(),
            EProfileSettingIdRingingTone2 );
        if( result != KErrNone )
            {
            return result;
            }
        }
    if( iModifiedFlags & EProfileFlagMessageAlertTone )
        {
        result = aToneHandler.CheckToneFileL(
                iProfileTones.MessageAlertTone(),
            EProfileSettingIdMsgTone );
        if( result != KErrNone )
            {
            return result;
            }
        }
    if( iModifiedFlags & EProfileFlagEmailAlertTone )
        {
        result = aToneHandler.CheckToneFileL(
                iProfileExtraTones.EmailAlertTone(), 
                EProfileSettingIdEmailTone );
        if( result != KErrNone )
            {
            return result;
            }
        }
    if( iModifiedFlags & EProfileFlagVideoCallRingingTone )
        {
        result = aToneHandler.CheckToneFileL(
                iProfileExtraTones.VideoCallRingingTone(),
                EProfileSettingIdVideoCallTone );
        }
        return result;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::HandleAutomatedContentL
// -----------------------------------------------------------------------------
//
TInt CProEngTonesImpl::HandleAutomatedContent(
        CProEngToneHandler& aToneHandler, const CProEngTonesImpl& aOldTones )
    {
    TInt err( KErrNone );
    if( iModifiedFlags & EProfileFlagRingingTone )
        {
        err = aToneHandler.HandleAutomatedContent(
                EProfileSettingIdRingingTone,
                iProfileTones.RingingTone1(),
                aOldTones.iProfileTones.RingingTone1() );
        iModifiedFlags &= ~EProfileFlagRingingTone;
        }
    if( !err && ( iModifiedFlags & KProEngFlagRingingTone2 ) )
        {
        err = aToneHandler.HandleAutomatedContent(
                EProfileSettingIdRingingTone2,
                iProfileTones.RingingTone2(),
                aOldTones.iProfileTones.RingingTone2() );
        iModifiedFlags &= ~KProEngFlagRingingTone2;
        }
    if( !err && ( iModifiedFlags & EProfileFlagMessageAlertTone ) )
        {
        err = aToneHandler.HandleAutomatedContent(
                EProfileSettingIdMsgTone,
                iProfileTones.MessageAlertTone(),
                aOldTones.iProfileTones.MessageAlertTone() );
        iModifiedFlags &= ~EProfileFlagMessageAlertTone;
        }
    if( !err && ( iModifiedFlags & EProfileFlagEmailAlertTone ) )
        {
        err = aToneHandler.HandleAutomatedContent(
                EProfileSettingIdEmailTone,
                iProfileExtraTones.EmailAlertTone(),
                aOldTones.iProfileExtraTones.EmailAlertTone() );
        iModifiedFlags &= ~EProfileFlagEmailAlertTone;
        }
    if( !err && ( iModifiedFlags & EProfileFlagVideoCallRingingTone ) )
        {
        err = aToneHandler.HandleAutomatedContent(
                EProfileSettingIdVideoCallTone,
                iProfileExtraTones.VideoCallRingingTone(),
                aOldTones.iProfileExtraTones.VideoCallRingingTone() );
        iModifiedFlags &= ~EProfileFlagVideoCallRingingTone;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CProEngTonesImpl::ConvertKNullToEmptyTone
// -----------------------------------------------------------------------------
//
const TDesC& CProEngTonesImpl::ConvertKNullToEmptyTone(
        const TDesC& aTone ) const
    {
    if( aTone.Length() == 0 )
        {
        return *iEmptyTone;
        }
    return aTone;
    }

//  End of File


