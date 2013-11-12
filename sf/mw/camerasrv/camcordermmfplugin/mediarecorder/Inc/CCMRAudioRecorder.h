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
* Description:  Audio recorder implementation
*
*/

#ifndef CCMRAUDIORECORDER_H
#define CCMRAUDIORECORDER_H

// INCLUDES

#include <mmf/server/mmfdatapathproxy.h>
#include <mmf/server/mmfdatasource.h>

#include "CCMRRecorderBase.h"
#include "CCMRAudioThreadProxy.h"


// FORWARD DECLARATIONS
class CCMRAudioCodecs;
class CCMRActiveOutput;


//CLASS DECLARATIONS

/**
* Audio recorder observer
*
* @since 2.1
*/
class MCMRAudioRecorderObserver
    {
    public:
        /**
        * Informs that audio recorded has changed its state
        * @param aState     new state
        */
        virtual void MaroStateChange(CCMRRecorderBase::TRecorderState aState) = 0;

        /**
        * Informs about error in audio recorder
        * @param aState     error code
        */
        virtual void MaroError(TInt aError) = 0;
    };



/**
* Audio recorder class
*
* @since 2.1
*/
class CCMRAudioRecorder : public CCMRRecorderBase,
                            public MCMRSubThreadEventMonitorObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CCMRAudioRecorder* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCMRAudioRecorder();

    public: // Constants
        enum TErrorCode
            {
            EInternalAssertionFailure = -10010,
            };

    public: // New functions
        
        /**
        * Open audio recorder
        * @since 2.1
        * @param aObserver      observer for audio recorder
        * @param aSource        audio source (handle to MMF audio input)
        * @param aOutputAO      output active object
        * @param aAudioType     audio fourCC code
        */
        void OpenL(MCMRAudioRecorderObserver *aObserver, MDataSource *aSource, 
                        CCMRActiveOutput* aOutputAO, const TFourCC& aAudioType, CCMRConfigManager* aConfig );

        /**
        * Set audio codec to be used (overrides the one given in OpenL)
        * @since 2.1
        * @param  aFourCC           audio FourCC
        * @return void
        */
        void SetAudioCodecL( const TFourCC& aAudioType );

        /**
        * Get the currently used audio codec
        * @since 2.1
        * @param  aFourCC           Used codec as FourCC
        * @return void
        */
        void GetAudioCodecL( TFourCC& aAudioType ) const;

        /**
        * Get supported & installed audio codecs
        * @since 2.1
        * @since 2.1
        * @param  aAudioTypes   List of supported & installed audio codecs
        * @return void
        */
        void GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes ) const;

       /**
        * Set bitrate for audio codec. If we use AMR-NB, the given bitrate
        * is interpreted proprietarily: bitrates that are not exactly AMR bitrates 
        * mean that voice activity detection is used and the actual AMR bitrate is 
        * the given bitrate rounded upwards to the next AMR bitrate
        * E.g. aBitRate = 5500 means the AMR bit-rate is 5900 and VAD is enabled
        * @since 2.1
        * @param aBitRate   bitrate for audio
        * @return void
        */
        void SetTargetBitRateL(TInt aBitRate);

       /**
        * Set samplerate for audio codec. AAC only
        * @since 3.1u
        * @param aSampleRate   samplerate for audio
        * @return void
        */
        void SetTargetSampleRateL(TInt aSampleRate);

       /**
        * Set channel mode for audio codec. AAC only
        * @since 3.1u
        * @param aChannelMode   channel mode for audio
        * @return void
        */
        void SetChannelModeL(TInt aChannelMode);

        /**
        * Get the used recording gain
        * @since 2.1
        * @return gain
        */
        TInt GainL();

        /**
        * Get the max recording gain
        * @since 2.1
        * @return max gain
        */
        TInt MaxGainL();

        /**
        * Set the recording gain. Can be used before and during recording
        * @since 2.1
        * @param aGain  gain
        */
        void SetGainL(TInt aGain);

        /**
        * Set priority settings
        * @since 2.1
        * @param  aPrioritySettings audio priority settings
        * @return error code
        */
        void SetPriorityL(const TMMFPrioritySettings& aPrioritySettings);

        /**
        * Wait until audio thread has stopped
        * @since 2.1
        */
        void WaitUntilStoppedL();

        /**
        * Get default AV sync adjustment for start of recording, depending on codec type
        * @since 2.8
        * @return AV sync adjustment value for starting of recording
        */
        TInt AVSyncAdjustmentStart();
        /**
        * Get default AV sync adjustment for resume of recording, depending on codec type
        * @since 2.8
        * @return AV sync adjustment value for resuming recording
        */
        TInt AVSyncAdjustmentResume();

        /**                     
        * Get audio thread priority
        * @return TThreadPriority aThreadPriority
        */
        TThreadPriority GetThreadPriority() const;

    public: // Functions from base classes

        /**
        * From CCMRRecorderBase Prepares/primes for recording
        */
        void PrepareL();

        /**
        * From CCMRRecorderBase Starts recording
        */
        void RecordL();

        /**
        * From CCMRRecorderBase Stops recording (async => must wait for state change)
        */
        void StopL();

        /**
        * From CCMRRecorderBase Pauses recording (async => must wait for state change)
        */
        void PauseL();

        /**
        * From CCMRRecorderBase Resumes recording
        */
        void ResumeL();

        /**
        * From MMMFDataPathEventMonitorObserver Handles event from datapathproxy
        */
	    void HandleEvent(const TMMFEvent& aEvent);

    private:

        /**
        * C++ default constructor.
        */
        CCMRAudioRecorder();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();


    private:    // Data

      
        // Audio priority settings
        TMMFPrioritySettings iPrioritySettings;

        // Observer
        MCMRAudioRecorderObserver* iObserver;

        // Requested audio codec fourCC
        TFourCC iAudioCodecFourCC;
        // MMF datapath
        RCMRAudioThreadProxy iThreadProxy;

        // ETrue if iDataPathProxy was opened => can be closed
        TBool iThreadCreated;

        // Datapath event monitor
        CCMRSubThreadEventMonitor *iThreadEventMonitor;

        // MMF audio input
        MDataSource* iDataSource;

        // Active object for outputting data & accessing mediasink
        CCMRActiveOutput *iOutputAO;

        // Audio codec-specific data is available from this object
        CCMRAudioCodecs* iAudioCodecs;
        
        // Config manager
        // Doesn't own.
        CCMRConfigManager* iConfig;        

        // Error code saved while waiting for a completion of an async operation
        TInt iErrorCode;
    };

#endif      
            
// End of File



