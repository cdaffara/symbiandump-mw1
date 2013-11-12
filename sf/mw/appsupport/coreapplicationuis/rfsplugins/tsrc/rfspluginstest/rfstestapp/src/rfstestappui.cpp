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
* Description:  Implementation of CRfsTestAppUi class
*
*/


#include <akncommondialogsdynmem.h>
#include <aknwaitdialog.h>
#include <commondialogs.hrh>
#include <rfstestapp.rsg>
#include <aknlists.h>
#include <aknpopup.h>
#include <driveinfo.h>

#include "datacreator.h"
#include "rfstestappui.h"
#include "rfstestappcmds.hrh"
#include "rfstestview.h"
#include "trace.h"
#include "formatterrfsplugincommon.h"

// Default exclude list location
_LIT( KExcludeListOnC, "c:\\private\\100059C9\\excludelist.txt" );
_LIT( KDefaultSpecFile, "e:\\testdata\\testspec_folder_structure.spc" );
_LIT( KDefaultExcludeFile, "e:\\testdata\\testexclude_folder_structure.exc" );
_LIT( KDefaultAppSpecFile, "e:\\testdata\\appspec\\app1_spec.spc" );
_LIT( KDefaultAppExcludeFile, "e:\\testdata\\appspec\\app1_exclude_everything.exc" );
_LIT( KTitleExcludeFiles, "List of exclude files" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// DataCreatorCallBack
// ---------------------------------------------------------------------------
//
static TInt DataCreatorCallBack( TAny* aPtr )
    {
    static_cast<CRfsTestAppUi*>( aPtr )->DataCreated();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// AskPathL
// ---------------------------------------------------------------------------
//
static TBool AskPathL( TDes& aPath, TInt aResId )
    {
    FUNC_LOG;

    TParsePtr parse( aPath );
    TPtrC rootFolder = parse.DriveAndPath();
    INFO_1( "Root folder for opening test files: %S", &rootFolder );
    TBool ret = AknCommonDialogsDynMem::RunSelectDlgLD(
        AknCommonDialogsDynMem::EMemoryTypePhone |
        AknCommonDialogsDynMem::EMemoryTypeMMC,
        aPath,
        rootFolder,
        R_RFSTESTAPP_MEMORY_SELECTION,
        aResId );

    return ret;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CRfsTestAppUi::CRfsTestAppUi
// ---------------------------------------------------------------------------
//
CRfsTestAppUi::CRfsTestAppUi() : iView( NULL )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::ConstructL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::ConstructL()
    {
    FUNC_LOG;

    BaseConstructL();
    iView = CRfsTestView::NewL( ClientRect() );
    AddToStackL( iView );
    User::LeaveIfError( iFs.Connect() );
    iSystemDrive = iFs.GetSystemDriveChar();
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::CreateListBoxL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::CreateListBoxL()
    {
    iListBox = new( ELeave ) CAknSinglePopupMenuStyleListBox();
    iPopup = CAknPopupList::NewL( iListBox, R_AVKON_SOFTKEYS_OK_EMPTY );
    iPopup->SetTitleL( KTitleExcludeFiles );
    // Set up listbox
    iListBox->ConstructL( iPopup, EAknListBoxSelectionList | EAknListBoxLoopScrolling);
    // Create listbox model and give the model to the listbox
    iListModel = new( ELeave ) CDesCArraySeg( 20 );

    // Create scroll indicator
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame( )->SetScrollBarVisibilityL(
                    CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
    iListBox->Model( )->SetItemTextArray( iListModel );
    iListBox->Model( )->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
    }
    

// ---------------------------------------------------------------------------
// CRfsTestAppUi::~CRfsTestAppUi
// ---------------------------------------------------------------------------
//
CRfsTestAppUi::~CRfsTestAppUi()
    {
    FUNC_LOG;

    delete iDataCreator;
    ClearProgressBar();
    if ( iView ) RemoveFromStack( iView );
    delete iView;

    CleanListBox();
    iFs.Close();
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::CleanListBox
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::CleanListBox()
    {
    FUNC_LOG;

    delete iListModel;
    iListModel = NULL;
    delete iListBox;
    iListBox = NULL;
    delete ( CBase* ) iPopup;
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::HandleCommandL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::DataCreated()
    {
    FUNC_LOG;

    ClearProgressBar();
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::HandleCommandL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;
    INFO_1( "Command ID: %d", aCommand );

    switch ( aCommand )
        {
        case ESetupTestCmd:
            SetupTestL( KDefaultSpecFile, KDefaultExcludeFile, KExcludeListOnC );
            break;
        case ESetupAppTestCmd:
            SetupTestL( KDefaultAppSpecFile, KDefaultAppExcludeFile, KNullDesC );
            break;
        case EListListsCmd:
            ListListsL();
            break;
        case EAknSoftkeyExit:
        case EAknSoftkeyBack:
        case EEikCmdExit:
            {
            Exit();
            break;
            }
        default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::DialogDismissedL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::DialogDismissedL( TInt /*aButtonId*/ )
    {
    FUNC_LOG;

    if ( iDataCreator )
        {
        iDataCreator->Cancel();
        }
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::SetupTestL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::SetupTestL( const TDesC& aSpec, 
                                const TDesC& aExclude,
                                const TDesC& aExcludeTarget )
    {
    FUNC_LOG;

    HBufC* specFile = HBufC::NewLC( KMaxFileName );
    TPtr specFileDes = specFile->Des();
    specFileDes = aSpec;
    if ( AskPathL( specFileDes, R_RFSTESTAPP_SPECFILE_SELECTION ) )
        {
        HBufC* excludeFile = HBufC::NewLC( KMaxFileName );
        TPtr excludeFileDes = excludeFile->Des();
        excludeFileDes = aExclude;

        if ( AskPathL( excludeFileDes, R_RFSTESTAPP_EXCLUDELIST_SELECTION ) )
            {
            HBufC* excludeTargetFile;
            
            if( !aExcludeTarget.Compare(KNullDesC) )
                {
                TParsePtrC name( excludeFileDes );
                excludeTargetFile = HBufC::NewLC( 
                    KApplicationExcludeListPath().Length() + name.NameAndExt().Length() );
                TPtr fileDes = excludeTargetFile->Des();
                fileDes.Append( KApplicationExcludeListPath );
                fileDes.Append( name.NameAndExt() );
                fileDes[0] = iSystemDrive;
                }
            else
                {
                excludeTargetFile = KExcludeListOnC().AllocLC();
                }
            
            ClearProgressBar();
            iProgress = new ( ELeave ) CAknWaitDialog(
                reinterpret_cast< CEikDialog** >( &iProgress ), ETrue );
            iProgress->PrepareLC( R_RFSTESTAPP_PROGRESS );
            iProgress->SetCallback( this );
            iProgress->RunLD();

            delete iDataCreator;
            iDataCreator = NULL;
            iDataCreator = CDataCreator::NewL(
                *specFile,
                *excludeFile,
                *excludeTargetFile,
                TCallBack( DataCreatorCallBack, this ) );
            
            CleanupStack::PopAndDestroy( excludeTargetFile );
            }

        CleanupStack::PopAndDestroy( excludeFile );
        }
    CleanupStack::PopAndDestroy( specFile );
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::ListListsL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::ListListsL()
    {
    FUNC_LOG;

    HBufC* excludeFile = HBufC::NewLC( KMaxFileName );
    TPtr fileDes = excludeFile->Des();
    
    CreateListBoxL();

    // Append default exclude list file if exists    
    RFile file;
    fileDes.Append( iSystemDrive );
    fileDes.Append( KDriveDelimiter );
    fileDes.Append( KExcludeList );
    
    TInt err = file.Open( iFs, fileDes, EFileRead );
    if ( err == KErrNone )
        {
        file.Close();
        iListModel->AppendL( fileDes );
        }

    fileDes.Zero();
    fileDes.Append( KApplicationExcludeListPath );
    fileDes.Append( KApplicationExcludeListFileSearchPattern );
    ListsFilesOnDriveL( fileDes, KDefaultRom()[0] );
    fileDes.Zero();
    fileDes.Append( KApplicationExcludeListPath );
    fileDes.Append( KApplicationExcludeListFileSearchPattern );
    ListsFilesOnDriveL( fileDes, iSystemDrive );
    
    iListBox->HandleItemAdditionL( );
    iPopup->ExecuteLD( );
    iPopup = NULL;
    CleanListBox();

    CleanupStack::PopAndDestroy( excludeFile );
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::ListsFilesOnDriveL
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::ListsFilesOnDriveL( TPtr aBuf, TChar aDrive )
    {
    FUNC_LOG;
    
    CDir* dir = NULL;
    
    aBuf[0] = aDrive;
    TInt err = iFs.GetDir( aBuf, KEntryAttNormal, ESortNone, dir );

    if( err == KErrNone )
        {
        CleanupStack::PushL( dir );
        // Remove search pattern.
        aBuf.Delete( KApplicationExcludeListPath().Length(), 
            KApplicationExcludeListFileSearchPattern().Length() );
    
        for( TInt i = 0; i < dir->Count(); i++ )
            {
            const TEntry& entry = ( *dir )[i];
            aBuf.Append( entry.iName );
    
            RFile file;
            err = file.Open( iFs, aBuf, EFileRead );
            if ( err == KErrNone )
                {
                file.Close();
                iListModel->AppendL( aBuf );
                }
            // Remove file name.
            aBuf.Delete( KApplicationExcludeListPath().Length(), entry.iName.Length() );
            }
        CleanupStack::PopAndDestroy( dir );
        dir = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CRfsTestAppUi::ClearProgressBar
// ---------------------------------------------------------------------------
//
void CRfsTestAppUi::ClearProgressBar()
    {
    FUNC_LOG

    if ( iProgress )
        {
        TRAPD_ERR( err, iProgress->ProcessFinishedL() );
        ERROR( err, "Failed to update progress bar" );
        delete iProgress;
        iProgress = NULL;
        }
    }
