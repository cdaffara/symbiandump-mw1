/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Performance monitor timer
*
*/


#ifndef CCMRPERFMONITOR_H
#define CCMRPERFMONITOR_H

// INCLUDES
#include <e32base.h>
#include <mmf/devvideo/devvideobase.h>

// FORWARD DECLARATIONS
class CCMRActiveOutput;
class CCMRVideoRecorderClient;
class MMMFClockSource;
class CCMRMediaRecorder;

// CLASS DECLARATION

/**
*  Performance monitor class. Continuously checks the performance of the system (e.g. timestamps) 
*  and adjusts video settings based on the current state
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRPerfMonitor : public CTimer, public MMMFClockSource
    {
    public: // Constants
        enum TErrorCode
            {
            EInternalAssertionFailure = -10060
            };
        
    public:

        /**
        * Two-phased constructor.
        */
        static CCMRPerfMonitor* NewL(
            CCMRActiveOutput* aAudioOutput, 
            CCMRActiveOutput* aVideoOutput, 
            CCMRVideoRecorderClient* aVideoRecorder,
            CCMRMediaRecorder* aMediaRecorder);

        /**
        * Destructor.
        */
        ~CCMRPerfMonitor();

    public: // New functions


        /**
        * Starts monitoring
        * @since 2.1
        */
        void StartL();

        /**
        * Stops monitoring
        * @since 2.1
        */
        void StopL();

        void PauseL();

    public: // From MMMFClockSource

	    /**
	    Retrieves a custom interface for the clock source.

	    @param  "aInterface"	"Interface UID, defined by the entity specifying the interface."
	    @return "Pointer to the interface implementation, or NULL if the interface is not available. 
    			The pointer must be cast to the appropriate interface class."
    	*/
    	TAny* CustomInterface(TUid aInterface);

	    /**
	    Retrieves the current stream time.

	    @return "The number of microseconds passed in the clock compared to the reference time."
	    */
	    TTimeIntervalMicroSeconds Time();

    protected: // From CTimer/CActive

        /**
        * From CTimer Checks status & possibly adjusts settings
        */
        void RunL();

        /**
        * From CTimer Cancels timer
        */
        void DoCancel();

        /**
        * From CTimer Handles leaves from RunL
        */
        TInt RunError(TInt aError);

    private: // Constructors

        /**
        * C++ default constructor.
        */
        inline CCMRPerfMonitor() : CTimer(EPriorityHigh) {};

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL(            
            CCMRActiveOutput* aAudioOutput, 
            CCMRActiveOutput* aVideoOutput, 
            CCMRVideoRecorderClient* aVideoRecorder,
            CCMRMediaRecorder* aMediaRecorder);


    private: // Data
        // active object handling audio output
        CCMRActiveOutput* iAudioOutput;
        // active object handling video output
        CCMRActiveOutput* iVideoOutput;
        // video recorder client handling video settings
        CCMRVideoRecorderClient* iVideoRecorder;
        // Mediarecorder - audio/video settings, audioenabled
        CCMRMediaRecorder* iMediaRecorder;

        // counter for total adjusted time in milliseconds
        TInt iAdjustedTimeMs;
        // latest audio timestamp
        TTimeIntervalMicroSeconds iAudioTime;
        // Pause offset when iClockSource in use.
        TTimeIntervalMicroSeconds iClockSourcePauseOffset;
        // whether performance monitor's timed monitoring function is running or not
        TBool iTimedMonitoringRunning;
        // System clocksource started
        TBool iClockSourceStarted;
        // Performance monitor paused
        TBool iPerfMonPaused;
        
        TTime iStartTime;
        TTimeIntervalMicroSeconds iTimeStampWhenPaused;
    };


#endif // CCMRPERFMONITOR_H


// End of file
