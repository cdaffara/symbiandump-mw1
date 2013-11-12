/*
* Copyright (c) 2007,2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CStartupAnimationWrapper class
*
*/


#include <apgtask.h>
#include <e32debug.h>
#include <e32property.h>
#include <startupdomainpskeys.h>

#include "startupanimationwrapper.h"
#include "startup.hrh"
#include "StartupDefines.h"
#include "startupview.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::NewL
//
// ---------------------------------------------------------------------------
//
CStartupAnimationWrapper* CStartupAnimationWrapper::NewL( CStartupView& aView )
    {
    TRACES("CStartupAnimationWrapper::NewL()");

    CStartupAnimationWrapper* self =
        new( ELeave ) CStartupAnimationWrapper( aView );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    TRACES("CStartupAnimationWrapper::NewL(): End");
    return self;
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::~CStartupAnimationWrapper
//
// ---------------------------------------------------------------------------
//
CStartupAnimationWrapper::~CStartupAnimationWrapper()
    {
    TRACES("CStartupAnimationWrapper::~CStartupAnimationWrapper()");

    Cancel();
    if ( iCallBackCaller )
        {
        iCallBackCaller->Cancel();
        }
    delete iCallBackCaller;
    delete iCtrl;

    TRACES("CStartupAnimationWrapper::~CStartupAnimationWrapper(): end");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::PreLoad
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::PreLoad(
    const TRect& aRect,
    const CCoeControl& aContainer,
    const CSAnimStartupCtrl::TAnimationParams& aParams,
    const TBool aPlayDefaultBeep,
    const TInt aSyncCommand )
    {
    TRACES1("CStartupAnimationWrapper::PreLoad(), iState = %d", iState);

    if ( iState == EIdle ) // This component will not be re-used if first animation is cancelled
        {
        TRAPD( errorCode,
            LoadL( aRect, aContainer, aParams, aPlayDefaultBeep, aSyncCommand ) );
        if ( errorCode != KErrNone )
            {
            TRACES1("CStartupAnimationWrapper::LoadL() leave code = %d", errorCode);

            DestroyCtrl();
            }
        }

    TRACES("CStartupAnimationWrapper::PreLoad(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::Play
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::Play(
    const TRect& aRect,
    const CCoeControl& aContainer,
    const CSAnimStartupCtrl::TAnimationParams& aParams,
    const TBool aPlayDefaultBeep,
    const TInt aSyncCommand,
    const TCallBack& aCallBack )
    {
    TRACES("CStartupAnimationWrapper::Play()");

    iCallBackCaller->Set( aCallBack );

    if ( iState == ELoading )
        {
        iPlayImmediately = ETrue;
        }
    else if ( iState == EReady )
        {
        StartPlaying();
        }
    else if ( iState == EIdle )
        {
        iPlayImmediately = ETrue;

        TRAPD( errorCode,
            LoadL( aRect, aContainer, aParams, aPlayDefaultBeep, aSyncCommand ) );
        if ( errorCode != KErrNone )
            {
            TRACES1("CStartupAnimationWrapper::LoadL() leave code = %d", errorCode);

            DestroyCtrl();
            iCallBackCaller->CallBack();
            }
        }

    TRACES("CStartupAnimationWrapper::Play(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::WasCancelled
//
// ---------------------------------------------------------------------------
//
TBool CStartupAnimationWrapper::WasCancelled() const
    {
    return ( iState == ECancelled );
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::DoCancel
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::DoCancel()
    {
    TRACES("CStartupAnimationWrapper::DoCancel()");

    iState = ECancelled;
    DestroyCtrl();
    iCallBackCaller->CallBack();

    TRACES("CStartupAnimationWrapper::DoCancel(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::RunL
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::RunL()
    {
    TRACES1("CStartupAnimationWrapper::RunL(): iStatus = %d", iStatus.Int());

    if ( iState == ELoading && iStatus.Int() == KErrNone )
        {
        iState = EReady;

        if ( iPlayImmediately )
            {
            StartPlaying();
            }
        }
    else
        {
        iState = ( iStatus.Int() == KErrCancel ? ECancelled : EIdle );
        DestroyCtrl();
        iCallBackCaller->CallBack();
        }

    TRACES("CStartupAnimationWrapper::RunL(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::CStartupAnimationWrapper
//
// ---------------------------------------------------------------------------
//
CStartupAnimationWrapper::CStartupAnimationWrapper( CStartupView& aView )
  : CActive( EPriorityStandard ),
    iView( aView ),
    iState( EIdle ),
    iHasContent( EFalse )
    {
    TRACES("CStartupAnimationWrapper::CStartupAnimationWrapper()");

    CActiveScheduler::Add( this );

    TRACES("CStartupAnimationWrapper::CStartupAnimationWrapper(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::ConstructL
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::ConstructL()
    {
    TRACES("CStartupAnimationWrapper::ConstructL()");
    TRACES("CStartupAnimationWrapper::ConstructL(): End");

    iCallBackCaller = new ( ELeave ) CAsyncCallBack( EPriorityStandard );
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::LoadL
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::LoadL(
    const TRect& /*aRect*/,
    const CCoeControl& aContainer,
    const CSAnimStartupCtrl::TAnimationParams& aParams,
    const TBool aPlayDefaultBeep,
    const TInt aSyncCommand )
    {
    TRACES("CStartupAnimationWrapper::LoadL()");
    __ASSERT_DEBUG( !iCtrl, PANIC( EStartupPanicClassMemberVariableIsNotNull ) );
    __ASSERT_DEBUG( iState == EIdle, PANIC( EStartupInvalidInternalState ) );

    iCtrl = CSAnimStartupCtrl::NewL( iView.Rect(), aContainer );
    iView.SetComponent( *iCtrl );
    iState = ELoading;
    iCtrl->Load( aParams, aPlayDefaultBeep, aSyncCommand, iStatus );
    iHasContent = iCtrl->HasContent();
    SetActive();

    TRACES("CStartupAnimationWrapper::LoadL(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::StartPlaying
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::StartPlaying()
    {
    TRACES("CStartupAnimationWrapper::StartPlaying()");

    iState = EPlaying;
    RProperty::Set( KPSUidStartup, KPSSplashShutdown, ESplashShutdown );

    BringToForeground();

    iCtrl->Start( iStatus );
    SetActive();

    TRACES("CStartupAnimationWrapper::StartPlaying(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::DestroyCtrl
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::DestroyCtrl()
    {
    TRACES("CStartupAnimationWrapper::DestroyCtrl()");

    if ( iCtrl )
        {
        iCtrl->Cancel();
        iView.RemoveComponent();
        delete iCtrl;
        iCtrl = NULL;
        }

    TRACES("CStartupAnimationWrapper::DestroyCtrl(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::HasContent
//
// ---------------------------------------------------------------------------
//
TBool CStartupAnimationWrapper::HasContent() const
    {
    return iHasContent;
    }


// ---------------------------------------------------------------------------
// CStartupAnimationWrapper::BringToForeground
//
// ---------------------------------------------------------------------------
//
void CStartupAnimationWrapper::BringToForeground()
    {
    TRACES("CStartupAnimationWrapper::BringToForeground()");

    TApaTask self( CCoeEnv::Static()->WsSession() );
    self.SetWgId( CCoeEnv::Static()->RootWin().Identifier() );
    self.BringToForeground();

    TRACES("CStartupAnimationWrapper::BringToForeground(): End");
    }
