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
* Description:  Profile extra tones implementation.
*
*/



//  INCLUDES
#include "CProfileExtraSettingsImpl.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfileEngUtils.h"
#include "CProfilePttSettingsImpl.h"
#include "CProfileFeedbackSettingsImpl.h"
#include "CProfile3DToneSettingsImpl.h"

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

CProfileExtraSettingsImpl* CProfileExtraSettingsImpl::NewL()
    {
    CProfileExtraSettingsImpl* self = new( ELeave ) CProfileExtraSettingsImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

CProfileExtraSettingsImpl::~CProfileExtraSettingsImpl()
    {
    delete iPttSettings;
    delete iFeedbackSettings;    
    delete i3DToneSettings;
    }

void CProfileExtraSettingsImpl::InternalizeL( CRepository& aCenRep,
                                              TInt aProfileId )
    {
    iPttSettings->InternalizeL( aCenRep, aProfileId );
    iFeedbackSettings->InternalizeL( aCenRep, aProfileId );    
    i3DToneSettings->InternalizeL( aCenRep, aProfileId );
    }

void CProfileExtraSettingsImpl::ExternalizeL( CRepository& aCenRep,
                                              TInt aProfileId ) const
    {
    iPttSettings->ExternalizeL( aCenRep, aProfileId );
    iFeedbackSettings->ExternalizeL( aCenRep, aProfileId );    
    i3DToneSettings->ExternalizeL( aCenRep, aProfileId );
    }

const MProfilePttSettings& CProfileExtraSettingsImpl::ProfilePttSettings() const
    {
    return *iPttSettings;
    }

MProfileSetPttSettings& CProfileExtraSettingsImpl::ProfileSetPttSettings() const
    {
    return *iPttSettings;
    }

const MProfileFeedbackSettings&
CProfileExtraSettingsImpl::ProfileFeedbackSettings() const
    {
    return *iFeedbackSettings;
    }

MProfileSetFeedbackSettings&
CProfileExtraSettingsImpl::ProfileSetFeedbackSettings() const
    {
    return *iFeedbackSettings;
    }

const MProfile3DToneSettings&
CProfileExtraSettingsImpl::Profile3DToneSettings() const
    {
    return *i3DToneSettings;
    }

MProfileSet3DToneSettings&
CProfileExtraSettingsImpl::ProfileSet3DToneSettings() const
    {
    return *i3DToneSettings;
    }

void CProfileExtraSettingsImpl::ConstructL()
    {
    iPttSettings = CProfilePttSettingsImpl::NewL();
    iFeedbackSettings = CProfileFeedbackSettingsImpl::NewL();    
    i3DToneSettings = CProfile3DToneSettingsImpl::NewL();
    }

// End of File

