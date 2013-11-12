/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <avkon.hrh>
#include <aknquerydialog.h>
#include <commondialogs.hrh>
#include <akncommondialogsdynmem.h>
#include <t_oomtestapp.rsg>
#include "t_oomtestappcmd.hrh"
#include "t_oomtestappApplication.h"
#include "t_oomtestappAppUi.h"
#include "t_oomtestappAppView.h"

_LIT( KOomConfigTargetFile, "c:\\private\\10207218\\oomconfig.xml" );
_LIT( KOomPriorityNormal, "normal" );
_LIT( KOomPriorityHigh, "high" );
_LIT( KOomPriorityBusy, "busy" );
const TInt KInitialAllocSize = 2048;

// ---------------------------------------------------------------------------
// AskPathL
// ---------------------------------------------------------------------------
//
static TBool AskPathL( TDes& aPath )
    {
    TParsePtr parse( aPath );
    TPtrC rootFolder = parse.DriveAndPath();
    TBool ret = AknCommonDialogsDynMem::RunSelectDlgLD(
        AknCommonDialogsDynMem::EMemoryTypePhone |
        AknCommonDialogsDynMem::EMemoryTypeMMC,
        aPath,
        rootFolder,
        R_MEMORY_SELECTION,
        R_FILE_SELECTION );
    return ret;
    }


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// Ct_oomtestappAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ct_oomtestappAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	User::LeaveIfError( iOomSession.Connect() );

	// Create view object
	iAppView = Ct_oomtestappAppView::NewL(ClientRect(), *this);
	
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppUi::Ct_oomtestappAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
Ct_oomtestappAppUi::Ct_oomtestappAppUi()
	{
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppUi::~Ct_oomtestappAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
Ct_oomtestappAppUi::~Ct_oomtestappAppUi()
	{
    delete iAppView;
    iOomSession.Close();
    User::Free(iMem);
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void Ct_oomtestappAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
        case EOomTestAppSelectConfig:
            {
            TFileName fileName;
            if ( AskPathL( fileName ) )
                {
                RFs& fs( iEikonEnv->FsSession() );
                fs.MkDirAll( KOomConfigTargetFile ); // Ignore error
                CFileMan* fileMan = CFileMan::NewL( fs );
                CleanupStack::PushL( fileMan );
                User::LeaveIfError( fileMan->Copy( fileName, KOomConfigTargetFile ) );
                CleanupStack::PopAndDestroy( fileMan );
                }
            break;
            }
        case EOomTestAppToggleSystem:
            {
            iEikonEnv->SetSystem( !(iEikonEnv->IsSystem()) );
            iAppView->DrawDeferred();
            break;
            }
        case EOomTestAppAllocMemory:
            {
            TInt size(KInitialAllocSize);
            CAknNumberQueryDialog* query = CAknNumberQueryDialog::NewL(size);
            if ( query->ExecuteLD( R_ALLOC_QUERY ) )
                {
                User::Free(iMem);
                iMem = NULL;
                size *= KKiloByte;
                iMem = User::AllocL(size);
                iAppView->DrawDeferred();
                }
            break;
            }
        case EOomTestAppFreeMemory:
            {
            User::Free(iMem);
            iMem = NULL;
            iAppView->DrawDeferred();
            break;
            }
        case EOomTestAppAllocMemWithPermission:
            {
            TInt size(KInitialAllocSize);
            CAknNumberQueryDialog* query = CAknNumberQueryDialog::NewL(size);
            if ( query->ExecuteLD( R_ALLOC_QUERY ) )
                {
                User::Free(iMem);
                iMem = NULL;
                size *= KKiloByte;
                if ( iPriority != ROomMonitorSession::EOomPriorityBusy )
                    {
                    // Prevent OOM to close this app while it is freeing memory
                    iOomSession.SetOomPriority(ROomMonitorSession::EOomPriorityBusy);
                    }
                iOomSession.RequestFreeMemory( size );
                if ( iPriority != ROomMonitorSession::EOomPriorityBusy )
                    {
                    // Allow OOM to close this app again
                    iOomSession.SetOomPriority(iPriority);
                    }
                iMem = User::AllocL(size);
                iAppView->DrawDeferred();
                }
            break;
            }
        case EOomTestAppSetPriorityNormal:
            {
            iPriority = ROomMonitorSession::EOomPriorityNormal;
            iOomSession.SetOomPriority(iPriority);
            iAppView->DrawDeferred();
            break;
            }
        case EOomTestAppSetPriorityHigh:
            {
            iPriority = ROomMonitorSession::EOomPriorityHigh;
            iOomSession.SetOomPriority(iPriority);
            iAppView->DrawDeferred();
            break;
            }
        case EOomTestAppSetPriorityBusy:
            {
            iPriority = ROomMonitorSession::EOomPriorityBusy;
            iOomSession.SetOomPriority(iPriority);
            iAppView->DrawDeferred();
            break;
            }    
		case EEikCmdExit:
		case EAknSoftkeyExit: // Fall through
		    {
			Exit();
			break;
		    }
		default:
		    {
			break;
		    }
		}
	}

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void Ct_oomtestappAppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect(ClientRect());
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppUi::AllocatedHeap
// -----------------------------------------------------------------------------
//
TInt Ct_oomtestappAppUi::AllocatedHeap() const
    {
    TInt ret( 0 );
    RHeap& heap( User::Heap() );
    heap.AllocSize( ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// Ct_oomtestappAppUi::Priority
// -----------------------------------------------------------------------------
//
TPtrC Ct_oomtestappAppUi::Priority() const
    {
    switch ( iPriority )
        {
        case ROomMonitorSession::EOomPriorityNormal:
            {
            return KOomPriorityNormal();
            }
        case ROomMonitorSession::EOomPriorityHigh:
            {
            return KOomPriorityHigh();
            }
        case ROomMonitorSession::EOomPriorityBusy:
            {
            return KOomPriorityBusy();
            }
        default:
            {
            break;
            }
        }
    return KNullDesC();
    }
        

// End of File
