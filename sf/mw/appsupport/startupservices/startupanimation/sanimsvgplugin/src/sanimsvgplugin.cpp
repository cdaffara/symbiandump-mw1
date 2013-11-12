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
* Description:  Implementation of CSAnimSvgPlugin class
*
*/


#include <SVGEngineInterfaceImpl.h>

#include "sanimsvgplugin.h"
#include "assert.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimSvgPlugin* CSAnimSvgPlugin::NewL( TAny* aConstructionParameters )
    {
    FUNC_LOG;

    CSAnimSvgPlugin* self =
        new( ELeave ) CSAnimSvgPlugin( aConstructionParameters );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::~CSAnimSvgPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimSvgPlugin::~CSAnimSvgPlugin()
    {
    FUNC_LOG;

    delete iImage;
    delete iMask;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPlugin::Load(
    RFs& /*aFs*/,
    const TDesC& aFileName,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;
    INFO_1( "File name: %S", &aFileName );

    delete iEngine;
    iEngine = NULL;
    delete iMask;
    iMask = NULL;
    delete iImage;
    iImage = NULL;

    TRAPD( errorCode, LoadL( aFileName ) );
    ERROR( errorCode, "Failed to load image file" );
    SetClientRequest( aStatus );
    CompleteClientRequest( errorCode );
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPlugin::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;
    ASSERT_TRACE( iEngine, SAnimPanic::ENotInitialized );

    SetClientRequest( aStatus );

    TBool isAnimation = iEngine->SvgHasAnimation( iEngine->SvgDocument() );
    if ( isAnimation )
        {
        INFO( "SVG animation" );

        iEngine->AddAnimationListener( this );
        }

    MSvgError* error = NULL;
    iEngine->Start( error );
    TInt errorCode = SvgToSymbianErr( error );
    if ( errorCode != KErrNone )
        {
        CompleteClientRequest( errorCode );
        }
    else if ( !isAnimation )
        {
        CompleteClientRequest( KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::Cancel
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPlugin::Cancel()
    {
    FUNC_LOG;

    CompleteClientRequest( KErrCancel );
    if ( iEngine )
        {
        iEngine->Stop();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::BackroundColour
//
// ---------------------------------------------------------------------------
//
TRgb CSAnimSvgPlugin::BackroundColour() const
    {
    FUNC_LOG;

    return KRgbWhite;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPlugin::UpdateScreen()
    {
    FUNC_LOG;

    if ( iImage && iMask )
        {
        iEngine->GenerateMask( iMask );
        iObserver.UpdateScreen( *iImage, *iMask );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::AnimationStarted
//
// ---------------------------------------------------------------------------
//
TBool CSAnimSvgPlugin::AnimationStarted( TBool /*isAnimationIndefinite*/ )
    {
    FUNC_LOG;
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::AnimationPaused
//
// ---------------------------------------------------------------------------
//
TBool CSAnimSvgPlugin::AnimationPaused()
    {
    FUNC_LOG;
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::AnimationEnded
//
// ---------------------------------------------------------------------------
//
TBool CSAnimSvgPlugin::AnimationEnded()
    {
    FUNC_LOG;

    CompleteClientRequest( KErrNone );
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::CSAnimSvgPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimSvgPlugin::CSAnimSvgPlugin( TAny* aConstructionParameters )
  : CSAnimSvgPluginBase( aConstructionParameters )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::ConstructL
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPlugin::ConstructL()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPlugin::LoadL
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPlugin::LoadL( const TDesC& aFileName )
    {
    FUNC_LOG;

    iImage = new ( ELeave ) CFbsBitmap();
    User::LeaveIfError( iImage->Create( iSize, iDisplayMode ) );
    iMask = new ( ELeave ) CFbsBitmap();
    User::LeaveIfError( iMask->Create( iSize, EGray256 ) );

    iEngine = CSvgEngineInterfaceImpl::NewL( iImage, this, iFontSpec );
    iEngine->SetBackgroundColor( KRgbWhite.Internal() );
    iEngine->SetDRMMode( EFalse );

    TInt errorCode = SvgToSymbianErr( iEngine->Load( aFileName ) );
    ERROR( errorCode, "Failed to load SVG file" );
    User::LeaveIfError( errorCode );
    }
