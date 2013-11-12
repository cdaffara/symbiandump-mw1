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
* Description:  Implementation of CSAnimMifPlugin class
*
*/


#include <SVGEngineInterfaceImpl.h>

#include "sanimmifplugin.h"
#include "assert.h"
#include "trace.h"

/** MIF header contents. */
class TMifHeader
    {
public:
    TInt32 iUID;
    TInt32 iVersion;
    TInt32 iArrayOffset;
    TInt32 iArrayLength;
    };

typedef TPckgBuf<TMifHeader> TMifHeaderPckg;

/** Icon offset element from MIF file. */
class TIconOffsetElement
    {
public:
    TInt32 iOffset;
    TInt32 iLength;
    };

typedef TPckgBuf<TIconOffsetElement> TIconOffsetElementPckg;

/** Icon header contents. */
class TIconHeader
    {
public:
    TInt32 iUID;
    TInt32 iVersion;
    TInt32 iDataOffset;
    TInt32 iDataLength;
    TInt32 iType;
    TInt32 iDepth;
    TInt32 iAnimated;
    TInt32 iMaskDepth;
    };

typedef TPckgBuf<TIconHeader> TIconHeaderPckg;

/** Type identifier for SVG. */
const TInt KSvgType = 1;

/** Number of array elements one frame occupies (bitmap & mask = 2). */
const TInt KArrElementsPerFrame = 2;

/** Step through icon array with this granularity. */
const TInt KIconArrStep = 2;

// ======== LOCAL FUNCTIONS ========

