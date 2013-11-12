/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: casatnotifier.cpp
*
*/

#include "casatnotifier.h"

// ============================ MEMBER FUNCTIONS =============================
// -----------------------------------------------------------------------------
// CCaSatNotifier::CCaSatNotifier
// C++ default constructor
// -----------------------------------------------------------------------------
//
CCaSatNotifier::CCaSatNotifier( MCaSatNotifierCallback* aCallback,
        TUid aCategory, TUint aKey ) :
    CActive( EPriorityNormal )
    {
    CActiveScheduler::Add( this );

    iCallback = aCallback;
    iCategory = aCategory;
    iKey = aKey;
    }

// -----------------------------------------------------------------------------
// CCaSatNotifier::ConstructL
// S2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CCaSatNotifier::ConstructL()
    {
    // Prepare automatically
    iProperty.Attach( iCategory, iKey );
    SetActive();
    iStatus = KRequestPending;
    iProperty.Subscribe( iStatus );
    }

// ---------------------------------------------------------------------------
// CCaSatNotifier::NewLC
// ---------------------------------------------------------------------------
//
CCaSatNotifier* CCaSatNotifier::NewLC(
        MCaSatNotifierCallback* aCallback, TUid aCategory, TUint aKey )
    {
    CCaSatNotifier* self = new ( ELeave ) CCaSatNotifier( aCallback,
            aCategory, aKey );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// ---------------------------------------------------------------------------
// CCaSatNotifier::NewL
// ---------------------------------------------------------------------------
//
CCaSatNotifier* CCaSatNotifier::NewL(
        MCaSatNotifierCallback* aCallback, TUid aCategory, TUint aKey )
    {
    CCaSatNotifier* self = CCaSatNotifier::NewLC( aCallback, aCategory,
            aKey );
    CleanupStack::Pop();//self
    return self;
    }

// ---------------------------------------------------------------------------
// CCaSatNotifier::~CCaSatNotifier
// ---------------------------------------------------------------------------
//

CCaSatNotifier::~CCaSatNotifier()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CCaSatNotifier::DoCancel
// ---------------------------------------------------------------------------
//
void CCaSatNotifier::DoCancel()
    {
    iProperty.Cancel();
    }

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (error ignored)
// ---------------------------------------------------------------------------
// CCaSatNotifier::RunError
// ---------------------------------------------------------------------------
//
TInt CCaSatNotifier::RunError( TInt /*aError*/)
    {
    return KErrNone;
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// ---------------------------------------------------------------------------
// CCaSatNotifier::RunL
// ---------------------------------------------------------------------------
//
void CCaSatNotifier::RunL()
    {
    // Filter out cancel status
    if( iStatus != KErrCancel )
        {
        // Re-issue request before notifying
        SetActive();
        iStatus = KRequestPending;
        iProperty.Subscribe( iStatus );
        iCallback->SatChangeL();
        }
    else
        {
        // For PC-Lint
        }
    }
// End of File
