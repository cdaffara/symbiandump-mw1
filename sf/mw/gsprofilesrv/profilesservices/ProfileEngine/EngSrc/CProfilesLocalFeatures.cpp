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
* Description:  Implementation of the CProfilesLocalFeatures.
*
*/



// INCLUDE FILES
#include "CProfilesLocalFeatures.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfilesDebug.h"
#include <centralrepository.h>
#include <apmstd.h>

namespace
    {
    _LIT( KProfilesSpace, " " );
    }
     
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::CProfilesLocalFeatures
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfilesLocalFeatures::CProfilesLocalFeatures()
    : iFeatureBitMask( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::CProfilesLocalFeatures
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfilesLocalFeatures::CProfilesLocalFeatures( TInt aFeatureBitMask )
    : iFeatureBitMask( aFeatureBitMask )
    {
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfilesLocalFeatures::ConstructL()
    {
    CRepository* cenrep = CRepository::NewL( KCRUidProfilesLV );
    CleanupStack::PushL( cenrep );
    User::LeaveIfError( cenrep->Get( KProfilesLVFlags, iFeatureBitMask ) );
    ReadListKeyL( *cenrep, KProEngBlockedTones, iBlockedTones );
    ReadListKeyL( *cenrep, KProEngBlockedUnprotectedTones,
                  iBlockedUnprotectedTones );
    ReadListKeyL( *cenrep, KProEngBlockedProtectedTones,
                  iBlockedProtectedTones );
    ReadListKeyL( *cenrep, KProEngBlockedDemoPlayTones,
                  iBlockedDemoPlayTones );
    ReadListKeyL( *cenrep, KProEngExcludedTones,
                  iExcludedTones );
    CleanupStack::PopAndDestroy(); // cenrep
    if( ( iBlockedTones.Length() > 0 ) ||
        ( iBlockedUnprotectedTones.Length() > 0 ) ||
        ( iBlockedProtectedTones.Length() > 0 ) )
        {
        iMimeBuf.CreateL( KMaxDataTypeLength + KProfilesSpace().Length() );
        }
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::NewL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfilesLocalFeatures* CProfilesLocalFeatures::NewL()
    {
    CProfilesLocalFeatures* self = new ( ELeave ) CProfilesLocalFeatures();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::NewL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfilesLocalFeatures* CProfilesLocalFeatures::NewL(
        CProfilesLocalFeatures& aLocalFeatures )
    {
    return new ( ELeave )
        CProfilesLocalFeatures( aLocalFeatures.iFeatureBitMask );
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsFeatureSupported
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::IsFeatureSupported( TInt aLocalFeatureId ) const
    {
    return iFeatureBitMask & aLocalFeatureId;
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsBlockedType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::IsBlockedType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedTones, aMimeType );
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsBlockedUnprotectedType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::IsBlockedUnprotectedType(
        const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedUnprotectedTones, aMimeType );
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsBlockedProtectedType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::IsBlockedProtectedType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedProtectedTones, aMimeType );
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsBlockedDemoPlayType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::IsBlockedDemoPlayType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedDemoPlayTones, aMimeType );
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsExcludedType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::IsExcludedType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iExcludedTones, aMimeType );
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::ReadListKeyL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilesLocalFeatures::ReadListKeyL( CRepository& aCenRep,
                                           TUint32 aKey,
                                           RBuf& aValue )
    {
    TBuf<1> tmp; // Magic: 1 char to get length of actual value
    TInt realLen = 0;
    TInt err = aCenRep.Get( aKey, tmp, realLen );
    if ( err == KErrOverflow )
        {
        // Prepare list of blocked MIME types
        aValue.CreateL( realLen + KProfilesSpace().Length() );
        User::LeaveIfError( aCenRep.Get( aKey, aValue ) );
        aValue.Append( KProfilesSpace );
        }
    else
        {
        User::LeaveIfError( err );
        }
    }

// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::ContainsMimeTypeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfilesLocalFeatures::ContainsMimeType( const TDesC& aList,
                                                const TDesC& aMimeType )
    {
    if( aList.Length() == 0 )
        {
        PRODEBUG(
                "CProfilesLocalFeatures::ContainsMimeTypeL(), return false" );
        return EFalse;
        }

    TBool ret( EFalse );
    iMimeBuf.Copy( aMimeType );
    // FindF() would find "audio/3gpp" in "audio/3gpp2" without
    // the added space.
    iMimeBuf.Append( KProfilesSpace );

    // If result is not KErrNotFound, this MIME-type is indeed blocked
    ret = ( aList.FindF( iMimeBuf ) >= 0 );

    PRODEBUG3( "CProfilesLocalFeatures::ContainsMimeTypeL(%S,%S), returning %d",
               &aList, &aMimeType, ret );
    return ret;
    }

CProfilesLocalFeatures::~CProfilesLocalFeatures()
    {
    iBlockedTones.Close();
    iBlockedUnprotectedTones.Close();
    iBlockedProtectedTones.Close();
    iBlockedDemoPlayTones.Close();
    iExcludedTones.Close();
    iMimeBuf.Close();
    }

//  End of File