static TInt TimerCallBack( TAny* aPtr )
    {
    FUNC_LOG;

    static_cast<CSAnimMifPlugin*>( aPtr )->TimerExpired();
    return KErrNone;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimMifPlugin::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimMifPlugin* CSAnimMifPlugin::NewL( TAny* aConstructionParameters )
    {
    FUNC_LOG;

    CSAnimMifPlugin* self =
        new( ELeave ) CSAnimMifPlugin( aConstructionParameters );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::~CSAnimMifPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimMifPlugin::~CSAnimMifPlugin()
    {
    FUNC_LOG;

    delete iDummy;
    delete iTimer;
    iFrames.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::TimerExpired
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::TimerExpired()
    {
    FUNC_LOG;

    if ( iCurrentFrame < iFrames.Count() - KArrElementsPerFrame )
        {
        iCurrentFrame += KArrElementsPerFrame; // Each other array element is a mask
        RefreshView();
        }
    else
        {
        iTimer->Cancel();
        CompleteClientRequest( KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::Load(
    RFs& aFs,
    const TDesC& aFileName,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;
    INFO_1( "File name: %S", &aFileName );

    iTimer->Cancel();
    iFrames.ResetAndDestroy();
    iCurrentFrame = -1;

    TRAPD( errorCode, LoadL( aFs, aFileName ) );
    ERROR( errorCode, "Failed to load image file" );
    SetClientRequest( aStatus );
    CompleteClientRequest( errorCode );
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;
    ASSERT_TRACE( iEngine, SAnimPanic::ENotInitialized );
    ASSERT_TRACE( !( iTimer->IsActive() ), SAnimPanic::EInternalError );

    SetClientRequest( aStatus );

    iCurrentFrame = 0;
    if ( iCurrentFrame < iFrames.Count() - 1 ) // Last one is a mask
        {
        RefreshView();
        iTimer->Start(
            iFrameDelay, iFrameDelay, TCallBack( TimerCallBack, this ) );
        }
    else
        {
        CompleteClientRequest( KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::Cancel
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::Cancel()
    {
    FUNC_LOG;

    CompleteClientRequest( KErrCancel );

    iTimer->Cancel();
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::BackroundColour
//
// ---------------------------------------------------------------------------
//
TRgb CSAnimMifPlugin::BackroundColour() const
    {
    FUNC_LOG;

    return KRgbWhite;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::UpdateScreen()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::CSAnimMifPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimMifPlugin::CSAnimMifPlugin( TAny* aConstructionParameters )
  : CSAnimSvgPluginBase( aConstructionParameters ),
    iCurrentFrame( -1 )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::ConstructL
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::ConstructL()
    {
    FUNC_LOG;

    iTimer = CPeriodic::NewL( CActive::EPriorityHigh );
    iDummy = new ( ELeave ) CFbsBitmap();
    User::LeaveIfError( iDummy->Create( TSize( 0, 0 ), EGray256 ) );
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::LoadL
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::LoadL( RFs& aFs, const TDesC& aFileName )
    {
    FUNC_LOG;

    delete iEngine;
    iEngine = NULL;
    iEngine = CSvgEngineInterfaceImpl::NewL( iDummy, this, iFontSpec );
    iEngine->SetBackgroundColor( KRgbWhite.Internal() );
    iEngine->SetDRMMode( EFalse );

    RFile file;
    CleanupClosePushL( file );
    User::LeaveIfError( file.Open(
        aFs, aFileName, EFileRead | EFileShareReadersOnly ) );

    TMifHeaderPckg header;
    TInt errorCode = file.Read( header );
    ERROR( errorCode, "CSAnimMifPlugin::LoadL: failed to read file" );
    INFO_2( "CSAnimMifPlugin::LoadL: arr pos %d, arr length %d",
             header().iArrayOffset, header().iArrayLength );
    User::LeaveIfError( errorCode );
    TInt fileSize( 0 );
    errorCode = file.Size( fileSize );
    ERROR( errorCode, "CSAnimMifPlugin::LoadL: failed to read file size" );
    User::LeaveIfError( errorCode );

    if ( header().iArrayOffset < 0 || header().iArrayLength <= 0 ||
         header().iArrayOffset + header().iArrayLength <= 0 ||
         header().iArrayOffset + header().iArrayLength > fileSize )
        {
        User::Leave( KErrCorrupt );
        }

    ReadIconArrayL( file, header().iArrayOffset, header().iArrayLength );

    CleanupStack::PopAndDestroy( &file );
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::ReadIconArrayL
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::ReadIconArrayL(
    RFile& aFile,
    const TInt32 aPosition,
    const TInt32 aLength )
    {
    FUNC_LOG;

    const TInt elementSize = sizeof( TIconOffsetElementPckg ) / 2;
    INFO_1( "Reserving %d bytes", aLength * elementSize );

    HBufC8* iconArray = HBufC8::NewLC( aLength * elementSize );
    TPtr8 iconArrayPtr = iconArray->Des();
    TInt errorCode = aFile.Read( aPosition, iconArrayPtr, aLength * elementSize );
    ERROR( errorCode, "CSAnimMifPlugin::ReadIconArrayL failed to read from file" );
    User::LeaveIfError( errorCode );

    INFO_1( "Read %d bytes", iconArrayPtr.Length() );

    for ( TInt i = 0; i < ( iconArrayPtr.Length() / elementSize ); i += KIconArrStep )
        {
        INFO_1( "Read icon %d", i );

        ReadIconOffsetElementL(
            aFile, iconArrayPtr.MidTPtr( i * elementSize, elementSize ) );
        }

    CleanupStack::PopAndDestroy( iconArray );
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::ReadIconOffsetElementL
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::ReadIconOffsetElementL(
    RFile& aFile,
    const TDesC8& aData )
    {
    FUNC_LOG;

    TIconOffsetElementPckg element;
    element.Copy( aData );
    INFO_2( "Offset %d, length %d", element().iOffset, element().iLength );

    TIconHeaderPckg header;
    TInt errorCode = aFile.Read( element().iOffset, header );
    ERROR( errorCode, "CSAnimMifPlugin::ReadIconOffsetElementL failed to read from file" );
    User::LeaveIfError( errorCode );

    INFO_2( "Data pos %d, data length %d", header().iDataOffset, header().iDataLength );
    INFO_3( "Type %d, depth %d, mask depth %d", header().iType, header().iDepth, header().iMaskDepth );

    if ( header().iType != KSvgType )
        {
        ERROR_1( KErrNotSupported, "Unsupported file type: %d", header().iType );

        User::Leave( KErrNotSupported );
        }

    HBufC8* data = HBufC8::NewLC( header().iDataLength );
    TPtr8 dataPtr = data->Des();
    errorCode = aFile.Read( element().iOffset + header().iDataOffset, dataPtr );
    ERROR( errorCode, "CSAnimMifPlugin::ReadIconOffsetElementL failed to read from file" );
    User::LeaveIfError( errorCode );

    CFbsBitmap* mask = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( mask );
    User::LeaveIfError( mask->Create( iSize, EGray256 ) );

    CFbsBitmap* bmp = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( bmp );
    User::LeaveIfError(
        bmp->Create( iSize, static_cast<TDisplayMode>( header().iDepth ) ) );

    TInt handle( 0 );
    errorCode = SvgToSymbianErr( iEngine->PrepareDom( dataPtr, handle ) );
    ERROR( errorCode, "Failed to prepare DOM" );
    User::LeaveIfError( errorCode );

    errorCode = SvgToSymbianErr( iEngine->UseDom( handle, bmp, mask ) );
    ERROR( errorCode, "Failed to use DOM" );
    User::LeaveIfError( errorCode );

    iEngine->SetPreserveAspectRatio(
        NULL, ESvgPreserveAspectRatio_XmidYmid, ESvgMeetOrSlice_Meet, ETrue );

    MSvgError* error = NULL;
    iEngine->Start( error );
    errorCode = SvgToSymbianErr( error );
    ERROR( errorCode, "Failed to start engine" );
    User::LeaveIfError( errorCode );

    errorCode = SvgToSymbianErr( iEngine->UseDom( handle, NULL, NULL ) );
    ERROR( errorCode, "Failed to use DOM (empty)" );
    User::LeaveIfError( errorCode );

    iEngine->DeleteDom( handle );

    iFrames.AppendL( bmp );
    CleanupStack::Pop( bmp ); // It's now in the array.

    iFrames.AppendL( mask );
    CleanupStack::Pop( mask ); // It's now in the array.

    CleanupStack::PopAndDestroy( data );
    }


// ---------------------------------------------------------------------------
// CSAnimMifPlugin::RefreshView
//
// ---------------------------------------------------------------------------
//
void CSAnimMifPlugin::RefreshView()
    {
    FUNC_LOG;
    ASSERT_TRACE( iCurrentFrame < iFrames.Count() - 1, SAnimPanic::EInternalError );

    iObserver.UpdateScreen(
        *( iFrames[iCurrentFrame] ), *( iFrames[iCurrentFrame + 1] ) );
    }
