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
* Description:  CSysApMsgSimMemLowQuery implementation.
*
*/


// INCLUDES
#include <AknGlobalConfirmationQuery.h>
#include "SysApMsgSimMemLowQuery.h"
#include "SysAp.hrh"
#include <SecondaryDisplay/SecondaryDisplaySysApAPI.h>
#include <aknSDData.h>

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApMsgSimMemLowQuery::CSysApMsgSimMemLowQuery()
// ----------------------------------------------------------------------------

CSysApMsgSimMemLowQuery::CSysApMsgSimMemLowQuery(TBool aCoverDisplaySupported) 
    : CActive( EPriorityStandard ), iCoverDisplaySupported(aCoverDisplaySupported)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CSysApMsgSimMemLowQuery::~CSysApMsgSimMemLowQuery() 
// ----------------------------------------------------------------------------

CSysApMsgSimMemLowQuery::~CSysApMsgSimMemLowQuery() 
    {
    Cancel();
    delete iSimMemoryLowQuery;
    }


// ----------------------------------------------------------------------------
// CSysApMsgSimMemLowQuery::DoCancel() 
// ----------------------------------------------------------------------------

void CSysApMsgSimMemLowQuery::DoCancel() 
    {
    }

// ----------------------------------------------------------------------------
// CSysApMsgSimMemLowQuery::StartL()
// ----------------------------------------------------------------------------

void CSysApMsgSimMemLowQuery::StartL( const TDesC& aPrompt )
    {
    if( !IsActive() )
        {
        iSimMemoryLowQuery = CAknGlobalConfirmationQuery::NewL();

        // Set secondary display data if necessary
        if ( iCoverDisplaySupported )
            {
            CAknSDData* sd = CAknSDData::NewL(SecondaryDisplay::KCatSysAp, SecondaryDisplay::ECmdShowSimMemoryLowQuery, KNullDesC8);
            iSimMemoryLowQuery->SetSecondaryDisplayData(sd); // ownership to notifier client
            }

        iSimMemoryLowQuery->ShowConfirmationQueryL( 
                                        iStatus, 
                                        aPrompt, 
                                        R_AVKON_SOFTKEYS_OK_EMPTY, 
                                        R_QGN_NOTE_ERROR_ANIM,
                                        KNullDesC,
                                        0,
                                        0,
                                        CAknQueryDialog::EErrorTone,
                                        EFalse
                                        );
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CSysApMsgSimMemLowQuery::RunL() 
// ----------------------------------------------------------------------------

void CSysApMsgSimMemLowQuery::RunL() 
    {
    TRACES( RDebug::Print( _L( "CSysApMsgSimMemLowQuery::RunL: started, %d selected" ), iStatus.Int() ) );
    delete iSimMemoryLowQuery;
    iSimMemoryLowQuery = NULL;
    TRACES( RDebug::Print( _L( "CSysApMsgSimMemLowQuery::RunL: completed" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApMsgSimMemLowQuery::RunError( TInt aError )
// ----------------------------------------------------------------------------

#ifdef _DEBUG
TInt CSysApMsgSimMemLowQuery::RunError( TInt aError )
#else
TInt CSysApMsgSimMemLowQuery::RunError( TInt /* aError */ )
#endif
    {
    TRACES( RDebug::Print( _L( "CSysApMsgSimMemLowQuery::RunError: %d started" ), aError ) );
    delete iSimMemoryLowQuery;
    iSimMemoryLowQuery = NULL;
    TRACES( RDebug::Print( _L( "CSysApMsgSimMemLowQuery::RunError: completed" ) ) );
    return KErrNone;            
    }

// End of File
