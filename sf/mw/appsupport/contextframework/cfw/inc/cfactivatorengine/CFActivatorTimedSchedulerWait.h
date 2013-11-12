/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActivatorTimedSchedulerWait class declaration.
*
*/



#ifndef C_CFACTIVATORTIMEDSCHEDULERWAIT_H
#define C_CFACTIVATORTIMEDSCHEDULERWAIT_H

#include <e32base.h>

/**
*  Timed active scheduler wait.
*  This class wraps inside a timer and scheduler wait.
*  Wait will be automatically stopped after specified time.
*
*  @lib -
*  @since S60 4.0
*/
NONSHARABLE_CLASS( CCFActivatorTimedSchedulerWait ): public CTimer
    {
    public:  // Constructors and destructor

        // Two-phased constructor.
        static CCFActivatorTimedSchedulerWait* NewL();
        static CCFActivatorTimedSchedulerWait* NewLC();
               
        // Destructor.
        virtual ~CCFActivatorTimedSchedulerWait();
        
    public: // New methods
    
        /**
        * Starts waiting in the scheduler.
        * Wait will be automatically stopped after aInterval amount
        * of time has passed.
        * 
        * @since S60 4.0
        * @param aInterval Interval after wait will be stopped.
        * @return None
        */
        void Start( const TTimeIntervalMicroSeconds32& aInterval );

        /**
        * Stops scheduler wait.
        * 
        * @since S60 4.0
        * @param None
        * @return None
        */
        void Stop();
        
    private: // From base classes
    
        // @see CActive
        void RunL();
        
        // @see CActive
        void DoCancel();
        
    private:
    
        CCFActivatorTimedSchedulerWait();
        void ConstructL();

    private: // Data
    
        // Own: Active scheduler wait
        CActiveSchedulerWait* iWait;
    };

#endif//C_CFACTIVATORTIMEDSCHEDULERWAIT_H
