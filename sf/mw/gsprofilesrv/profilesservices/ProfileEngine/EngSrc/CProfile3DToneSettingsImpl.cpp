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
* Description:  Implementation of the CProfile3DToneSettingsImpl.
*
*/



// INCLUDE FILES
#include "CProfile3DToneSettingsImpl.h"
#include <centralrepository.h>
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfilesDebug.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfile3DToneSettingsImpl* CProfile3DToneSettingsImpl::NewL()
    {
    CProfile3DToneSettingsImpl* self = new( ELeave ) CProfile3DToneSettingsImpl;
    return self;
    }

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfile3DToneSettingsImpl* CProfile3DToneSettingsImpl::NewL(
    const MProfile3DToneSettings& aProfile3DToneSettings )
    {
    CProfile3DToneSettingsImpl* self = new( ELeave ) CProfile3DToneSettingsImpl;

    self->iEffect = aProfile3DToneSettings.Effect();
    self->iEcho = aProfile3DToneSettings.Echo();

    return self;
    }


// Destructor
CProfile3DToneSettingsImpl::~CProfile3DToneSettingsImpl()
    {
    }


// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfile3DToneSettingsImpl::InternalizeL( CRepository& aCenRep,
                                               TInt aProfileId )
    {
    PRODEBUG( "CProfile3DSettings::InternalizeL begin" );
    TInt tmp( 0 );
    // 3D effect
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEng3DToneEffect,
            aProfileId ), tmp ) );
    iEffect = TProfile3DToneEffect( tmp );

    // 3D echo
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEng3DToneEcho,
            aProfileId ), tmp ) );
    iEcho = TProfile3DToneEcho( tmp );
    PRODEBUG( "CProfile3DSettings::InternalizeL return" );
    }

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfile3DToneSettingsImpl::ExternalizeL( CRepository& aCenRep,
                                               TInt aProfileId ) const
    {
    // 3D effect
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEng3DToneEffect,
            aProfileId ), iEffect ) );

    // 3D echo
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEng3DToneEcho,
            aProfileId ), iEcho ) );

    }

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::Effect
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfile3DToneEffect CProfile3DToneSettingsImpl::Effect() const
    {
    return iEffect;
    }

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::SetEffect
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfile3DToneSettingsImpl::SetEffect( TProfile3DToneEffect aEffect )
    {
    iEffect = aEffect;
    }

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::Echo
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfile3DToneEcho CProfile3DToneSettingsImpl::Echo() const
    {
    return iEcho;
    }

// -----------------------------------------------------------------------------
// CProfile3DToneSettingsImpl::SetEcho
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfile3DToneSettingsImpl::SetEcho( TProfile3DToneEcho aEcho )
    {
    iEcho = aEcho;
    }

//  End of File

