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
* Description:  Definitions for a media sink object
*
*/


#ifndef CCMRMEDIASINK_H
#define CCMRMEDIASINK_H

#include <e32base.h>

// CLASS DECLARATION

/**
*  Buffer class used in MCMRMediaSink
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRMediaBuffer : public CBase
    {
    public:  // Constants

        // Buffer type
        enum TBufferType 
            {
            EAudioAMRNB = 0,
            EAudioAMRWB,            
            EVideoH263,
            EVideoMPEG4,
            EVideoMPEG4DecSpecInfo,
            EAudioDecSpecInfo = 6,
            EAudioMPEG4AAC,
            EVideoH264NAL,
            EVideoH264Bytestream,
            EVideoH264NALDecSpecInfo,
            EVideoH264BytestreamDecSpecInfo
            };        

    public:  // Constructors and destructor

        /**
        * C++ constructors. The 1st creates a buffer to be initialized later with Set, 
        * and the 2nd one initializes it already here
        */

        inline CCMRMediaBuffer() {}

        inline CCMRMediaBuffer(const TPtrC8& aBuffer, 
                        const TBufferType aType, 
                        const TInt aSize, 
                        const TBool aRandomAccessPoint,
                        const TTimeIntervalMicroSeconds& aTimeStamp) 
                        : iType(aType), iSize(aSize), iRandomAccessPoint(aRandomAccessPoint), iTimeStamp(aTimeStamp), iPtr(aBuffer) {}

        /**
        * Destructor.
        */
        inline ~CCMRMediaBuffer() {}

    public:  // New functions

        /**                     
        * Set the buffer parameters. This enables reusing the buffer
        * @since 2.1
        * @param const TPtrC8& aBuffer                          Data buffer
        * @param const TBufferType aType                        Buffer type
        * @param const TInt aSize                               Buffer size (in bytes)
        * @param const TBool aRandomAccessPoint                 ETrue if buffer has a video random access point
        * @param const TTimeIntervalMicroSeconds& aTimeStamp    Timestamp of the buffer
        * @return void
        */
        inline void Set(const TPtrC8& aBuffer, 
                        const TBufferType aType, 
                        const TInt aSize, 
                        const TBool aRandomAccessPoint,
                        const TTimeIntervalMicroSeconds& aTimeStamp)
            {
            iType = aType;
            iSize = aSize;
            iRandomAccessPoint = aRandomAccessPoint;
            iTimeStamp = aTimeStamp;
            iPtr.Set(aBuffer);
            }

        /**                     
        * Return the data pointer
        * @since 2.1
        * @return TPtrC8 pointer
        */
        inline TPtrC8 Data() const { return iPtr; }

        /**
        * Return buffer type
        * @since 2.1
        * @return TInt Error code
        */
        inline TBufferType Type() const { return iType; } 

        /**
        * Write Return buffer size
        * @since 2.1
        * @return TInt Error code
        */
        inline TInt BufferSize() const { return iSize; }

        /**
        * Query whether this buffer contains a frame that can be used as a random-access point
        * @since 2.1
        * @return TBool ETrue if this is a random access point
        */
        inline TBool RandomAccessPoint() const { return iRandomAccessPoint; } 

        /**
        * Write a buffer to sink
        * @since 2.1
        * @return TTimeIntervalMicroSeconds Timestamp
        */
        inline TTimeIntervalMicroSeconds TimeStamp() const { return iTimeStamp; }

    private:  // Data

        // media type
        TBufferType iType;         

        // media buffer size in bytes
        TInt iSize;                

        // ETrue if the media contained in this buffer
        // can be used as random access point in playback        
        TBool iRandomAccessPoint;  
                                   
        // time stamp in microseconds counting from the beginning of rec.
        TTimeIntervalMicroSeconds iTimeStamp;           

        // descriptor for returning the data pointer
        TPtrC8 iPtr;               
    };


// CLASS DECLARATION

/**
*  Abstract Media sink interface
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class MCMRMediaSink
    {
        
    public:

        /**
        * Write a buffer to sink
        * @since 2.1
        * @param CCMRMediaBuffer* aBuffer   The buffer to write
        * @return TInt                      Error code
        */
        virtual void WriteBufferL(CCMRMediaBuffer* aBuffer) = 0;

        /**
        * Inform video frame size to sink
        * @since 2.1
        * @param TSize aSize    Video frame size
        * @return TInt          Error code
        */
        virtual TInt SetVideoFrameSize(TSize aSize) = 0;

        /**
        * Inform average video bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Average video bitrate
        * @return TInt          Error code
        */
        virtual TInt SetAverageVideoBitRate(TInt aBitRate) = 0;

        /**
        * Inform maximum video bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Maximum expected video bitrate
        * @return TInt          Error code
        */
        virtual TInt SetMaxVideoBitRate(TInt aBitRate) = 0;

        /**
        * Inform average audio bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Average audio bitrate
        * @return TInt          Error code
        */
        virtual TInt SetAverageAudioBitRate(TInt aBitRate) = 0;
    };


#endif // CCMRMEDIASINK_H


// End of file
