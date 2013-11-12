/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CGLOBALWAITINGNOTE_H
#define CGLOBALWAITINGNOTE_H

// INCLUDES
#include <e32base.h>
#include <aknglobalprogressdialog.h>
#include <avkon.rsg>



/**
 * Global waiting message for XIMP Framework Eunit tests.
 */
class CGlobalWaitingNote : public CActive
    {
    public:

        /**
         * Shows the waiting message with progress bar.
         *
         * @param aMessage The message text to show.
         * @param aTotalShowTime The show time in milliseconds.
         * @param aProgressSteps How many progress steps the progress bar should have.
         */
        inline static void ShowMsgL( const TDesC& aMessage,
                                     TInt aTotalShowTime,
                                     TInt aProgressSteps );


    private:

        inline CGlobalWaitingNote();
        inline ~CGlobalWaitingNote();
        inline void ConstructL();

        inline void DoShowMsgL( const TDesC& aMessage,
                                TInt aTotalShowTime,
                                TInt aProgressSteps );

        inline static TInt ProgressTickCb( TAny* aSelf );
        inline TInt HandleProgressTick();

        inline void RunL();
        inline TInt RunError( TInt aError );
        inline void DoCancel();



    private: //data

        //OWN: Active Scheduler wait
        CActiveSchedulerWait    iWait;

        //OWN: Progress ticker
        CPeriodic*  iProgressTicker;

        //OWN: The global note.
        CAknGlobalProgressDialog*   iGlobalNote;

        //OWN: Total show time
        TInt iTotalShowTime;

        //OWN: Progress interval
        TInt iProgressInterval;

        //OWN: Current progress value
        TInt iCurrentProgress;
    };





// -----------------------------------------------------------------------------
// CGlobalWaitingNote public functions
// -----------------------------------------------------------------------------
//
void CGlobalWaitingNote::ShowMsgL( const TDesC& aMessage,
                                   TInt aTotalShowTime,
                                   TInt aProgressSteps )
    {
    CGlobalWaitingNote* self = new (ELeave) CGlobalWaitingNote;
    CleanupStack::PushL( self );
    self->ConstructL();
    self->DoShowMsgL( aMessage,
                      aTotalShowTime,
                      aProgressSteps );

    CleanupStack::PopAndDestroy();
    }



// -----------------------------------------------------------------------------
// CGlobalWaitingNote private functions
// -----------------------------------------------------------------------------
//
CGlobalWaitingNote::CGlobalWaitingNote()
    : CActive( CActive::EPriorityStandard )
    {
    }


CGlobalWaitingNote::~CGlobalWaitingNote()
    {
    delete iProgressTicker;
    delete iGlobalNote;
    }


void CGlobalWaitingNote::ConstructL()
    {
    CActiveScheduler::Add( this );
    iGlobalNote = CAknGlobalProgressDialog::NewL();

    iProgressTicker = CPeriodic::NewL( CActive::EPriorityStandard );
    }



void CGlobalWaitingNote::DoShowMsgL( const TDesC& aMessage,
                                     TInt aTotalShowTime,
                                     TInt aProgressSteps )
    {
    iGlobalNote->ShowProgressDialogL( iStatus,
                                      aMessage,
                                      R_AVKON_SOFTKEYS_OK_EMPTY,
                                      aTotalShowTime );
    SetActive();

    iProgressInterval =  aTotalShowTime / aProgressSteps;
    iTotalShowTime = aTotalShowTime;
    iCurrentProgress = 0;

    TCallBack progressCb( CGlobalWaitingNote::ProgressTickCb, this );
    iProgressTicker->Start( iProgressInterval, iProgressInterval, progressCb );

    iWait.Start();
    }


TInt CGlobalWaitingNote::ProgressTickCb( TAny* aSelf )
    {
    return ( (CGlobalWaitingNote*) aSelf )->HandleProgressTick();
    }


TInt CGlobalWaitingNote::HandleProgressTick()
    {
    if( iCurrentProgress < iTotalShowTime )
        {
        iCurrentProgress += iProgressInterval;
        iGlobalNote->UpdateProgressDialog( iCurrentProgress, iTotalShowTime );
        }

    else
        {
        iGlobalNote->ProcessFinished();
        }

    return KErrNone;
    }



void CGlobalWaitingNote::RunL()
    {
    iWait.AsyncStop();
    }


TInt CGlobalWaitingNote::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }


void CGlobalWaitingNote::DoCancel()
    {
    iGlobalNote->ProcessFinished();
    }

#endif // CGLOBALWAITINGNOTE_H

// end of file



