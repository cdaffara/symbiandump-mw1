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
* Description:  3GP data sink implementation class
*
*/


#ifndef __CAMC3GPDATASINKIMP_H__
#define __CAMC3GPDATASINKIMP_H__

//  INCLUDES

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>
#include <3gplibrary/mp4lib.h>
#include <mmf/common/mmffourcc.h>

#include "CCMRMediaSink.h"
#include "CamC3GPDataSink.h"

// FORWARD DECLARATIONS
class CMMFFile;

// CLASS DECLARATION

/**
*  CCamC3GPDataSinkImp class.
*  This class implements the 3GP data sink methods.
*
*  @lib camc3gpsink
*  @since 2.1
*/
class CCamC3GPDataSinkImp : public CCamC3GPDataSink, public M3GPMP4LibAsyncTempFileRemoverObserver
    {
    public: // Methods called internally or by the controller

        /**
        * Default constructor.
        */
        CCamC3GPDataSinkImp() : CCamC3GPDataSink() {};

        /**
        * Destructor.
        */
		~CCamC3GPDataSinkImp(void);

        /**
        * Constructor.
        */
    	void ConstructL(M3GPDataSinkObserver *aObserver);

        /**
        * Open a file for writing.
        * @since 2.1
        * @param aFileName File name
        * @param aAudioCodecType Four CC code of the used audio codec
        * @param aVideoCodecType Four CC code of the used video codec
        * @param aFileFormat Used file format TCamCSinkFileFormat
        * @return No return value
        */
        void OpenFileL(TFileName aFileName, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat );

        /**
        * Open a 3GP file for writing.
        * @since 3.2
        * @param aMMFFile CMMFFile from MMF
        * @param aAudioCodecType Four CC code of the used audio codec
        * @param aVideoCodecType Four CC code of the used video codec
        * @param aFileFormat Used file format TCamCSinkFileFormat
        * @return No return value
        */
        void OpenFileL(CMMFFile* aMMFFile, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat );

        /**
        * Order the sink to finalize and close the current 3GP file.
        * @since 2.1
        * @param None
        * @return No return value
        */
        void SinkStopL();

        /**
        * Set size limit of the 3GP file to be recorded in bytes.
        * @since 2.1
        * @param aSize Limit
        * @return No return value
        */
        void SetSizeLimit(TUint aSize);

        /**
        * Return the amount of time recording has been on in microseconds.
        * @since 2.1
        * @param None
        * @return Elapsed time
        */
        TTimeIntervalMicroSeconds GetElapsedTime();

        /**
        * Return the estimated remaining time for the recording in microseconds.
        * @since 2.1
        * @param None
        * @return Remaining time
        */
        TTimeIntervalMicroSeconds GetRemainingTimeL();


    public: // Methods called from MCMRMediaSink

        /**
        * Give video frame size to sink.
        * @since 2.1
        * @param aSize Frame size
        * @return Error code
        */
        TInt SetVideoFrameSize(TSize aSize);

        /**
        * Give average video bitrate to sink.
        * @since 2.1
        * @param aBitRate Average bitrate
        * @return Error code
        */
        TInt SetAverageVideoBitRate(TInt aBitRate);

        /**
        * Give maximum video bitrate to sink.
        * @since 2.1
        * @param aBitRate Maximum bitrate
        * @return Error code
        */
        TInt SetMaxVideoBitRate(TInt aBitRate);

        /**
        * Give average audio bitrate to sink.
        * @since 2.1
        * @param aBitRate Average bitrate
        * @return Error code
        */
        TInt SetAverageAudioBitRate(TInt aBitRate);

        /**
        * Write an audio/video buffer to the sink.
        * @since 2.1
        * @param aBuffer Buffer containing audio/video data
        * @return Errorcode
        */
        void WriteBufferL(CCMRMediaBuffer* aBuffer);


    public: // Methods from M3GPMP4LibAsyncTempFileRemoverObserver
        void M3GPMP4LibDeleteTempFileName( MP4FileName tempFileName );

    private: // Private methods

        /**
        * Estimate current file size
        * @since 2.1
        * @param None
        * @return File size
        */
        TUint CurrentFileSize() const;

        /**
        * Estimate current metadata size
        * @since 2.1
        * @param None
        * @return File size
        */
        TUint CurrentMetadataSize() const;

        /**
        * Calculate free space on a drive in bytes.
        * @since 2.1
        * @param None
        * @return File size
        */
        TInt64 DriveFreeSpaceL();

        /**
        * Determines AAC audio frame duration.
        * @since 2.8
        * @param Incoming buffer with audio dec spec info
        * @return None
        */
        void DetermineAACFrameDurationL(CCMRMediaBuffer* aBuffer);

        /**
        * Convert AVC specific decoder config info from NAL (GenericPayload) encapsulation to
        * AVC Decoder Configuration Record -format
        * @since 3.2
        * @param Source buffer with AVC specific decoder config info.
        * @param Destination buffer to write AVC Decoder Configuration Record.
        * @return None
        */
        void ConvertAVCHeaderNALL( CCMRMediaBuffer* aBuffer, TDes8& aDstBuf );

        /**
        * Convert AVC specific decoder config info from Bytestream (ElementaryStream) encapsulation to
        * AVC Decoder Configuration Record -format
        * @since 3.2
        * @param Source buffer with AVC specific decoder config info.
        * @param Destination buffer to write AVC Decoder Configuration Record.
        * @return None
        */
        void ConvertAVCHeaderByteStreamL( CCMRMediaBuffer* aBuffer, TDes8& aDstBuf );

        /**
        * Converts AVC frame from Bytestream (ElementaryStream) encapsulation to
        * file format AVC sample structure by replacing bytestream headers with NAL unit sizes.
        * @since 5.0
        * @param Source buffer with one or more bytestream header seperated NAL units.
        * @return None
        */
        void ConvertBytestreamHeadersToNALSizes( CCMRMediaBuffer* aBuffer );

        /**
        * Converts AVC frame from NAL (EGenericPayload) encapsulation to
        * file format AVC sample structure by replacing NAL encapsulation with NAL unit sizes.
        * @since 5.0
        * @param Source buffer with one or more NAL header seperated NAL units.
        * @param Destination buffer to write NAL units with length information.
        * @param Destination buffer size.
        * @return None
        */
        void ConvertNALEncapsulationToNALSizes( CCMRMediaBuffer* aBuffer );
        
        /**
        * Compose UDTA (User Data) to video file
        * @since 5.2
        * @param None
        * @return None
        */
        void ComposeUDTAL();
        
        /**
        * Convert 32bit unsigned value to little endian format into buffer
        * @since 5.2
        * @param None
        * @return None
        */
        void LittleEndianPut32(TUint8 *aPtr, TUint32 aVal);

        /**
        * Convert 16bit unsigned value to little endian format into buffer
        * @since 5.2
        * @param None
        * @return None
        */
        void LittleEndianPut16(TUint8 *aPtr, TUint16 aVal);

    private:

        static TInt IdleDelete( TAny* aCont );

    public:

        TInt DoIdleDelete();

    private:

        // Observer to inform controller
        M3GPDataSinkObserver *iObserver;

        // 3GP library handle
        MP4Handle   iMP4Handle;

        // Number of bytes received from media recorder
        TUint       iBytesReceived;

        // Number of bytes of metadata currently accumulated
        TUint       iBytesOfMetadata;

        // Horizontal resolution of video
        TInt        iVideoXResolution;

        // Vertical resolution of video
        TInt        iVideoYResolution;

        // Average bitrate of video
        TInt        iVideoAverageBitRate;

        // Maximum bitrate of video
        TInt        iVideoMaxBitRate;

        // Average bitrate of audio
        TInt        iAudioAverageBitRate;

        // Type of buffer coming from media recorder
        CCMRMediaBuffer::TBufferType iBufferType;

        // Size of buffer coming from media recorder
        TInt        iBufferSize;

        // Types of codecs used in the output file
        TUint       iFileCodecType;

        // Timestamp of video frame coming from media recorder
        TTimeIntervalMicroSeconds   iVideoTimestamp;

        // Timestamp of video frame buffered in the sink
        TTimeIntervalMicroSeconds   iVideoBufferTimestamp;

        // Timestamp of the first video frame
        TTimeIntervalMicroSeconds   iFirstVideoFrameTimestamp;

        // Duration of the video frame buffered in the sink
        TUint       iVideoFrameDuration;

        // Is current video frame a random access point
        TBool       iVideoRandomAccessPoint;

        // Is the buffered video frame a random access point
        TBool       iVideoBufferRandomAccessPoint;

        // Sink buffers one video frame in this buffer
        TUint8*     iVideoBuffer;

        // Size of buffered video frame
        TUint       iVideoBufferFrameSize;

        // Size of video buffer
        TUint       iVideoBufferSize;

        // Number of video frames
        TUint       iVideoFrameNumber;

        // Number of intra video frames
        TUint       iVideoIntraFrameNumber;

        // Video decoder specific info size
        TInt        iVideoDecSpecInfoSize;

        // Audio decoder specific info size
        TInt        iAudioDecSpecInfoSize;

        // Sink buffers audio frames in this buffer
        TUint8*     iAudioBuffer;

        // Size of buffered audio frames
        TUint       iAudioBufferFrameSize;

        // Size of audio buffer
        TUint       iAudioBufferSize;

        // Number of audio frames
        TUint       iAudioFrameNumber;

        // Number of audio frames in the buffer
        TUint       iAudioFramesInBuffer;

        // Size limit of the file in bytes
        TInt64       iSizeLimit;

        // Has file size limit bee reached?
        TBool       iFileSizeLimitReached;

        // File server session handle
        RFs*        iFS;

        // Drive number of the output file
        TInt        iDriveNumber;

        // Has the disk been filled to the safety limit?
        TBool       iDiskFull;

        // Output filename
        TFileName   iFileName;

        // Temporary filename
        TFileName   iTmpFileName;

        // MMF resource to acquire file handle
        CMMFFile* iMMFFile;

        // File handle from MMF
        RFile * iFile;

        // Used to determine whether file handles or descriptors are used
        TBool iFileHandleExists;

        // Amount of remaining recording time
        TTimeIntervalMicroSeconds   iRemainingTime;

        // Remaining free disk space
        TInt64      iFreeDiskSpace;

        // Counter to check the real free disk space
        TInt        iFreeDiskSpaceCounter;

        // Available space on disk at start of recording
        TInt64  iAvailableSpaceAtStart;

        // Avarage of end time
        TTimeIntervalMicroSeconds  iAvarageEndTime;

        // Duration of AAC frame
        TInt iAudioAACFrameDuration;

        // AAC Audio Samplerate
        TInt iAudioAACSamplerate;

        // AVC video level id
        TInt iAVCOutputLevel;

        // Critical level for current target disk
        TInt iCriticalDiskVal;

        // File name queue for the temporary meta data files to be deleted.
        RPointerArray<MP4FileName>* iDeleteFileQueue;

        // Async file deletion on background
        CIdle* iIdleDelete;

    };

#endif
