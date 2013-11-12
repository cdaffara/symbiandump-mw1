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
* Description:  Implementation of CSAnimPluginCtrl class
*
*/


#include <ecom/ecom.h>

#include "sanimpluginctrl.h"
#include "sanimplugin.h"
#include "assert.h"
#include "trace.h"

/** Maximum length for any file name extension (actually 4 should be enough). */
const TInt KMaxFileExtLength = 6;
/** Buffer long enough to hold any file name extension. */
typedef TBuf<KMaxFileExtLength> TFileNameExtension;
typedef TBuf8<KMaxFileExtLength> TFileNameExtension8;

// ======== LOCAL FUNCTIONS ========

static void CleanupRImplInfoPtrArray( TAny* aPtr )
    {
    static_cast<RImplInfoPtrArray*>( aPtr )->ResetAndDestroy();
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimPluginCtrl* CSAnimPluginCtrl::NewL(
    const TDesC& aFileName,
    const TUint32 aIfUid,
    TAny* aConstructionParameters )
    {
    FUNC_LOG;

    CSAnimPluginCtrl* self = new( ELeave ) CSAnimPluginCtrl;
    CleanupStack::PushL( self );
    self->ConstructL( aFileName, aIfUid, aConstructionParameters );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::~CSAnimPluginCtrl
//
// ---------------------------------------------------------------------------
//
CSAnimPluginCtrl::~CSAnimPluginCtrl()
    {
    FUNC_LOG;

    Cancel();

    delete iPlugin;
    REComSession::DestroyedImplementation( iDtorIDKey );

    delete iFileName;
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::Load( RFs& aFs, const TCallBack& aCallBack )
    {
    FUNC_LOG;
    ASSERT_TRACE( !IsActive(), SAnimPanic::EInternalError );

    iSuccessCode = KRequestPending;
    iCallBack = aCallBack;

    TIMESTAMP( "Load - start" );

    iPlugin->Load( aFs, *iFileName, iStatus );
    SetActive();
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::Start( const TCallBack& aCallBack )
    {
    FUNC_LOG;
    ASSERT_TRACE( !IsActive(), SAnimPanic::EInternalError );


    if ( !iReadyToStart )
        {
        iSuccessCode = KErrNotReady;
        aCallBack.CallBack();
        }
    else
        {
        iSuccessCode = KRequestPending;
        iCallBack = aCallBack;

        iPlugin->Start( iStatus );
        SetActive();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::SuccessCode
//
// ---------------------------------------------------------------------------
//
TInt CSAnimPluginCtrl::SuccessCode() const
    {
    return iSuccessCode;
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::Plugin
//
// ---------------------------------------------------------------------------
//
CSAnimPlugin& CSAnimPluginCtrl::Plugin()
    {
    return *iPlugin;
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::DoCancel
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::DoCancel()
    {
    FUNC_LOG;

    iPlugin->Cancel();
    iSuccessCode = KErrCancel;
    iCallBack.CallBack();
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::RunL
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::RunL()
    {
    FUNC_LOG;
    INFO_1( "Plug-in command completed with %d", iStatus.Int() );

    if ( !iReadyToStart && iStatus.Int() == KErrNone )
        {
        TIMESTAMP( "Load - end" );

        iReadyToStart = ETrue;
        }

    iSuccessCode = iStatus.Int();
    iCallBack.CallBack();
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::CSAnimPluginCtrl
//
// ---------------------------------------------------------------------------
//
CSAnimPluginCtrl::CSAnimPluginCtrl()
  : CActive( EPriorityStandard ),
    iReadyToStart( EFalse )
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::ConstructL
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::ConstructL(
    const TDesC& aFileName,
    const TUint32 aIfUid,
    TAny* aConstructionParameters )
    {
    FUNC_LOG;

    iFileName = aFileName.AllocL();

    TFileNameExtension ext;
    GetExtension( aFileName, ext );
    if ( ext.Length() > 0 )
        {
        FindPluginByExtensionL( ext, aIfUid, aConstructionParameters );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::GetExtension
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::GetExtension( const TDesC& aFileName, TDes& aExtension )
    {
    FUNC_LOG;

    aExtension = KNullDesC;

    TInt pos = aFileName.LocateReverse( '.' );
    if ( pos >= 0 && aFileName.Length() - pos - 1 <= aExtension.MaxLength() )
        {
        aExtension = aFileName.Right( aFileName.Length() - pos - 1 );
        }

    aExtension.LowerCase();

    INFO_1( "File extension: '%S'", &aExtension );
    }


// ---------------------------------------------------------------------------
// CSAnimPluginCtrl::FindPluginByExtensionL
//
// ---------------------------------------------------------------------------
//
void CSAnimPluginCtrl::FindPluginByExtensionL(
    const TDesC& aExtension,
    const TUint32 aIfUid,
    TAny* aConstructionParameters )
    {
    FUNC_LOG;
    ASSERT_TRACE( aExtension.Length() > 0, SAnimPanic::EInternalError );

    TEComResolverParams params;
    params.SetGenericMatch( ETrue );
    TFileNameExtension8 ext;
    ext.Copy( aExtension );
    params.SetDataType( ext );

    RImplInfoPtrArray implInfoArray;
    CleanupStack::PushL(
        TCleanupItem( CleanupRImplInfoPtrArray, &implInfoArray ) );
    REComSession::ListImplementationsL(
        TUid::Uid( aIfUid ),
        params,
        implInfoArray );
    if ( implInfoArray.Count() > 0 )
        {
        TUid implUid = implInfoArray[ 0 ]->ImplementationUid();
        INFO_2( "%d plug-ins found, using 0x%08x", implInfoArray.Count(), implUid );

        iPlugin = static_cast<CSAnimPlugin*>(
            REComSession::CreateImplementationL(
                implUid, iDtorIDKey, aConstructionParameters ) );
        }

    CleanupStack::PopAndDestroy( &implInfoArray );
    if ( !iPlugin )
        {
        User::Leave( KErrNotFound );
        }
    }
