/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApWaitNote implementation.
*
*/


// INCLUDES

#include <avkon.hrh>
#include <avkon.rsg>
#include "SysAp.hrh"
#include "SysApWaitNote.h"
#include <SecondaryDisplay/SecondaryDisplaySysApAPI.h>
#include <aknSDData.h>


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApWaitNote::NewL() 
// ----------------------------------------------------------------------------
CSysApWaitNote* CSysApWaitNote::NewL(TBool aCoverDisplaySupported)
    {
    CSysApWaitNote* self = new(ELeave) CSysApWaitNote(aCoverDisplaySupported);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApWaitNote::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApWaitNote::ConstructL()
    {
    iWaitNote = CAknGlobalNote::NewL();           
    iWaitNote->SetSoftkeys( R_AVKON_SOFTKEYS_EMPTY );
    }

// ----------------------------------------------------------------------------
// CSysApWaitNote::CSysApWaitNote()
// ----------------------------------------------------------------------------

CSysApWaitNote::CSysApWaitNote(TBool aCoverDisplaySupported) : iActive( EFalse ),
    iCoverDisplaySupported(aCoverDisplaySupported)
    {
    }

// ----------------------------------------------------------------------------
// CSysApWaitNote::~CSysApWaitNote() 
// ----------------------------------------------------------------------------

CSysApWaitNote::~CSysApWaitNote() 
    {
    Cancel();
    delete iWaitNote;
    iWaitNote = NULL;
    }


// ----------------------------------------------------------------------------
// CSysApWaitNote::Cancel() 
// ----------------------------------------------------------------------------

void CSysApWaitNote::Cancel() 
    {
    TRACES( RDebug::Print( _L("CSysApWaitNote::Cancel()" ) ) );
    TRAPD ( err, iWaitNote->CancelNoteL( iNoteId ) );
    iActive = EFalse;
    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApWaitNote::Cancel(), error: %d" ), err ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApWaitNote::ShowNoteL()
// ----------------------------------------------------------------------------

void CSysApWaitNote::ShowNoteL( const TSysApWaitNoteIds aNote, HBufC* aNoteStringBuf )
    {
    TRACES( RDebug::Print( _L("CSysApWaitNote::ShowNoteL") ) );

    if ( iActive )
        {
        Cancel();
        }
    iActive = ETrue;
    TInt tone ( EAvkonSIDNoSound );
    iWaitNote->SetTone( tone );
    TRACES( RDebug::Print( _L("CSysApWaitNote::StartL: ShowNoteL()" ) ) );
    
    if ( aNote == EClosingApplicationsNote )
        {
        // Set secondary display data if necessary
        if ( iCoverDisplaySupported )
            {
            CAknSDData* sd = CAknSDData::NewL(SecondaryDisplay::KCatSysAp, SecondaryDisplay::ECmdShowClosingApplicationsNote, KNullDesC8);
            iWaitNote->SetSecondaryDisplayData(sd); // ownership to notifier client
            }
        
        }
    
    iNoteId = iWaitNote->ShowNoteL( EAknGlobalWaitNote, aNoteStringBuf->Des() );
    }

// End of File



