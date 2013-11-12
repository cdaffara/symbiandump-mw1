/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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


// INCLUDE FILES
#include <MediatorDomainUIDs.h>
#include "SysApMediatorObserver.h"
#include "SysApAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CSysApMediatorObserver::ConstructL()
// ----------------------------------------------------
void CSysApMediatorObserver::ConstructL()
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL()")));

    __ASSERT_ALWAYS( iSysApAppUi, User::Invariant());

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL(): iCommandInitiator")));
    iCommandInitiator = CMediatorCommandInitiator::NewL(this);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL(): iCommandResponder")));
    iCommandResponder = CMediatorCommandResponder::NewL(this);

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL(): iEventProvider")));
    iEventProvider = CMediatorEventProvider::NewL();

    TInt err = iCommandResponder->RegisterCommand(KMediatorSecondaryDisplayDomain,
                                       SecondaryDisplay::KCatStartup,
                                       SecondaryDisplay::ECmdStartupPhaseSkip,
                                       TVersion(1,0,0),
                                       ECapabilitySwEvent,
                                       KMaximumAnimationTime);

    if ( err != KErrNone )
        {
        // If Startup application has not unregistered command, registration will fail
        TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL() - Error registering ECmdStartupPhaseSkip for shutdown animation skip: %d"), err));
        }

    err = iEventProvider->RegisterEvent( KMediatorSecondaryDisplayDomain,
                                                SecondaryDisplay::KCatStartup,
                                                SecondaryDisplay::EMsgStartupEvent,
                                                TVersion(1,0,0),
                                                ECapabilitySwEvent );

    if ( err != KErrNone )
        {
        // If Startup application has not unregistered event, registration will fail
        TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL() - Error registering EMsgStartupEvent for shutdown animation skip: %d"), err));
        }

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::ConstructL(): End")));
    }

// ----------------------------------------------------
// CSysApMediatorObserver::NewL( CSysApAppUi* aStartupAppUi )
// ----------------------------------------------------
CSysApMediatorObserver* CSysApMediatorObserver::NewL( CSysApAppUi* aSysApAppUi )
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::NewL()")));
    CSysApMediatorObserver* self = new (ELeave) CSysApMediatorObserver( aSysApAppUi );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::NewL(): End")));
    return self;
    }

// ----------------------------------------------------
// CSysApMediatorObserver::CSysApMediatorObserver( CSysApAppUi* aSysApAppUi )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CSysApMediatorObserver::CSysApMediatorObserver( CSysApAppUi* aSysApAppUi ) :
    iSysApAppUi( aSysApAppUi )
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::CSysApMediatorObserver()")));
    }

// ----------------------------------------------------
// CSysApMediatorObserver::~CSysApMediatorObserver()
// ----------------------------------------------------
CSysApMediatorObserver::~CSysApMediatorObserver()
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::~CSysApMediatorObserver()")));
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    delete iCommandInitiator;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    delete iCommandResponder;
    delete iEventProvider;
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::~CSysApMediatorObserver(): End")));
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------
// CSysApMediatorObserver::IssueCommand(TInt aCommandId, TInt aData)
// ---------------------------------------------------------
TInt CSysApMediatorObserver::IssueCommand(TInt aCommandId, TInt aData)
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::IssueCommand(aCommandId: %d, aData: %d)"), aCommandId, aData));

    TPckgBuf<TInt> data( aData );
    TInt err = iCommandInitiator->IssueCommand( KMediatorSecondaryDisplayDomain,
                                       SecondaryDisplay::KCatStartup,
                                       aCommandId,
                                       TVersion(1,0,0),
                                       data);

    if ( err != KErrNone )
        {
        TRACES(RDebug::Print( _L("CSysApMediatorObserver::IssueCommand() - Error issuing command: %d"), err));
        }

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::IssueCommand(): End")));
    return err;
    }

// ---------------------------------------------------------
// SysApMediatorObserver::CommandResponseL(...)
// ---------------------------------------------------------
void CSysApMediatorObserver::CommandResponseL( TUid /*aDomain*/,
                                                 TUid /*aCategory*/,
                                                 TInt aCommandId,
                                                 TInt /*aStatus*/,
                                                 const TDesC8& /*aData*/ )
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::CommandResponseL()")));
    if (aCommandId == SecondaryDisplay::ECmdStartupSync)
        {
        // We only issue shutdown animatio sync, so no need to examine response data
        iSysApAppUi->ShutdownAnimationSyncOK();
        }

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::CommandResponseL(): End")));
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------
// CSysApMediatorObserver::MediatorCommandL(...)
// ---------------------------------------------------------
void CSysApMediatorObserver::MediatorCommandL( TUid aDomain,
                                                 TUid aCategory,
                                                 TInt aCommandId,
                                                 TVersion /*aVersion*/,
                                                 const TDesC8& /*aData*/ )
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::MediatorCommandL()")));

    if (aCommandId == SecondaryDisplay::ECmdStartupPhaseSkip)
        {
        TRACES(RDebug::Print( _L("CSysApMediatorObserver::MediatorCommandL(): ECmdStartupPhaseSkip")));

        // At this point only skip command can be the shutdown animation skip
        iSysApAppUi->SkipShutdownAnimation();

        TPckgBuf<TInt> data( NULL );
        iCommandResponder->IssueResponse( aDomain,
                                          aCategory,
                                          aCommandId,
                                          KErrNone,
                                          data );
        }

    TRACES(RDebug::Print( _L("CSysApMediatorObserver::MediatorCommandL(): End")));
    }


// ---------------------------------------------------------
// CSysApMediatorObserver::CancelMediatorCommand(...)
// ---------------------------------------------------------
void CSysApMediatorObserver::CancelMediatorCommand( TUid /*aDomain*/,
                                                      TUid /*aCategory*/,
                                                      TInt /*aCommandId*/ )
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::CancelMediatorCommand()")));

    // Do nothing.
    }

// ---------------------------------------------------------
// SysApMediatorObserver::ShutdownAnimationSkipped()
// ---------------------------------------------------------
void CSysApMediatorObserver::ShutdownAnimationSkipped()
    {
    TRACES(RDebug::Print( _L("CSysApMediatorObserver::ShutdownAnimationSkipped()")));

    // Raise skip event
    TPckgBuf<TInt> data( SecondaryDisplay::EShutdownAnimSkipped );
    TInt err = iEventProvider->RaiseEvent( KMediatorSecondaryDisplayDomain,
                                       SecondaryDisplay::KCatStartup,
                                       SecondaryDisplay::EMsgStartupEvent,
                                       TVersion(1,0,0),
                                       data);

    if ( err != KErrNone )
        {
        TRACES(RDebug::Print( _L("CSysApMediatorObserver::ShutdownAnimationSkipped() - Error raising event: %d"), err));
        }

    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------
// CSysApMediatorObserver::SyncShutdownAnimation()
// ---------------------------------------------------------
TInt CSysApMediatorObserver::SyncShutdownAnimation()
    {
    return IssueCommand( SecondaryDisplay::ECmdStartupSync, SecondaryDisplay::EStartShutdownAnimation );
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


//  End of File
