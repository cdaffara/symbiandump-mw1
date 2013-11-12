/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *
 */

#include "caclientsubsession.h"
#include "caclientnotifier.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CCaClientNotifier* CCaClientNotifier::NewL(
        RCaClientSubSession* aSubsession )
    {
    CCaClientNotifier* notifier = NewLC( aSubsession );
    CleanupStack::Pop( notifier );
    return notifier;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CCaClientNotifier* CCaClientNotifier::NewLC(
        RCaClientSubSession* aSubsession )
    {
    CCaClientNotifier* notifier = new ( ELeave ) CCaClientNotifier(
            aSubsession );
    CleanupStack::PushL( notifier );
    notifier->ConstructL();
    return notifier;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CCaClientNotifier::ConstructL()
    {
    iSubsession->RegisterForNotificationsL( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CCaClientNotifier::CCaClientNotifier( RCaClientSubSession* aSubsession ) :
    CActive( EPriorityStandard ), iSubsession( aSubsession )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CCaClientNotifier::~CCaClientNotifier()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CCaClientNotifier::RunL()
    {
    TInt error = iStatus.Int();
    if( error == KErrNone )
        {
        TRAP( error, iSubsession->GetChangeInfoL() );
        if( error == KErrNone )
            {
            iSubsession->RegisterForNotificationsL( iStatus );
            SetActive();
            iSubsession->NotifyObserver();
            }
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CCaClientNotifier::DoCancel()
    {
    iSubsession->UnregisterForNotifications();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TInt CCaClientNotifier::RunError( TInt /*aError*/ )
    {
    TRAPD( err, iSubsession->RegisterForNotificationsL( iStatus ) );
    if( err==KErrNone )
        {
        SetActive();
        }
    return KErrNone;
    }
