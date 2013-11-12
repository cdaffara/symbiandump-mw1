/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for an active object outputting data
*
*/


#ifndef CCMRACTIVEOUTPUT_H
#define CCMRACTIVEOUTPUT_H

// INCLUDES
#include <e32base.h>
#include "CCMRMediaSink.h"
#include "CCMRMediaSource.h"



// CLASS DECLARATION
/**
*  Observer for active output class. This can't be 
*  MCMRMediaSource, since CCMRAudioFormatSink is one source and 
*  it doesn't have any error signalling capabilities
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class MCMRActiveOutputObserver
    {
    public:
        /**
        * Informs error in the active object
        * @since 2.1
        * @param aError Error code
        */
        virtual void MaooError(TInt aError) = 0;
    };

// CLASS DECLARATION

/**
*  Active output class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRActiveOutput : public CActive
    {
        
    public:

        static CCMRActiveOutput* NewL(MCMRMediaSink* aSink, MCMRActiveOutputObserver* aObserver);

        ~CCMRActiveOutput();

    public: // New functions

        /**
        * Register source (audio or video) for this active object
        * @since 2.1
        * @param aSource  source object
        */
        virtual void RegisterSourceL(MCMRMediaSource* aSource);

        /**
        * Start output by giving the first request to registered source
        * @since 2.1
        */
        virtual void Start();

        /**
        * Stops output, flushes buffer queue
        * @since 2.1
        */
        virtual void Stop();

        /**
        * Pass video frame size to mediasink
        * @since 2.1
        * @param aSize Size
        * @return TInt Error code
        */
        virtual TInt SetVideoFrameSize(TSize aSize);

        /**
        * Pass average video bitrate to mediasink
        * @since 2.1
        * @param aBitRate bitrate
        * @return TInt Error code
        */
        virtual TInt SetAverageVideoBitRate(TInt aBitRate);

        /**
        * Pass maximum video bitrate to mediasink
        * @since 2.1
        * @param aBitRate bitrate
        * @return TInt Error code
        */
        virtual TInt SetMaxVideoBitRate(TInt aBitRate);

        /**
        * Pass average audio bitrate to mediasink
        * @since 2.1
        * @param aBitRate bitrate
        * @return TInt Error code
        */
        virtual TInt SetAverageAudioBitRate(TInt aBitRate);

        /**
        * Flush all buffers from source to sink. This could be done by an external
        * object if it notifies that there is a queue in the source, 
        * or the destructor of this class
        * @since 2.1
        * @return void
        */
        virtual void Flush();

        /**
        * Return the number of buffers waiting in the input of the active object
        * @since 2.1
        * @return TInt Number of buffers waiting
        */
        virtual TInt NumBuffersInQueueL();

        /**
        * Return the latest time stamp from the input stream
        * @since 2.1
        * @param TTimeIntervalMicroSeconds& aTimeStamp
        */
        virtual void LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const;

        /**
        * Return the duration of the recording
        * @since 2.6
        * @param TTimeIntervalMicroSeconds& aDuration
        */
        virtual void DurationL(TTimeIntervalMicroSeconds& aDuration) const;

    protected: // From CActive

        /**
        * From CActive Writes buffers from source to sink
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        TInt RunError(TInt aError);

    private: // Constructors

        /**
        * C++ default constructor.
        */
        CCMRActiveOutput();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL(MCMRMediaSink* aSink, MCMRActiveOutputObserver* aObserver);

	private: // State related
		enum TState
			{
			EStateIdle = 0,
			EStateTimerPause,
			EStateRequestingNewData
			};
		
		void SetState( TState aState );
		void RequestTimerPause();
		void RequestNewData();

    private: // Data

        // Media sink where to write data
        MCMRMediaSink* iSink;

        // Media source where data comes from
        MCMRMediaSource* iMediaSource;

        // Observer for error callback
        MCMRActiveOutputObserver* iObserver;

        // ETrue if Start was called
        TBool iRunning;

    	// Mutex used to use variables safely in case of use from other thread
        RMutex      iMutexObj; 

        // Buffer that is in use
        CCMRMediaBuffer* iBuffer;
        
        // State
        TState iState;
        
        // Timer for pausing
        RTimer iTimer;
    };


#endif // CCMRMEDIASINK_H


// End of file
