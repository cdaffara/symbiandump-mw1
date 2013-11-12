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
* Description:  Implementation of CProEngMediaVariation.
*
*/



// INCLUDE FILES
#include    "CProEngMediaVariation.h"
#include    "ProEngMmfInfoUtility.h"
#include 	<centralrepository.h>
#include    <apgcli.h> // RApaLsSession
#include    <ProfileEngineInternalCRKeys.h>
#include    <MProfileUtilitySingleton.h>
#include    <MProfilesLocalFeatures.h>

namespace
	{
	// CONSTANTS
	_LIT( KProEngVideoMimeType, "video/*" );
	_LIT( KProEngRMVideoMimeType, "application/vnd.rn-realmedia" );
	_LIT( KProEngSDPVideoMimeType, "application/sdp" );
#ifdef RD_VIDEO_AS_RINGING_TONE	
    const TInt KProEngFeatureIdVideoAsRingingTone( 1 );
#else
    const TInt KProEngFeatureIdVideoAsRingingTone( 0 );
#endif
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngMediaVariation::CProEngMediaVariation
// -----------------------------------------------------------------------------
//
CProEngMediaVariation::CProEngMediaVariation()
    {
    }

// -----------------------------------------------------------------------------
// CProEngMediaVariation::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngMediaVariation::ConstructL()
    {
    User::LeaveIfError( iApaLsSession.Connect() );

    // Singleton must be released in destructor
    iLocalFeatures = &( ProfileUtilityInstanceL().ProfilesLocalFeatures() );
                        
    const TInt KProEngMimesGranularity( 10 );
    iSupportedMimeTypes = new ( ELeave ) CDesC8ArrayFlat(
            KProEngMimesGranularity );
    iMimeTypeBuf.CreateL( KMaxDataTypeLength ); 
    ProEngMmfInfoUtility::GetSupportedMimeTypesL( *iSupportedMimeTypes );
    }

// -----------------------------------------------------------------------------
// CProEngMediaVariation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngMediaVariation* CProEngMediaVariation::NewL()
    {
    CProEngMediaVariation* self =
        new ( ELeave ) CProEngMediaVariation();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CProEngMediaVariation::~CProEngMediaVariation()
    {
    delete iSupportedMimeTypes;
    iMimeTypeBuf.Close();
    iApaLsSession.Close();
    
    if ( iLocalFeatures )
        {
        ReleaseProfileUtility();    
        }
    }

// -----------------------------------------------------------------------------
// CProEngMediaVariation::DataTypeL
// -----------------------------------------------------------------------------
//
void CProEngMediaVariation::GetDataTypeL(
        const TDesC& aFileName, TDes16& aDataType ) const
    {
    TUid dummyUid = { 0 }; // instantiate as zero
    TDataType dataType( dummyUid );
    User::LeaveIfError(
        iApaLsSession.AppForDocument( aFileName, dummyUid, dataType ) );
    aDataType.Copy( dataType.Des8() );
    }

// -----------------------------------------------------------------------------
// CProEngMediaVariation::IsSupported
// -----------------------------------------------------------------------------
//
TBool CProEngMediaVariation::IsSupported( const TDesC16& aMimeType )
    {
    if( !KProEngFeatureIdVideoAsRingingTone &&
        ( ( aMimeType.MatchF( KProEngVideoMimeType ) == 0 ) ||
          ( aMimeType.CompareF( KProEngRMVideoMimeType ) == 0 ) ||
          ( aMimeType.CompareF( KProEngSDPVideoMimeType ) == 0 ) ) )
        {
        return EFalse;
        }

    if( iLocalFeatures->IsBlockedType( aMimeType ) ||
        iLocalFeatures->IsExcludedType( aMimeType ) )
        {
        return EFalse;
        }

    TInt ignore( 0 );
    iMimeTypeBuf.Copy( aMimeType );
    if( iSupportedMimeTypes->FindIsq( iMimeTypeBuf, ignore ) != KErrNone )
        {
        return EFalse;
        }
     
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CProEngMediaVariation::IsAllowedProtected
// -----------------------------------------------------------------------------
//
TBool CProEngMediaVariation::IsAllowedProtected( const TDesC16& aMimeType )
    const
    {
    return !iLocalFeatures->IsBlockedProtectedType( aMimeType );
    }

// -----------------------------------------------------------------------------
// CProEngMediaVariation::IsAllowedUnProtected
// -----------------------------------------------------------------------------
//
TBool CProEngMediaVariation::IsAllowedUnProtected( const TDesC16& aMimeType )
    const
    {
    return !iLocalFeatures->IsBlockedUnprotectedType( aMimeType );
    }

//  End of File

