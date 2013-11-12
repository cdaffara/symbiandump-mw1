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
* Description:  Drive unlock handler implementation
 *
*/



// INCLUDE FILES
#include <e32std.h>
#include <f32file.h>
#include <e32property.h>
#include <UikonInternalPSKeys.h>
#include "sysapdriveunlockhandler.h"
#include "sysapdrivelist.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::CSysApDriveUnlockHandler
// ---------------------------------------------------------------------------
//
CSysApDriveUnlockHandler* CSysApDriveUnlockHandler::NewL(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported )
    {
    return new ( ELeave ) CSysApDriveUnlockHandler(
        aSysApDriveList,
        aSysApAppUi,
        aMemoryCardLockSupported );
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::CSysApDriveUnlockHandler
// ---------------------------------------------------------------------------
//
CSysApDriveUnlockHandler::CSysApDriveUnlockHandler(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported ) :
    iSysApDriveList( aSysApDriveList ),
    iSysApAppUi( aSysApAppUi ),
    iMemoryCardLockSupported( aMemoryCardLockSupported ),
    iMemCardPwdDialog(NULL)
    {
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::~CSysApDriveUnlockHandler
// ---------------------------------------------------------------------------
//
CSysApDriveUnlockHandler::~CSysApDriveUnlockHandler()
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveUnlockHandler::~CSysApDriveUnlockHandler" ) ) );

    iIgnoreQueryResult = ETrue;
    if (iMemCardPwdDialog!=NULL)
        {
        //MemoryCardDialog already exist
        delete iMemCardPwdDialog;
        iMemCardPwdDialog = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::StartUnlock
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::StartUnlock()
    {
    TRAPD( err, DoStartQueryIfNeededL() );
    if ( err != KErrNone )
        {
        DoStopUnlock( err );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::StopUnlock
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::StopUnlock( TInt aDrive )
    {
    TBool isOngoing( IsQueryOngoing() );

    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::StopUnlock: ongoing: %d, drive: %d" ),
        isOngoing, iDriveToUnlock ) );

    if ( !isOngoing || iDriveToUnlock != aDrive )
        {
        return;
        }
    // Stop current query and continue with other locked memory cards
    iSysApDriveList.MarkDriveUnlockQueryShown( iDriveToUnlock );
    DoStopUnlock( KErrNone );
    TRAPD( err, DoStartQueryIfNeededL() );
    if ( err != KErrNone )
        {
        DoStopUnlock( err );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::UnlockComplete
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::UnlockComplete( TInt aResult )
    {
    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::UnlockComplete: ignore: %d, drive: %d, result: %d" ),
        iIgnoreQueryResult, iDriveToUnlock, aResult ) );

    if ( iIgnoreQueryResult || iDriveToUnlock == KErrNotFound )
        {
        return;
        }

    // Handle default memory card functionality
    if ( iDriveToUnlock == iSysApDriveList.DefaultMemoryCard() )
        {
        if ( aResult == KErrNone )
            {
            RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
            }
        else
            {
            RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
            }
        }

    // Mark handled and start next query
    iSysApDriveList.MarkDriveUnlockQueryShown( iDriveToUnlock );
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::QueryShowCB
// ---------------------------------------------------------------------------
//
TInt CSysApDriveUnlockHandler::QueryShowCB( TAny* aPtr )
    {
    CSysApDriveUnlockHandler* self =
        static_cast< CSysApDriveUnlockHandler* >( aPtr );
    TRAPD( err, self->ShowUnlockQueryL() );
    if ( err != KErrNone )
        {
        self->DoStopUnlock( err );
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::ShowUnlockQueryL
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::ShowUnlockQueryL()
    {
    iDriveToUnlock = iSysApDriveList.DriveToUnlock();
    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::ShowUnlockQueryL: drive: %d" ),
        iDriveToUnlock ) );

    if (iMemCardPwdDialog!=NULL)
        {
        //PowerMenu already exist
        delete iMemCardPwdDialog;
        iMemCardPwdDialog = NULL;
        } 
    TRACES( RDebug::Print(_L("CSysApAppUi::HandleKeyEventL, JEELani 01") ) );
    iMemCardPwdDialog = CHbDeviceInputDialogSymbian::NewL();
    TRACES( RDebug::Print(_L("CSysApAppUi::HandleKeyEventL, JEELani 02") ) );
    iMemCardPwdDialog->ShowL();
    TRACES( RDebug::Print(_L("CSysApAppUi::HandleKeyEventL, JEELani 03") ) );    
}

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::IsQueryOngoing
// ---------------------------------------------------------------------------
//
TBool CSysApDriveUnlockHandler::IsQueryOngoing() const
    {
    return 0; 
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::DoStartQueryIfNeededL
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::DoStartQueryIfNeededL()
    {
    iDriveToUnlock = iSysApDriveList.DriveToUnlock();

    if ( iDriveToUnlock == KErrNotFound )
        {
        _LIT(KChargingNote,"Phone does not found drive to unlock");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KChargingNote);
        TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString         
        return;
        }

    if ( iMemoryCardLockSupported )
        {
        ShowUnlockQueryL();
        }
    else
        {
        iSysApAppUi.ShowQueryL( ESysApMemoryCardLockedNote );
        _LIT(KChargingNote,"Phone does not support locked memory cards");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KChargingNote);
        TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString    
        iSysApDriveList.MarkDriveUnlockQueryShown( iDriveToUnlock );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::DoStartQueryAsyncIfNeeded
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::DoStartQueryAsyncIfNeeded()
    {
    iDriveToUnlock = iSysApDriveList.DriveToUnlock();
    if ( iDriveToUnlock == KErrNotFound )
        {
        DoStopUnlock( KErrNone );
        return;
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::DoStopUnlock
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::DoStopUnlock( TInt aError )
    {
    if ( aError != KErrNone )
        {
        TRACES( RDebug::Print(
            _L( "CSysApMCSysApMMCUnlockObserver::DoStopUnlock: error: %d" ),
            aError ) );
        }
    ReleaseMemoryForInputCardDialog();
    iIgnoreQueryResult = ETrue;
    iIgnoreQueryResult = EFalse;
    }

void CSysApDriveUnlockHandler::ReleaseMemoryForInputCardDialog()
    {
    if (iMemCardPwdDialog!=NULL)
        {
        //MemoryCardDialog already exist
        delete iMemCardPwdDialog;
        iMemCardPwdDialog = NULL;
        }
    }

TInt CSysApDriveUnlockHandler::CheckMemoryDialogIfNeeded()
  {
    TRACES( RDebug::Print(_L( "CSysApDriveUnlockHandler::CheckMemoryDialogIfNeeded(): Begin" )));
    TDriveNumber drive( static_cast< TDriveNumber >( iDriveToUnlock ) );
    TRACES( RDebug::Print(_L( "CSysApDriveUnlockHandler::CheckMemoryDialogIfNeeded()" )));
    TPtrC aStringPointer11 = iMemCardPwdDialog->getTextL();           
    ConvertCharactersToPwd(aStringPointer11,iPassword);
    
    if(!aStringPointer11.Length())
        {
        return EFalse;  // It meant user has pressed OK without password
        }

    TRACES( RDebug::Print(_L( "CSysApMCSysApMMCUnlockObserver::RunL; iPassword %S" ),&iPassword));              
    CEikonEnv* eikEnv = CEikonEnv:: Static();
    TInt err = eikEnv->FsSession().UnlockDrive(drive,iPassword,ETrue);
    ReleaseMemoryForInputCardDialog();
    
    if ( err == KErrNone)
        {
        TRACES( RDebug::Print(_L( "CSysApAppUi::ReleaseMemoryCardCustomDialogMemory(), Drive Unlocked Succesfully" )));
        _LIT(KUnlockNote,"The memory is unlocked!");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KUnlockNote);   
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString
        return ETrue;
        }
    else if( err == KErrAccessDenied ) 
        {
        TRACES( RDebug::Print(_L( "CSysApAppUi::ReleaseMemoryCardCustomDialogMemory(), Password InCorrect" )));
        _LIT(KUnlockDeniedNote,"The password is incorrect, try again!");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KUnlockDeniedNote);   
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString
        return EFalse;
        }
    else if( err == KErrAlreadyExists ) 
        {
        TRACES( RDebug::Print(_L( "CSysApAppUi::ReleaseMemoryCardCustomDialogMemory(), Already been Drive Unlocked" )));
        _LIT(KUnlockAlreadyExistNote,"The disk has already been unlocked!");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KUnlockAlreadyExistNote);
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString
        return ETrue;
        }
    else if( err == KErrNotSupported ) 
        {
        TRACES( RDebug::Print(_L( "CSysApAppUi::ReleaseMemoryCardCustomDialogMemory(), Media does not support password locking." )));
        _LIT(KUnlockNotSupporrtedNote,"The media does not support password locking!");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KUnlockNotSupporrtedNote);
        TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString
        return ETrue;
        }
    else
        {
        // check for error -18 what it is . 
        _LIT(KUnlockOperationCancelNote,"Error occurred, operation cancelled!");
        HBufC* aString = HBufC16::NewLC(150);
        TPtrC aStringPointer = aString->Des();
        aStringPointer.Set(KUnlockOperationCancelNote);
        TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
        iSysApAppUi.ShowExampleUiNoteL( aStringPointer );
        CleanupStack::PopAndDestroy(); // aString
        return ETrue;
        }
}
        
HBufC8* CSysApDriveUnlockHandler::Convert16to8L(TDesC16& aStr)//const
    {
    
    HBufC8* newFrom1 = HBufC8::NewL(aStr.Length());
    
    newFrom1->Des().Copy(aStr);
    
    return newFrom1;
    }

void CSysApDriveUnlockHandler::ConvertCharactersToPwd(TDesC& aWord, TDes8& aConverted)
{
    aConverted.FillZ(aConverted.MaxLength());
    aConverted.Zero();
     
    if (aWord.Length())
    {
    aConverted.Copy( (TUint8*)(&aWord[0]), aWord.Size() );
    }
}

// End of File
