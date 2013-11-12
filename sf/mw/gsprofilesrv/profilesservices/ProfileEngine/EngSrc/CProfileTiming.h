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
* Description:  This class handles the interaction with the scheduler and
*                updates the data needed by timed profiles to Central
*                Repository.
*
*/



#ifndef CPROFILETIMING_H
#define CPROFILETIMING_H

//  INCLUDES
#include <csch_cli.h>
#include <centralrepository.h>

// FORWARD DECLARATIONS
class MProfilesLocalFeatures;

// CLASS DECLARATION

/**
*  The interface to the timed profile.
*  This class handles the interaction with the scheduler and updates the data
*  needed by timed profiles to Central Repository.
*
*  @lib profileeng.lib
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CProfileTiming ) : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileTiming* NewL( CRepository& aProfileRepository,
                                     MProfilesLocalFeatures& aFeatures );

        /**
        * Destructor.
        */
        virtual ~CProfileTiming();

    public: // New functions

        /**
         * Times the profile with the given ID.
         * @param aPreviousId the ID of the profile to be activated after the
         *        timer expires. This is the profile which was active before
         *        the current profile was timed.
         * @param aTime the time when the timer expires.
         */
        void SetTimedProfileL( TInt aPreviousId, TTime aTime );

        /**
         * Deletes the newly-scheduled task for timed profile. This is used
         * only when update to Central Repository has failed. Does not return
         * any error codes or leave because this will be called only in error
         * situation and there isn't much that could do if this fails also.
         */
        void RollbackTimedProfile();

        /**
         * Cancels the current timed profile if one is active.
         * @returns the ID of the profile which was active before the
         *          cancelled timed profile or KErrNotFound.
         */
        TInt CancelTimedProfileL();

        /**
         * Returns boolean value indicating whether there is currently an
         * active timing.
         * @return ETrue if the current active profile is timed, EFalse
         *         otherwise.
         */
        TBool IsTimingActiveL();

    private: // New functions

        /**
         * Saves the data needed by timer task DLL and for cancelling the task
         * in Central Repository.
         * @param aPreviousId the ID of the profile which was active before the
         *        timed profile. This is the profile which should be reactivated
         *        when the timer goes off.
         * @param aHandleId the scheduling handle ID of the timed profile task.
         * @param aTaskId the task ID of the timed profile task.
         */
        void SaveSchedulingData( TInt aPreviousId = -1,
                                 TInt aHandleId = -1,
                                 TInt aTaskId = -1 );

        /**
         * Gets the information about current timed profile from Central
         * Repository.
         * @param aPreviousId contains the ID of the profile which should be
         *        re-activated when the timer goes off on return.
         * @param aHandleId contains the ID of the scheduling handle of the
         *        timed profile on return.
         * @param aTaskId contains the ID assigned for the task of the timed
         *        profile on return.
         */
        void GetSchedulingDataL( TInt& aPreviousId, TInt& aHandleId,
                                 TInt& aTaskId );

        /**
         * Creates connection to the task scheduler. This method is not called
         * before the connection to the server is really needed.
         */
        void ConnectToSchedulerL();

    private:

        /**
        * C++ constructor.
        */
        CProfileTiming( CRepository& aProfileRepository,
                        MProfilesLocalFeatures& aFeatures );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
        // Profiles Central Repository reference
        CRepository& iRepository;
        
        // Profiles local features reference
        MProfilesLocalFeatures& iFeatures;
        
        // Own: Scheduler for scheduling profile timing tasks
        RScheduler iScheduler;

        // Own: Array used as container for the schedule of a timed profile
        CArrayFixFlat<TScheduleEntryInfo2>* iEntryList;

        // this is needed by ScheduleTask(), doesn't really contain anything
        TSchedulerItemRef iSchedulerItemRef;

        // Own: task data needed by ScheduleTask(), in practise this is empty
        HBufC* iTaskData;

        // Path to the DLL to be run by the scheduler when the timer goes off
        TFileName iTaskDllPath;

        // Task ID for rollback in case of errors
        TInt iTaskId;

        // Flag indicating whether the connection to the scheduler is open
        TBool iConnected;
    };

#endif      // CPROFILETIMING_H

// End of File
