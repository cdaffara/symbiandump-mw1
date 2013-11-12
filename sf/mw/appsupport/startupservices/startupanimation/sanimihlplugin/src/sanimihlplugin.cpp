/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSAnimIhlPlugin class
*
*/


#include "sanimihlplugin.h"
#include "sanimihlctrl.h"
#include "assert.h"
#include "trace.h"

// ======== LOCAL FUNCTIONS ========

static TInt CtrlCallBack( TAny* aPtr )
    {
    FUNC_LOG;

    static_cast<CSAnimIhlPlugin*>( aPtr )->RequestComplete();
    return KErrNone;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimIhlPlugin* CSAnimIhlPlugin::NewL( TAny* aConstructionParameters )
    {
    FUNC_LOG;

    CSAnimIhlPlugin* self =
        new( ELeave ) CSAnimIhlPlugin( aConstructionParameters );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::~CSAnimIhlPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimIhlPlugin::~CSAnimIhlPlugin()
    {
    FUNC_LOG;

    delete iIhlCtrl;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::RequestComplete
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlPlugin::RequestComplete()
    {
    FUNC_LOG;

    CompleteClientRequest( iIhlCtrl->SuccessCode() );
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlPlugin::Load(
    RFs& aFs,
    const TDesC& aFileName,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;
    INFO_1( "File name: %S", &aFileName );

    SetClientRequest( aStatus );

    TRAPD( errorCode, LoadL( aFs, aFileName ) );
    ERROR( errorCode, "Failed to load animation" );
    if ( errorCode != KErrNone )
        {
        CompleteClientRequest( errorCode );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlPlugin::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;
    ASSERT_TRACE( iIhlCtrl, SAnimPanic::ENotInitialized );

    SetClientRequest( aStatus );
    iIhlCtrl->Play( TCallBack( CtrlCallBack, this ) );
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::Cancel
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlPlugin::Cancel()
    {
    FUNC_LOG;

    if ( iIhlCtrl )
        {
        iIhlCtrl->Stop();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::BackroundColour
//
// ---------------------------------------------------------------------------
//
TRgb CSAnimIhlPlugin::BackroundColour() const
    {
    FUNC_LOG;

    if ( iIhlCtrl )
        {
        return iIhlCtrl->BackroundColour();
        }
    else return TRgb();
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::CSAnimIhlPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimIhlPlugin::CSAnimIhlPlugin( TAny* aConstructionParameters )
  : CSAnimImagePlugin( aConstructionParameters ),
    iIhlCtrl( NULL )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPlugin::LoadL
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlPlugin::LoadL( RFs& aFs, const TDesC& aFileName )
    {
    FUNC_LOG;

    delete iIhlCtrl;
    iIhlCtrl = NULL;

    iIhlCtrl = CSAnimIhlCtrl::NewL(
        iObserver, aFs, aFileName, iDisplayMode, iSize, iFrameDelay,
        iScalingEnabled );
    iIhlCtrl->Load( TCallBack( CtrlCallBack, this ) );
    }
