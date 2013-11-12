/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#include "cainstallnotifier.h"
#include "cadef.h"

using namespace Usif;

EXPORT_C CCaInstallNotifier* CCaInstallNotifier::NewL(
        MCaInstallListener& aListener, TNotificationType aNotificationType )
    {
    CCaInstallNotifier* self = new ( ELeave ) CCaInstallNotifier( aListener );
    CleanupStack::PushL( self );
    self->ConstructL( aNotificationType );
    CleanupStack::Pop( self );
    return self;
    }

CCaInstallNotifier::~CCaInstallNotifier()
    {
    delete iNotifier;
    }

CCaInstallNotifier::CCaInstallNotifier( MCaInstallListener& aListener ) :
        iListener( aListener )
{
}

void CCaInstallNotifier::ConstructL( TNotificationType aNotificationType )
{
    iNotifier = CSifOperationsNotifier::NewL( *this );
    iNotificationType = aNotificationType;
}

void CCaInstallNotifier::EndOperationHandler(const CSifOperationEndData& aEndData)
{
    TRAP_IGNORE
        (
        if( aEndData.ErrorCode() == KErrNone )
            {
            iListener.HandleInstallNotifyL();
            }
        iNotifier->CancelSubscribeL( iKey );
        )
}

void CCaInstallNotifier::ProgressOperationHandler(
        const CSifOperationProgressData& /*aProgressData*/)
{
}

void CCaInstallNotifier::StartOperationHandler(
        TUint aKey, const CSifOperationStartData& aStartData)
{
    TRAP_IGNORE
        (
        if( iNotificationType == EAllTypesNotification )
            {
            iKey = aKey;
            iNotifier->SubscribeL( aKey, ETrue );
            }
        else if ( iNotificationType == ESisInstallNotification
                && aStartData.SoftwareType().Compare(
                        KCaAttrAppTypeValueNative ) == KErrNone )
            {
            iKey = aKey;
            iNotifier->SubscribeL( aKey, ETrue );
            }
        )
}
