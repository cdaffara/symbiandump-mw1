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
* Description:  Implementation of CSAnimEngineImpl class
*
*/


#include "sanimimageplugin.h"
#include "sanimtoneplugin.h"

#include "sanimengineimpl.h"
#include "sanimpluginctrl.h"
#include "trace.h"

/** IF UID for Startup Image Plug-in API. */
const TUint32 KImagePlugInApiUid = 0x2000B118;

/** IF UID for Startup Tone Plug-in API. */
const TUint32 KTonePlugInApiUid = 0x2000B119;

// ======== LOCAL FUNCTIONS ========

static TInt PluginCallBack( TAny* aPtr )
    {
    static_cast<CSAnimEngineImpl*>( aPtr )->PluginFinished();
    return KErrNone;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimEngineImpl::CSAnimEngineImpl
//
// ---------------------------------------------------------------------------
//
CSAnimEngineImpl::CSAnimEngineImpl( RFs& aFs, MSAnimObserver& aObserver )
  : iFs( aFs ),
    iObserver( aObserver )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::~CSAnimEngineImpl
//
// ---------------------------------------------------------------------------
//
CSAnimEngineImpl::~CSAnimEngineImpl()
    {
    FUNC_LOG;

    Cancel();

    delete iTonePluginCtrl;
    delete iImagePluginCtrl;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::PluginFinished
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::PluginFinished()
    {
    FUNC_LOG;

    if ( iImagePluginCtrl && iTonePluginCtrl )
        {
        TInt imgSuccessCode = iImagePluginCtrl->SuccessCode();
        TInt toneSuccessCode = iTonePluginCtrl->SuccessCode();
        if ( imgSuccessCode != KRequestPending &&
             toneSuccessCode != KRequestPending )
            {
            if ( imgSuccessCode != KErrNone )
                {
                CompleteClientRequest( imgSuccessCode );
                }
            else
                {
                CompleteClientRequest( toneSuccessCode );
                }
            }
        // else wait until next call
        }
    else if ( iImagePluginCtrl )
        {
        CompleteClientRequest( iImagePluginCtrl->SuccessCode() );
        }
    else if ( iTonePluginCtrl )
        {
        CompleteClientRequest( iTonePluginCtrl->SuccessCode() );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::SetImageProperties
//
// ---------------------------------------------------------------------------
//
TInt CSAnimEngineImpl::SetImageProperties(
    const TDesC& aImageFileName,
    const TDisplayMode aDisplayMode,
    const TSize& aSize,
    const TTimeIntervalMicroSeconds32& aFrameDelay,
    const TBool aScalingEnabled,
    const TInt aRepeatCount )
    {
    FUNC_LOG;

    TInt errorCode = KErrNotReady;
    if ( ( !iImagePluginCtrl || !( iImagePluginCtrl->IsActive() ) ) &&
         ( !iTonePluginCtrl || !( iTonePluginCtrl->IsActive() ) ) )
        {
        delete iImagePluginCtrl;
        iImagePluginCtrl = NULL;

        errorCode = KErrNone;
        if ( aImageFileName.Length() > 0 )
            {
            TRAP( errorCode, iImagePluginCtrl = CSAnimPluginCtrl::NewL(
                aImageFileName, KImagePlugInApiUid, &iObserver ) );
            ERROR_1( errorCode, "Failed to create image plug-in for file '%S'", &aImageFileName );
            }

        if ( iImagePluginCtrl )
            {
            CSAnimImagePlugin& plugin =
                static_cast<CSAnimImagePlugin&>( iImagePluginCtrl->Plugin() );
            plugin.SetDisplayMode( aDisplayMode );
            plugin.SetSize( aSize );
            plugin.SetFrameDelay( aFrameDelay );
            plugin.SetScalingEnabled( aScalingEnabled );
            plugin.SetRepeatCount( aRepeatCount );
            }
        }

    return errorCode;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::SetToneProperties
//
// ---------------------------------------------------------------------------
//
TInt CSAnimEngineImpl::SetToneProperties(
    const TDesC& aToneFileName,
    const TInt aVolume,
    const TTimeIntervalMicroSeconds& aVolumeRamp,
    const TInt aRepeatCount )
    {
    FUNC_LOG;

    TInt errorCode = KErrNotReady;
    if ( ( !iImagePluginCtrl || !( iImagePluginCtrl->IsActive() ) ) &&
         ( !iTonePluginCtrl || !( iTonePluginCtrl->IsActive() ) ) )
        {
        delete iTonePluginCtrl;
        iTonePluginCtrl = NULL;

        errorCode = KErrNone;
        if ( aToneFileName.Length() > 0 )
            {
            TRAP( errorCode, iTonePluginCtrl =
                CSAnimPluginCtrl::NewL( aToneFileName, KTonePlugInApiUid, NULL ) );
            ERROR_1( errorCode, "Failed to create tone plug-in for file '%S'", &aToneFileName );
            }

        if ( iTonePluginCtrl )
            {
            CSAnimTonePlugin& plugin =
                static_cast<CSAnimTonePlugin&>( iTonePluginCtrl->Plugin() );
            plugin.SetVolume( aVolume );
            plugin.SetVolumeRamp( aVolumeRamp );
            plugin.SetRepeatCount( aRepeatCount );
            }
        }

    return errorCode;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::Load( TRequestStatus& aStatus )
    {
    FUNC_LOG;

    if ( ( iImagePluginCtrl && iImagePluginCtrl->IsActive() ) ||
         ( iTonePluginCtrl && iTonePluginCtrl->IsActive() ) )
        {
        TRequestStatus* status = &aStatus;
        User::RequestComplete( status, KErrNotReady );
        }
    else if ( !iImagePluginCtrl && !iTonePluginCtrl )
        {
        TRequestStatus* status = &aStatus;
        User::RequestComplete( status, KErrNone );
        }
    else
        {
        SetClientRequest( aStatus );

        if ( iImagePluginCtrl )
            {
            iImagePluginCtrl->Load( iFs, TCallBack( PluginCallBack, this ) );
            }
        if ( iTonePluginCtrl )
            {
            iTonePluginCtrl->Load( iFs, TCallBack( PluginCallBack, this ) );
            }

        SetRequestPending(); // Only do it here, just before returning
        }
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::BackroundColour
//
// ---------------------------------------------------------------------------
//
TRgb CSAnimEngineImpl::BackroundColour() const
    {
    FUNC_LOG;

    if ( iImagePluginCtrl )
        {
        return ( static_cast<CSAnimImagePlugin&>(
            iImagePluginCtrl->Plugin() ) ).BackroundColour();
        }

    return TRgb();
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;

    if ( ( iImagePluginCtrl && iImagePluginCtrl->IsActive() ) ||
         ( iTonePluginCtrl && iTonePluginCtrl->IsActive() ) )
        {
        TRequestStatus* status = &aStatus;
        User::RequestComplete( status, KErrNotReady );
        }
    else if ( !iImagePluginCtrl && !iTonePluginCtrl )
        {
        TRequestStatus* status = &aStatus;
        User::RequestComplete( status, KErrNone );
        }
    else
        {
        User::ResetInactivityTime();
        SetClientRequest( aStatus );

        if ( iImagePluginCtrl )
            {
            iImagePluginCtrl->Start( TCallBack( PluginCallBack, this ) );
            }
        if ( iTonePluginCtrl )
            {
            iTonePluginCtrl->Start( TCallBack( PluginCallBack, this ) );
            }

        SetRequestPending(); // Only do it here, just before returning
        }
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::Cancel
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::Cancel()
    {
    FUNC_LOG;

    if ( iImagePluginCtrl )
        {
        iImagePluginCtrl->Cancel();
        }
    if ( iTonePluginCtrl )
        {
        iTonePluginCtrl->Cancel();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::SetClientRequest
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::SetClientRequest( TRequestStatus& aStatus )
    {
    FUNC_LOG;

    __ASSERT_ALWAYS( !iClientStatus, User::Invariant() );

    iClientStatus = &aStatus;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::SetRequestPending
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::SetRequestPending()
    {
    FUNC_LOG;

    if ( iClientStatus ) // It may already have been completed.
        {
        *iClientStatus = KRequestPending;
        }
    }


// ---------------------------------------------------------------------------
// CSAnimEngineImpl::CompleteClientRequest
//
// ---------------------------------------------------------------------------
//
void CSAnimEngineImpl::CompleteClientRequest( const TInt aStatusCode )
    {
    FUNC_LOG;

    if ( iClientStatus )
        {
        User::RequestComplete( iClientStatus, aStatusCode );
        iClientStatus = NULL;
        }
    }
