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

#include <s32mem.h>

#include <usif/scr/scr.h>
#include <usif/scr/screntries.h>

#include "caprogresscanner.h"
#include "caarraycleanup.inl"

using namespace Usif;

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaProgresScanner* CCaProgresScanner::NewL( IUninstallObserver* aObserver )
    {
    CCaProgresScanner* self = new ( ELeave ) CCaProgresScanner( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaProgresScanner::~CCaProgresScanner()
    {
    delete iNotifier;
    iSoftwareRegistry.Close();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaProgresScanner::CCaProgresScanner( IUninstallObserver* aObserver ) :
    iObserver( aObserver )
    {
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::ConstructL()
    {
    iNotifier = CSifOperationsNotifier::NewL( *this );
    User::LeaveIfError( iSoftwareRegistry.Connect() );//user leave if error.
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::StartOperationHandler( TUint aKey,
        const CSifOperationStartData& aStartData )
    {
    //TODO: temporary solution. Some error in scr was found
    if( aStartData.OperationPhase() == EUninstalling 
            || aStartData.OperationPhase() == EInstalling 
            || aStartData.OperationPhase() == 0)
        {
        TRAP_IGNORE(
                iComponentId = iSoftwareRegistry.GetComponentIdL(
                        aStartData.GlobalComponentId(), 
                        aStartData.SoftwareType() );
                SubscribeL( aKey ); 
                );
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::EndOperationHandler(
        const CSifOperationEndData& aEndData )
    {
    TRAP_IGNORE( EndOperationL( aEndData.ErrorCode() ) );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::ProgressOperationHandler(
        const CSifOperationProgressData& aProgressData )
    {
    TRAP_IGNORE( UpdateProgressL( aProgressData ) );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::UpdateProgressL(
        const CSifOperationProgressData& aProgressData )
    {
    if( aProgressData.Phase() == EUninstalling )
        {
        TInt progress = (aProgressData.CurrentProgess()*100)/aProgressData.Total();
        iObserver->progressChange( iComponentId, progress );
        }
    else
    	{
        //TODO: temporary solution. Some error in scr was found
        //Progress should be remove for different operation
        iObserver->progressChange( iComponentId, -1 );
		CancelSubscribeL();
    	}
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::EndOperationL( TInt aError )
    {
    if( aError == KErrNone )
        {
        iObserver->progressChange( iComponentId, 101 );
        }
    else
        {
        //information for observer that uninstallation was failed
        iObserver->progressChange( iComponentId, KErrGeneral ); 
        }
    CancelSubscribeL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::CancelSubscribeL()
	{
	iNotifier->CancelSubscribeL( iKey );
	}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaProgresScanner::SubscribeL( TUint aKey )
    {
    iKey = aKey;
    iNotifier->SubscribeL( aKey, ETrue );
    iObserver->progressChange( iComponentId, 0 );
    }
