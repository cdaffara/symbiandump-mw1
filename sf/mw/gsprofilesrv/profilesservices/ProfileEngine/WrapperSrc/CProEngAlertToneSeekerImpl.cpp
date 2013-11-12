/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngAlertToneSeekerImpl
*
*/



// INCLUDE FILES
#include    "CProEngAlertToneSeekerImpl.h"
#include    "CProEngPostFilter.h"
#include    <MCLFItemListModel.h>
#include    <MCLFContentListingEngine.h>
#include    <MCLFItem.h>
#include    <ContentListingFactory.h>
#include    <f32file.h>  // RFs, TParse
#include    <pathinfo.h>
#include    <MProEngAlertToneSeekerObserver.h>

namespace
    {
    // Constants
    const TInt KProEngRomFileArrayGranularity( 40 );
    const TInt KProEngToneListGranularity( 80 );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::CProEngAlertToneSeekerImpl
// -----------------------------------------------------------------------------
//
CProEngAlertToneSeekerImpl::CProEngAlertToneSeekerImpl()
    {
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::ConstructL()
    {
    iContentListingEngine = ContentListingFactory::NewContentListingEngineLC(); // CSI: 49 # iContentLisingEngine is popped out of the stack immediately
    CleanupStack::Pop(); // iContentListingEngine
    iPostFilter = CProEngPostFilter::NewL();
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngAlertToneSeekerImpl* CProEngAlertToneSeekerImpl::NewL()
    {
    CProEngAlertToneSeekerImpl* self =
        new ( ELeave ) CProEngAlertToneSeekerImpl();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CProEngAlertToneSeekerImpl::~CProEngAlertToneSeekerImpl()
    {
    delete iPostFilter;
    delete iRomFiles;
    delete iModel;
    delete iContentListingEngine;
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::FetchAlertToneListL
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::FetchAlertToneListL(
        MProEngAlertToneSeekerObserver& aObserver )
    {
    iObserver = &aObserver;

    if( !iRomFiles )
        {
        CreateRomFileListL();
        }

    CreateListModelL();

    iModel->RefreshL(); // asynchronous
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::CancelFetch
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::CancelFetch()
    {
    if( iModel )
        {
        iModel->CancelRefresh();
        }
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::FetchAlertToneListL
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::HandleOperationEventL(
        TCLFOperationEvent aOperationEvent,
        TInt aError )
    {
    // We are only interested of refresh complete, ignore other events:
    if( aOperationEvent != ECLFRefreshComplete )
        {
        return;
        }

    if( aError != KErrNone )
        {
        iObserver->HandleError( aError );
        return;
        }

    CDesCArray* toneList =
        new ( ELeave ) CDesCArraySeg( KProEngToneListGranularity );
    CleanupStack::PushL( toneList );

    // copy the files in ROM
    TInt count( iRomFiles->Count() );
    for( TInt i( 0 ); i<count; ++i )
        {
        toneList->AppendL( ( *iRomFiles )[i] );
        }

    // copy the files found by Content Listing Framework
    count = iModel->ItemCount();
    for( TInt i( 0 ); i<count; ++i )
        {
        TPtrC fileNameAndPath;
        aError = iModel->Item( i ).GetField( ECLFFieldIdFileNameAndPath,
                                             fileNameAndPath );
        if( aError )
            {
            break;
            }

        toneList->AppendL( fileNameAndPath );
        }

    if( aError != KErrNone )
        {
        CleanupStack::PopAndDestroy( toneList );
        iObserver->HandleError( aError );
        return;
        }

    CleanupStack::Pop( toneList );

    TRAP( aError,
          iObserver->HandleAlertToneListCompletedL( toneList ) );
    if( aError != KErrNone )
        {
        iObserver->HandleError( aError );
        }
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::CreateListModelL
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::CreateListModelL()
    {
    if( iModel )
        {
        delete iModel;
        iModel = NULL;
        }

    iModel = iContentListingEngine->CreateListModelLC( *this );
    CleanupStack::Pop(); // iModel

    // array for wanted media types
    RArray<TInt> array;
    CleanupClosePushL( array );

    array.AppendL( ECLFMediaTypeMusic );
    array.AppendL( ECLFMediaTypeSound );
#ifdef RD_VIDEO_AS_RINGING_TONE
    array.AppendL( ECLFMediaTypeVideo );
#endif  // RD_VIDEO_AS_RINGING_TONE

    iModel->SetWantedMediaTypesL( array.Array() );
    iModel->SetPostFilter( iPostFilter );

    CleanupStack::PopAndDestroy( &array );
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::CreateRomFileListL
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::CreateRomFileListL()
    {
    iRomFiles = new ( ELeave ) CDesCArraySeg( KProEngRomFileArrayGranularity );

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    RBuf pathBuf;
    CleanupClosePushL( pathBuf );
    pathBuf.CreateL( KMaxFileName );
    pathBuf.Append( PathInfo::RomRootPath() );
    TInt rootPathLength( pathBuf.Length() );

    TParse* parse = new ( ELeave ) TParse;
    CleanupStack::PushL( parse );

    pathBuf.Append( PathInfo::DigitalSoundsPath() );
    AppendRomFilesL( fs, pathBuf, *parse );

    pathBuf.SetLength( rootPathLength );
    pathBuf.Append( PathInfo::SimpleSoundsPath() );
    AppendRomFilesL( fs, pathBuf, *parse );
    
    CleanupStack::PopAndDestroy( 3, &fs );
    }

// -----------------------------------------------------------------------------
// CProEngAlertToneSeekerImpl::AppendRomFilesL
// -----------------------------------------------------------------------------
//
void CProEngAlertToneSeekerImpl::AppendRomFilesL( RFs& aFs, const TDesC& aPath,
                                                  TParse& aParse )
    {
    CDir* dir;
    TInt err( aFs.GetDir( aPath, KEntryAttNormal, ESortNone, dir ) );

    if( ( err != KErrNone ) && ( err != KErrPathNotFound ) )
        {
        User::Leave( err );
        }

    if( err == KErrNone )
        {
        CleanupStack::PushL( dir );

        TInt count( dir->Count() );
        for( TInt i( 0 ); i<count; ++i )
            {
            // concatenate the directory path and the file name
            aParse.Set( aPath, &( ( *dir )[i].iName ), NULL );

            iRomFiles->AppendL( aParse.FullName() );
            }

        CleanupStack::PopAndDestroy( dir );
        }
    }

//  End of File

