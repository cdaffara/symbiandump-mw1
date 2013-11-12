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
* Description:  Audio thread proxy classes
*
*/

#ifndef CCMRAUDIOTHREADPROXYSESSION_H
#define CCMRAUDIOTHREADPROXYSESSION_H

// INCLUDES
#include "CCMRAudioThreadProxy.h"



// FORWARD DECLARATIONS
class CCMRAudioInput;
class CCMRAudioThreadProxyServer;
class MDataSource;
class CCMRAudioCodecData;

//CLASS DECLARATIONS



/**
*  Audio thread proxy session class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRAudioThreadProxySession : public CCMRThreadProxySession
    {
    public:   // Constants        

        enum TErrorCode
            {
            EInternalAssertionFailure = -10050,
            };

    public: // constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CCMRAudioThreadProxySession* NewL();
        /**
        * Destructor.
        */
        ~CCMRAudioThreadProxySession();

    public:
        /**
        *  Internal class for timeout audio datapath's asynchronous pause
        *  Calls CCMRAudioThreadProxySession::SendEventToClient with KMMFEventCategoryPlaybackComplete
        *  if datapath doesn't do it in given time
        */
        class CCMRAudioPauseTimer : public CTimer
            {
            public: // constructors and destructor
                
                /**
                * Two-phased constructor.
                */
                static CCMRAudioPauseTimer* NewL(CCMRAudioThreadProxySession* aHost);
                
                /**
                * Destructor.
                */
                ~CCMRAudioPauseTimer();

                /**
                * Symbian 2nd phase constructor
                */
                void ConstructL();

            protected: // from base classes

                /**
                * From CActive Does the required action (calls the observer)
                */
                void RunL();

                /**
                * From CActive Cancels pending actions
                */
                void DoCancel();

            private: // constructor
                /**
                * C++ default constructor.
                */
                inline CCMRAudioPauseTimer(CCMRAudioThreadProxySession* aHost) : CTimer(EPriorityNormal), iHost(aHost) {};

            private: // data
                // Handle to the host of the timer, to whom the timer signals its completion (calls iHost->SendEventToClient)
                CCMRAudioThreadProxySession* iHost;
            };

    public: // from base classes
        /**                     
        * From CCMRThreadProxySession/CSharableSessions service a message.
        */
        void ServiceL(const RMessage2& aMessage);

        /**                     
        * From CCMRThreadProxySession/MAsyncEventHandler catches 
        * completion events before calling base class implementation
        */
        TInt SendEventToClient(const TMMFEvent& aEvent);

    private: // new functions
        
        /**                     
        * Set output (sink) for audio
        * @return TBool
        */
        TBool SetOutputL(const RMessage2& aMessage);
        /**                     
        * Add datasource
        * @return TBool
        */
        TBool AddDataSourceL(const RMessage2& aMessage);
        /**                     
        * Set Config manager.
        * @return TBool
        */
        TBool SetConfigManager(const RMessage2& aMessage);        
        /**                     
        * Set used audio codec
        * @return TBool
        */
        TBool SetAudioCodecL(const RMessage2& aMessage);
        /**                     
        * Set audio priority
        * @return TBool
        */
        TBool SetPriorityL(const RMessage2& aMessage);
        /**                     
        * Set audio gain
        * @return TBool
        */
        TBool SetGainL(const RMessage2& aMessage);
        /**                     
        * Get audio gain
        * @return TBool
        */
        TBool GetGainL(const RMessage2& aMessage);
        /**                     
        * Get max audio gain
        * @return TBool
        */
        TBool MaxGainL(const RMessage2& aMessage);
        /**                     
        * Set used codec data
        * @return TBool
        */
        TBool SetCodecDataL(const RMessage2& aMessage);
        /**                     
        * Prime datapath
        * @return TBool
        */
        TBool PrimeL();
        /**                     
        * Start playing (recording)
        * @return TBool
        */
        TBool PlayL();
        /**                     
        * Pause datapath
        * @return TBool
        */
        TBool PauseL();
        /**                     
        * Stop datapath
        * @return TBool
        */
        TBool StopL();

        /**                     
        * Check if datapath has stopped recording. Message is not completed before it has completed so client is waiting for it.
        * @return TBool
        */
        TBool CheckIfStoppedL();

    private: // constructor
        /**
        * C++ default constructor.
        */
        inline CCMRAudioThreadProxySession() {};

    private: // data
        // state variables for async datapath stopping/pausing
        TBool iStoppingRecording;
        TBool iPausingRecording;
        TBool iInitializingAudioCodec;

        // ETrue if we are paused => no need to stop or pause any more even if asked
        TBool iPaused;

        // ETrue when waiting for stop to complete
        TBool iWaitingForStop;

        // MMF audio input
        MDataSource* iMMFAudioInput;

        // Internal audio input class that handles audio data
        CCMRAudioInput* iAudioInput;

        // Object handling audio codec specific tasks and data
        CCMRAudioCodecData* iAudioCodec;

        // timer for pause/stop
        CCMRAudioPauseTimer* iPauseTimer;
        
        // Handle to the message, needed in async stopping
        RMessage2* iMessage;

        // Active object that gets data from us
        CCMRActiveOutput* iActiveOutput;
        
        // Config manager.
        // Doesn't own.
        CCMRConfigManager* iConfig;

        // Thread id of the thread where active output object runs
        TUint iOutputThreadId;
    };

#endif      
            
// End of File



