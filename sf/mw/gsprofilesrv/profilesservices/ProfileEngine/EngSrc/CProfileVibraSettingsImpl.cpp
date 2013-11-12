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
* Description:  Profile PTT settings implementation.
*
*/

#include "CProfileVibraSettingsImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"

#include <ProfileInternal.hrh>
#include <centralrepository.h>


// ============================ MEMBER FUNCTIONS ===============================

//Default Constructor
CProfileVibraSettingsImpl::CProfileVibraSettingsImpl() : iAlertVibra( 0 )
    {
    
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileVibraSettingsImpl::ConstructL()
    {
    
    }


// -----------------------------------------------------------------------------
// CProfilePresenceImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileVibraSettingsImpl *CProfileVibraSettingsImpl::NewL()
    {
    CProfileVibraSettingsImpl* self = new( ELeave ) CProfileVibraSettingsImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// Destructor
CProfileVibraSettingsImpl::~CProfileVibraSettingsImpl()
    {
    
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileVibraSettingsImpl::InternalizeL( CRepository& aCenRep,
                                           TInt aProfileId )
    {
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngAlertVibra,
            aProfileId ), iAlertVibra) );
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileVibraSettingsImpl::ExternalizeL( CRepository& aCenRep,
                                           TInt aProfileId ) const
    {
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngAlertVibra,
            aProfileId ), iAlertVibra ) );
    }


// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::AlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileVibraSettingsImpl::AlertVibra() const
    {
    return iAlertVibra;
    }


// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::RingAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileVibraSettingsImpl::RingAlertVibra() const
    {
    return iAlertVibra & EProfileRingAlertVibra;
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::MessageAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CProfileVibraSettingsImpl::MessageAlertVibra() const
    {
    return iAlertVibra & EProfileMessageAlertVibra;
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::EmailAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CProfileVibraSettingsImpl::EmailAlertVibra() const
    {
    return iAlertVibra & EProfileEmailAlertVibra;
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::ReminderAlarmVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CProfileVibraSettingsImpl::ReminderAlarmVibra() const
    {
    return iAlertVibra & EProfileReminderAlarmVibra;
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::InformationVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CProfileVibraSettingsImpl::InformationVibra() const
    {
    return iAlertVibra & EProfileInformationVibra;
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::SetAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CProfileVibraSettingsImpl::SetAlertVibra(TInt aAlertVibra)
    {
    iAlertVibra = aAlertVibra;
    }


// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::SetRingAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CProfileVibraSettingsImpl::SetRingAlertVibra(TBool aRingAlertVibra)
    {
    if ( aRingAlertVibra )
        {
        iAlertVibra |= EProfileRingAlertVibra;
        }
    else
        {
        iAlertVibra &= ~EProfileRingAlertVibra;
        }
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::SetMessageAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CProfileVibraSettingsImpl::SetMessageAlertVibra(TBool aMessageAlertVibra)
    {
    if ( aMessageAlertVibra )
        {
        iAlertVibra |= EProfileMessageAlertVibra;
        }
    else
        {
        iAlertVibra &= ~EProfileMessageAlertVibra;
        }
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::SetEmailAlertVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CProfileVibraSettingsImpl::SetEmailAlertVibra(TBool aEmailAlertVibra)
    {
    if ( aEmailAlertVibra )
        {
        iAlertVibra |= EProfileEmailAlertVibra;
        }
    else
        {
        iAlertVibra &= ~EProfileEmailAlertVibra;
        }
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::SetReminderAlarmVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CProfileVibraSettingsImpl::SetReminderAlarmVibra(TBool aReminderAlarmVibra)
    {
    if ( aReminderAlarmVibra )
        {
        iAlertVibra |= EProfileReminderAlarmVibra;
        }
    else
        {
        iAlertVibra &= ~EProfileReminderAlarmVibra;
        }   
    }

// -----------------------------------------------------------------------------
// CProfileVibraSettingsImpl::SetInformationVibra
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CProfileVibraSettingsImpl::SetInformationVibra(TBool aInformationVibra)
    {
    if ( aInformationVibra )
        {
        iAlertVibra |= EProfileInformationVibra;
        }
    else
        {
        iAlertVibra &= ~EProfileInformationVibra;
        }   
    }

//End of File
