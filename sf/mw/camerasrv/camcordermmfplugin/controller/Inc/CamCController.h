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
* Description:  Camcorder controller plugin
*
*/



#ifndef CAMCCONTROLLER_H
#define CAMCCONTROLLER_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <ecom/implementationproxy.h>

#include <mmf/common/mmfcontroller.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#include "CamC3GPDataSinkObserver.h"
#include "CCMRMediaRecorder.h"
#include "CamC3GPDataSink.h"

// FORWARD DECLARATION
class CDesC8ArrayFlat;
class CMMFFile;

// CLASS DECLARATION

/**
*  Camcorder controller plugin
*
*  @lib camccontroller.lib
*  @since 2.1
*/
class CCamCController : public CMMFController, 
                        public MCMRMediaRecorderObserver,
                        public M3GPDataSinkObserver,
                        public MMMFVideoControllerCustomCommandImplementor, 
                        public MMMFVideoRecordControllerCustomCommandImplementor, 
                        public MMMFAudioRecordDeviceCustomCommandImplementor
    {
    public:

        /**
        * Controller internal states
        */
        enum TCamCControllerState
            {
            EStateNone = 0, 
            EStateOpen,
            EStatePrepared,
            EStateRecording,
            EStatePaused,
            EStateStopping
            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCamCController* NewL();

        /**
        * Destructor.
        */
        virtual ~CCamCController();

    private:

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

    protected:  // Functions from base classes

        /**
        * From CMMFController Add data source to controller.
        * @since 2.1
        * @param aDataSource A reference to the data source.
        * @return void
        */
        void AddDataSourceL(MDataSource& aDataSource);
    
        /**
        * From CMMFController Add data sink to controller.
        * @since 2.1
        * @param aDataSink A reference to the data sink.
        * @return void
        */
        void AddDataSinkL(MDataSink& aDataSink);

        /**
        * From CMMFController Remove data source from controller.
        * @since 2.1
        * @param  aDataSource A reference to the data source.
        * @return void
        */
        void RemoveDataSourceL(MDataSource& aDataSource);

        /**
        * From CMMFController Remove data sink from controller.
        * @since 2.1
        * @param aDataSink A reference to the data sink.
        * @return void
        */
        void RemoveDataSinkL(MDataSink& aDataSink);

        /**
        * From CMMFController Reset controller.
        * @since 2.1
        * @param void
        * @return void
        */
        void ResetL();

        /**
        * From CMMFController  Primes controller.
        * @since 2.1
        * @param void
        * @return void
        */
        void PrimeL();

        /**
        * From CMMFController Start recording.
        * @since 2.1
        * @param void
        * @return void
        */
        void PlayL();

        /**
        * From CMMFController Pause recording.
        * @since 2.1
        * @param void
        * @return void
        */
        void PauseL();

        /**
        * From CMMFController Stop recording.
        * @since 2.1
        * @param void
        * @return void
        */
        void StopL();

        /**
        * From CMMFController Returns current recording position. Not supported
        * DurationL() used instead.
        * @since 2.1
        * @param void
        * @return Current position of recording.
        */
        TTimeIntervalMicroSeconds PositionL() const;

        /**
        * From CMMFController Sets current recording position. Not supported.
        * @since 2.1
        * @param aPosition Reference to wanted position.
        * @return void
        */
        void SetPositionL(const TTimeIntervalMicroSeconds& aPosition);

        /**
        * From CMMFController Returns current duration of recording.
        * @since 2.1
        * @param void 
        * @return Current duration of recording.
        */
        TTimeIntervalMicroSeconds DurationL() const;

        /**
        * From CMMFController Set camcorder priority settings.
        * @since 2.1
        * @param aPrioritySettings Wanted priority.
        * @return void
        */
        void SetPrioritySettings(const TMMFPrioritySettings& aPrioritySettings);

        /**
        * From CMMFController Handle custom commands to controller.
        * @since 2.1
        * @param aMessage Message to controller.
        * @return void
        */
        void CustomCommand(TMMFMessage& aMessage);

        /**
        * From CMMFController Get number of metadata entries.
        * @since 2.1
        * @param aNumberOfEntries Reference used to return metadata count.
        * @return void
        */
        void GetNumberOfMetaDataEntriesL(TInt& aNumberOfEntries);

        /**
        * From CMMFController Returns metadata entry.
        * @since 2.1
        * @param aIndex Index to metadata entry.
        * @return Metadata entry
        */
        CMMFMetaDataEntry* GetMetaDataEntryL(TInt aIndex);

    public: // from MCMRMediaRecorderObserver

        /**
        * From MCMRMediaRecorderObserver Mediarecorder has completed prepare.
        * @since 2.1
        * @param aError Error code from mediarecorder.
        * @return void
        */      
        void MmroPrepareComplete(TInt aError);    

        /**
        * From MCMRMediaRecorderObserver  Mediarecorder has changed state.
        * @since 2.1
        * @param aState Current mediarecorder state.
        * @param aError Error code from mediarecorder.
        * @return void
        */      
        void MmroStateChange(TInt aState, TInt aError);

        /**
        * From MCMRMediaRecorderObserver Temporary error in mediarecorder.
        * @since 2.1
        * @param aError Error code from mediarecorder.
        * @return void
        */      
        void MmroTemporaryError(TInt aError);

        /**
        * From MCMRMediaRecorderObserver Fatal error in mediarecorder.
        * @since 2.1
        * @param aError Error code from mediarecorder.
        * @return void
        */
        void MmroFatalError(TInt aError);

    public: // from M3GPDataSinkObserver

        /**
        * From M3GPDataSinkObserver Filecomposer has run out of disk space.
        * @since 2.1
        * @param void
        * @return void
        */
        void MfcoDiskFullL();

        /**
        * From M3GPDataSinkObserver Filecomposer has met maximum file size.
        * @since 2.1
        * @param void
        * @return void
        */
        void MfcoSizeLimitReachedL();

    public: // from MMMFVideoControllerCustomCommandImplementor

        /**
        * From MMMFVideoControllerCustomCommandImplementor Get video frame size.
        * @since 2.1
        * @param aVideoFrameSize Reference to return video frame size.
        * @return void
        */
        void MvcGetVideoFrameSizeL(TSize& aVideoFrameSize);

        /**
        * From MMMFVideoControllerCustomCommandImplementor Get audio codec.
        * @since 2.1
        * @param aCodec Reference to return audio codec.
        * @return void
        */
        void MvcGetAudioCodecL(TFourCC& aCodec);

        /**
        * From MMMFVideoControllerCustomCommandImplementor Get video bit rate.
        * @since 2.1
        * @param aBitRate Reference to return video bit rate.
        * @return void
        */
        void MvcGetVideoBitRateL(TInt& aBitRate);

        /**
        * From MMMFVideoControllerCustomCommandImplementor Get audio bit rate.
        * @since 2.1
        * @param aBitRate Reference to return audio bit rate.
        * @return void
        */
        void MvcGetAudioBitRateL(TInt& aBitRate);

        /**
        * From MMMFVideoControllerCustomCommandImplementor Set video frame rate.
        * @since 2.1
        * @param aFramesPerSecond Video frame rate.
        * @return void
        */
        void MvcSetFrameRateL(TReal32 aFramesPerSecond);

        /**
        * From MMMFVideoControllerCustomCommandImplementor Get video frame rate.
        * @since 2.1
        * @param aFramesPerSecond Reference to return video frame rate.
        * @return void
        */
        void MvcGetFrameRateL(TReal32& aFramesPerSecond);

        /**
        * From MMMFVideoControllerCustomCommandImplementor 
        * @since 2.1
        * @param Get video mime type.
        * @return void
        */
        void MvcGetVideoMimeTypeL(TDes8& aMimeType);

    public: // from MMMFVideoRecordControllerCustomCommandImplementor
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set video format.
        * @since 2.1
        * @param aVideoFormatUid Video format.
        * @return void
        */
        void MvrcSetVideoFormatL(TUid aVideoFormatUid);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set video codec.
        * @since 2.1
        * @param aVideoCodec Video codec.
        * @return void
        */
        void MvrcSetVideoCodecL(const TDesC8& aVideoCodec);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set audio codec.
        * @since 2.1
        * @param aAudioCodec Audio codec.
        * @return void
        */
        void MvrcSetAudioCodecL(TFourCC aAudioCodec);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set video bit rate.
        * @since 2.1
        * @param aBitRate Video bit rate.
        * @return void
        */
        void MvrcSetVideoBitRateL(TInt& aBitRate);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set audio bit rate.
        * @since 2.1
        * @param aBitRate Audio bit rate.
        * @return void
        */
        void MvrcSetAudioBitRateL(TInt& aBitRate);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Add metadata entry.
        * @since 2.1
        * @param aNewEntry Metadata entry.
        * @return void
        */
        void MvrcAddMetaDataEntryL(const CMMFMetaDataEntry& aNewEntry);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Remove metadata entry.
        * @since 2.1
        * @param aIndex Index to metadata.
        * @return void
        */
        void MvrcRemoveMetaDataEntryL(TInt aIndex);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Replace metadata entry.
        * @since 2.1
        * @param aIndex Index to metadata.
        * @param aNewEntry Metadata entry.
        * @return void
        */
        void MvrcReplaceMetaDataEntryL(TInt aIndex, const CMMFMetaDataEntry& aNewEntry);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set maximum file size.
        * @since 2.1
        * @param aMaxFileSize Maximum file size.
        * @return void
        */
        void MvrcSetMaxFileSizeL(TInt aMaxFileSize);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor  Set audio enabled/disabled.
        * @since 2.1
        * @param aEnable Boolean to enable/disable audio. ETrue to enable.
        * @return void
        */
        void MvrcSetAudioEnabledL(TBool aEnable);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set video frame size.
        * @since 2.1
        * @param aFrameSize Video frame size.
        * @return void
        */
        void MvrcSetVideoFrameSizeL(TSize aFrameSize);
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Prepare camcorder for recording.
        * @since 2.1
        * @param void
        * @return void
        */
        void MvrcPrepareL();
        
        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Set camera handle.
        * @since 2.1
        * @param aCameraHandle Camera handle.
        * @return void
        */
        void MvrcSetCameraHandleL(TInt aCameraHandle);

        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Returns available recording time.
        * @since 2.1
        * @param aTime Available record time
        * @return void
        */
        void MvrcGetRecordTimeAvailableL(TTimeIntervalMicroSeconds& aTime);

        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Get supported audio sink types.
        * @since 2.1
        * @param aDataTypes Reference to return supported audio sink types.
        * @return void
        */
        void MvrcGetSupportedSinkAudioTypesL(RArray<TFourCC>& aDataTypes);

        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Get supported video sink types.
        * @since 2.1
        * @param aDataTypes Reference to return supported video sink types.
        * @return void
        */
        void MvrcGetSupportedSinkVideoTypesL(CDesC8Array& aDataTypes);

        /**
        * From MMMFVideoRecordControllerCustomCommandImplementor Get audio enabled
        * @since 2.1
        * @param aDataTypes Reference to return whether audio is enabled.
        * @return void
        */
        void MvrcGetAudioEnabledL(TBool& aEnabled);


    public: //from MMMFAudioRecordDeviceCustomCommandImplementor

        /**
        * From MMMFAudioRecordDeviceCustomCommandImplementor Set gain.
        * @since 2.1
        * @param aGain Gain value.
        * @return void
        */
        void MardSetGainL(TInt aGain);

        /**
        * From MMMFAudioRecordDeviceCustomCommandImplementor Get maximum gain.
        * @since 2.1
        * @param aMaxGain Maximum gain value.
        * @return void
        */
        void MardGetMaxGainL(TInt& aMaxGain);

        /**
        * From MMMFAudioRecordDeviceCustomCommandImplementor Get gain.
        * @since 2.1
        * @param aGain Reference to return gain.
        * @return void
        */
        void MardGetGainL(TInt& aGain);

        /**
        * From MMMFAudioRecordDeviceCustomCommandImplementor Set balance. Not supported.
        * @since 2.1
        * @param aBalance Balance value.
        * @return void
        */
        void MardSetBalanceL(TInt aBalance);

        /**
        * From MMMFAudioRecordDeviceCustomCommandImplementor Get balance. Not supported.
        * @since 2.1
        * @param aBalance Reference to return balance.
        * @return void
        */
        void MardGetBalanceL(TInt& aBalance);

    private:
        
        /**
        * Set new filename from custom command.
        * @since 2.1
        * @param aMessage Message to controller.
        * @return void
        */
        void NewFilenameL( TMMFMessage& aMessage );

        /**
        * Check if controller is ready to open mediarecorder and filecomposer.
        * @since 2.1
        * @param void
        * @return ETrue if ready, EFalse if not.
        */
        TBool ReadyToOpenSource();

        /**
        * Open mediarecorder adn filecomposer.
        * @since 2.1
        * @param void
        * @return void
        */
        void OpenSourceL();
        
    private:
    
        static TInt IdleStop( TAny* aCont ); 

    public: 

        void DoIdleStop();  


    private: // Data
        // Supported Video codecs
        CDesC8ArrayFlat* iSupportedVideoTypes;

        // Supported Audio codecs
        RArray<TFourCC> iSupportedAudioTypes;

        // Controller internal state
        TCamCControllerState iState;

        // Video codec
        HBufC8* iVideoCodec;

        // Audio codec
        TFourCC iAudioCodec;

        // Video file format
        CCamC3GPDataSink::TCamCSinkFileFormat iVideoFormat;

        // Camera handle
        TInt iCameraHandle;

        // Saved error code in case of internal stop.
        TInt iSavedStopError;
        
        // Whether current stop is asynchronous.
        TBool iAsyncStop;

        // Recorder file name
        TFileName iFileName;
        
        // MMF file sink
        CMMFFile* iMMFFile;
        
        // Whether Data Sink is performed via RFiles or Descriptors
        TBool iForceDataSinkFileName;

        // Whether composer is ready to receive buffers (OpenFileL called)
        TBool iFileComposerReady;
        
        // Whether mediarecorder complete message has arrived and preparecomplete event is sent to client.
        // Used for parallel preparing of filecomposer and mediarecorder.
        TBool  iMRPrepareCompleteSent;

        TMMFPrioritySettings iPrioritySettings;

        MDataSource* iAudioSource;

        // Mediarecorder
        CCMRMediaRecorder* iMediaRecorder;  // Data source

        // Filecomposer
        CCamC3GPDataSink* iFileComposer;
        
        CIdle* iIdleStop;

    };

#endif      // CAMCCONTROLLER_H  
            
// End of File
