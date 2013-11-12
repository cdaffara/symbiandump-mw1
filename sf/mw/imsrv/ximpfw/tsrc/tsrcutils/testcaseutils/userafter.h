/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object based wait.
*
*/

#ifndef CUSERAFTER_H__
#define CUSERAFTER_H__

// INCLUDES
#include <E32base.h>


// CLASS DESCRIPTION

/**
 * Active object based wait.
 *
 * Similar like User::After() but doesn't block
 * whole thread, but current RunL() with
 * CActiveSchedulerWait.
 */
class CUserAfter : public CTimer
    {
    public: //Construction
        static inline CUserAfter* NewL();
        static inline CUserAfter* NewLC();
       ~CUserAfter();


    public: //Wait support

        /**
         * Static "one shot" wait method.
         */
        static inline void AfterL( TInt aWaitTimeMicroSeconds );
        static inline void AfterSecondsL( TInt aWaitTimeSeconds );

        /**
         * Member wait method.
         */
        inline void After( TInt aWaitTimeMicroSeconds );


    private:
        CUserAfter();

        void RunL();
        void RunError();
        void DoCancel();


    private:    //data
        CActiveSchedulerWait iWait;

    };




// -----------------------------------------------------------------------------
// CUserAfter public functions
// -----------------------------------------------------------------------------
//
inline CUserAfter* CUserAfter::NewL()
    {
    CUserAfter* self = CUserAfter::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

inline CUserAfter* CUserAfter::NewLC()
    {
    CUserAfter* self = new (ELeave) CUserAfter();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


inline CUserAfter::~CUserAfter()
    {
    CTimer::Cancel();
    }

inline void CUserAfter::After( TInt aWaitTimeMicroSeconds )
    {
    CTimer::After( aWaitTimeMicroSeconds );
    iWait.Start();
    }


inline void CUserAfter::AfterL( TInt aWaitTimeMicroSeconds )
    {
    CUserAfter* after = CUserAfter::NewL();
    after->After( aWaitTimeMicroSeconds );
    delete after;
    }


inline void CUserAfter::AfterSecondsL( TInt aWaitTimeSeconds )
    {
    CUserAfter* after = CUserAfter::NewL();
    after->After( aWaitTimeSeconds * 1000000 );
    delete after;
    }


// -----------------------------------------------------------------------------
// CUserAfter private functions
// -----------------------------------------------------------------------------
//
inline CUserAfter::CUserAfter()
    : CTimer( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


inline void CUserAfter::RunL()
    {
    iWait.AsyncStop();
    Cancel();
    }

inline void CUserAfter::RunError()
    {
    }

inline void CUserAfter::DoCancel()
    {
    iWait.AsyncStop();
    CTimer::DoCancel();
    }


#endif // CUSERAFTER_H__

// End of File

