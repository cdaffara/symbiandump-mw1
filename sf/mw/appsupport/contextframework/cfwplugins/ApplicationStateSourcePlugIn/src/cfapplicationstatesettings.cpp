/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsulates information about application and its views.
*
*/


#include "cfapplicationstatesettings.h"

// ======== MEMBER FUNCTIONS ========

CCFApplicationStateSettings::CCFApplicationStateSettings()
    {
    }

void CCFApplicationStateSettings::ConstructL( const TUid& aUid,
    const TDesC& aName )
    {
    iApplicationInfo = CCFUidNamePair::NewL( aUid, aName );
    }

CCFApplicationStateSettings* CCFApplicationStateSettings::NewL(
    const TUid& aUid,
    const TDesC& aName )
    {
    CCFApplicationStateSettings* self
        = CCFApplicationStateSettings::NewLC( aUid, aName );
    CleanupStack::Pop( self );

    return self;
    }

CCFApplicationStateSettings* CCFApplicationStateSettings::NewLC(
    const TUid& aUid,
    const TDesC& aName )
    {
    CCFApplicationStateSettings* self
        = new( ELeave ) CCFApplicationStateSettings();
    CleanupStack::PushL( self );
    self->ConstructL( aUid, aName );

    return self;
    }

CCFApplicationStateSettings::~CCFApplicationStateSettings()
    {
    delete iApplicationInfo;
    iViewInfo.ResetAndDestroy();
    }


//-----------------------------------------------------------------------------
// CCFApplicationStateSettings::Uid
//-----------------------------------------------------------------------------
//
const TUid& CCFApplicationStateSettings::Uid() const
    {
    return iApplicationInfo->Uid();
    }

//-----------------------------------------------------------------------------
// CCFApplicationStateSettings::Name
//-----------------------------------------------------------------------------
//
const TDesC& CCFApplicationStateSettings::Name() const
    {
    if( !iApplicationInfo )
        {
        return KNullDesC;
        }
    else
        {
        return iApplicationInfo->Name();
        }
    }

//-----------------------------------------------------------------------------
// CCFApplicationStateSettings::AddViewSettingsL
//-----------------------------------------------------------------------------
//
void CCFApplicationStateSettings::AddViewSettingsL( 
    CCFUidNamePair* aViewSettings )
    {
    iViewInfo.AppendL( aViewSettings );
    }

//-----------------------------------------------------------------------------
// CCFApplicationStateSettings::AddViewSettingsL
//-----------------------------------------------------------------------------
//
void CCFApplicationStateSettings::GetViewName( const TUid& aUid, 
    TPtrC& aName ) const
    {
    for ( TInt i = 0; i < iViewInfo.Count(); ++i )
        {
        if ( aUid == iViewInfo[ i ]->Uid() )
            {
            aName.Set( iViewInfo[ i ]->Name() );
            break;
            }
        }
    }
