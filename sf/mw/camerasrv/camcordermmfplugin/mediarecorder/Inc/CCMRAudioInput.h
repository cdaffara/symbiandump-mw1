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
* Description:  Audio input class for handling audio data and giving it to CCMRActiveOutput
*
*/

#ifndef CCMRAUDIOINPUT_H
#define CCMRAUDIOINPUT_H

// INCLUDES
#include "CCMRMediaSource.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/sounddevice.h>


// FORWARD DECLARATIONS
class CCMRFifo;
class CCMRActiveOutput;
class CCMRAudioCodecData;
class CMMFDevSound;
class CCMRConfigManager;

// CLASS DECLARATIONS


/**
*  Audio input base class. This contains common interface and some common functionality 
*  for CCMRHWAudioInput and CCMRSWAudioInput classes. This class cannot be instantiated directly.
*  It is inherited from CTimer since CCMRSWAudioInput of it needs CTimer and 
*  inheriting this one from CBase would mean multiple inheritance; 
*  CCMRHWAudioInput needs CActive, but CTimer is inherited from CActive so CTimer is the common one.
*  Routines for CTimer are implemented in inherited classes
*
*  @lib camcmediarecorder.lib
*  @since 2.1
*/
class CCMRAudioInput : public CTimer, public MDataSink, public MCMRMediaSource, public MDevSoundObserver
    {

    public: // Constants
        enum TErrorCode
            {
            EInternalAssertionFailure = -10020
            };
    
        // States
        enum TInputState
            {
                EStateRecording,
                EStateStopping,
                EStateLastReceived,
                EStateStopped
            };

    public:  // Constructor and destructor
        
        /**
        * Destructor.
        */
        virtual ~CCMRAudioInput();

    public: // new functions

        /**
        * Send an event (error) to client
        * @param TMMFEvent& aEvent
        */
        virtual void SendEventToClient(const TMMFEvent& aEvent);

        /**
        * Primes source
        */
        virtual void SourcePrimeL();

        /**
        * Starts recording
        */
        virtual void SourceRecordL();

        /**
        * Pauses playing (recording)
        */
        virtual void SourcePauseL();

        /**
        * Stops playing (recording)
        */
        virtual void SourceStopL();

        /**
        * Sets priority settings
        * @param const TMMFPrioritySettings& aPrioritySettings
        */
        virtual void SetSourcePrioritySettings(const TMMFPrioritySettings& aPrioritySettings);

        /**
        * Get DevSound handle
        * @return CMMFDevSound& handle
        */
        virtual CMMFDevSound& SoundDevice();

        /**
        * Ask MMFAudioInput to fill next buffer
        * @param CMMFBuffer* aBuffer
        */
        virtual void FillBufferL();

        /**
        * Inform output about a new buffer
        * @param CMMFBuffer* aBuffer
        */
        virtual void NewBufferL(CMMFBuffer* aBuffer);

        /**
        * Set codecdata object for the audioinput
        * @param CCMRAudioCodecData* aCodecData
        */
        virtual void SetCodecL( CCMRAudioCodecData* aCodecData );

        /**                     
        * Configure codec
        */
        virtual void ConfigureCodecL() = 0;

        /**                     
        * Reset timestamp, used after a real stop (pause is also handled as stop internally)
        */
        virtual void ResetTimeStamp();

    public: // from base classes

        /**
        * From MDataSink, not supported
        */
        void ConstructSinkL( const TDesC8& /*aInitData*/ ) { User::Leave(KErrNotSupported); };

        /**
        * From MDataSink, check data type code, not called by MMFAudioInput and hence returns dummy
        */
        inline TFourCC SinkDataTypeCode(TMediaId /*aMediaId*/) {return KFourCCNULL;};

        /**
        * From MDataSink, asks to empty the given buffer, not called by MMFAudioInput and hence returns dummy
        */
        inline void EmptyBufferL(CMMFBuffer* /*aBuffer*/, MDataSource* /*aSupplier*/, TMediaId /*aMediaId*/) {};

        /**
        * From MDataSink, informs that the given buffer is filled, called by MMFAudioInput
        */
        void BufferFilledL(CMMFBuffer* aBuffer) = 0;

        /**
        * From MDataSink, asks if we can create a buffer, not called by MMFAudioInput and hence returns EFalse
        */
        inline TBool CanCreateSinkBuffer() {return EFalse;};

        /**
        * From MDataSink, asks to create a buffer, not called by MMFAudioInput and hence returns dummy
        */
        inline CMMFBuffer* CreateSinkBufferL(TMediaId /*aMediaId*/, TBool& /*aReference*/) {return NULL;};

        /**
        * From MCMRMediaSource Sink (output active object) is ready to accept new data
        */
        void RequestNewData(TRequestStatus& aStatus);

        /**
        * From MCMRMediaSource Sink (output active object) wants to cancel new data request
        */
        void RequestNewDataCancel(TRequestStatus& aStatus);

        /**
        * From MCMRMediaSource Get the next output buffer
        */
        CCMRMediaBuffer* GetNextBuffer();

        /**
        * From MCMRMediaSource Get the number of output buffers waiting in the source
        */
        TInt NumBuffersWaiting();

        /**
        * From MCMRMediaSource Return the latest time stamp from the input stream
        */
        void LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const;

        /**
        * From MCMRMediaSource Return the duration of the recording
        */
        void DurationL(TTimeIntervalMicroSeconds& aDuration) const;

        /**
        * From MCMRMediaSource Return a previously given buffer back to the source
        */
        void ReturnBuffer(CCMRMediaBuffer* aBuffer);

    public:
        /**
        * From MDevSoundObserver Handles initialization completion event.
        */        
        void InitializeComplete(TInt aError);
        
        /**
        * From MDevSoundObserver Handles tone play completion event.
        */        
        void ToneFinished(TInt aError);
        
        /**
        * From MDevSoundObserver Handles CMMFDevSound object's data request event.
        */        
        void BufferToBeFilled(CMMFBuffer* aBuffer);
        
        /**
        * From MDevSoundObserver Handles play completion or cancel event.
        */        
        void PlayError(TInt aError); 
        
        /**
        * From MDevSoundObserver Handles CMMFDevSound object's data request event.
        */        
        void BufferToBeEmptied(CMMFBuffer* aBuffer); 
        
        /**
        * From MDevSoundObserver Handles record completion or cancel event.
        */        
        void RecordError(TInt aError); 
        
        /**
        * From MDevSoundObserver Handles conversion completion or cancel event.
        */        
        void ConvertError(TInt aError);
        
        /**
        * From MDevSoundObserver Handles device event.
        */        
        void DeviceMessage(TUid aMessageType, const TDesC8& aMsg);

    protected:

        /**
        * C++ default constructor
        * @param MDataSource* aDataSource
        * @param TMediaId aMediaId
        */
        CCMRAudioInput(MDataSource* aDataSource, MAsyncEventHandler& aEventHandler) : 
            CTimer(EPriorityNormal), MDataSink(KUidMmfDataPath), iMMFDataSource(aDataSource), iEventHandler(aEventHandler) {};

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(CCMRActiveOutput* aOutput, TUint aThreadId, CCMRConfigManager* aConfig );


        /**
        * Internal function to get DevSound capabilities and configure DevSound appropriately.
        */
        virtual void ConfigDevSoundL();
        
        virtual void UpdateTimeL();
        
    private:
        virtual void AllocateInputBuffersL() = 0;

    protected: // data shared with inherited classes

        // Handle to MMFAudioInput
        MDataSource* iMMFDataSource;

        // Consumer
        CCMRActiveOutput* iConsumer;

        // ETrue if first buffer for the session has not been received
        TBool iFirstTime;

        // size of buffers allocated & managed in this class
        TUint iInputBufferSize;

        // State of the class
        TInputState iState;

        // ETrue if MMFDataPath has a buffer under processing
        TBool iUnderProcessing;

        TBool iFirstBufferRequested;
        
        // Counter for the number of received buffers
        TInt iCurrentSourceFrameNumber;
        // Counter for the number of stored bytes
        TInt iNumBytesStored;


        // Fifo for empty buffers that are available for storing data from MMFAudioInput/DevSound
        CCMRFifo* iInputEmpty;
        // Fifo is for buffers that are storing data from MMFAudioInput/DevSound (PCM or coded, depending on use case)
        CCMRFifo* iInputFilled;
        // Fifo for buffers that contain encoded data and are waiting for sink to retrieve
        CCMRFifo* iOutputFilled;
        // Fifo for buffers that sink has read and are available for reuse in iOutputFilled;
        // bufs can't go directly to iInputEmpty since this is always for coded frames, input bufs may be for PCM16. 
        // Depending on the derivative, this is either a temporary storage for buffers (HW) until audio thread moves them 
        // to iInputEmpty, or "permanent" meaning that the buffers are circulated only between iOutputEmptied and iOutputFilled
        CCMRFifo* iOutputEmptied;

        // Event handler
        MAsyncEventHandler& iEventHandler;

        // Handle to the buffer from MMFAudioInput
        CMMFBuffer* iDevSoundBuffer;

        // ETrue if we have just ref to buf, EFalse if we own it => we must also delete it
        TBool iSrcBufRef;

    	// Mutex used to use variables safely in case of use from other thread
        RMutex      iMutexObj; 
        TBool       iMutexCreated;

        // Audio codec-specific data is available from this object
        CCMRAudioCodecData* iCodecData;

        // Time stamp when paused; pause resets the original ones since it is actually stop
        TTimeIntervalMicroSeconds iTimeStampWhenPaused;
        // Number of audio frames received
        TInt iNumFramesReceived;
        // Number of audio frames stored in the queue and waiting for output to mediasink
        TInt iNumFramesWaiting;

        // Counter for sent buffers, used also in SW-variant to evaluate timeout for next buffer (are we ahead or late)
        TInt iSendBufCount;

    	TMMFPrioritySettings iPrioritySettings;

        // Reference to the buffer under processing
        CMMFDataBuffer* iProcessingBuffer;

    private: // data internal for this base class

        // Index of the next unread byte in iSrcBuffer (data=origin+index)
        TInt iProcessingIndex;
        // Number of unread bytes in iSrcBuffer
        TInt iSrcNumUnreadBytes;
        // Status info for output active object
        TRequestStatus* iSinkRequestStatus;

        // Buffer for data output to iOutput
        CCMRMediaBuffer* iMediaSinkBuffer;
        // CamcorderMMFPlugin config manager
        CCMRConfigManager* iConfig;
        // ETrue if iOutputBuffer is in use (supports only 1 output buffer at a time)
        TBool iOutputBufferInUse;

        // thread handle for output active object
        RThread iOutputThreadHandle;

        // Decoder configuration information for some codecs
        HBufC8* iDecConfigInfo;
        // EFalse until 1st buffer has been read by active output; is used to check if we need to provide decoder config info first
        TBool iDecConfigInfoChecked;

        TBool iSamplesUpdatedAfterStart;
        TTime iTimeWhenSamplesUpdated;
        TTimeIntervalMicroSeconds iLatestTimeStamp;
    };

#endif      
            
// End of File



