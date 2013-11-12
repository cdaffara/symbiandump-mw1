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
* Description:  3GP data sink class definition
*
*/



#ifndef __CAMC3GPDATASINK_H__
#define __CAMC3GPDATASINK_H__

//  INCLUDES

#include <e32base.h>
#include <e32std.h>

#include <mmf/common/mmffourcc.h>

#include "CCMRMediaSink.h"
#include "CamC3GPDataSinkObserver.h"

// FORWARD DECLARATIONS
class CMMFFile;

// CLASS DECLARATIONS

/**
*  CCamC3GPDataSink class
*  This is the abstract 3GP data sink class that is used by
*  CCamC3GPDataSinkImp class.
*
*  @lib camc3gpsink
*  @since 2.1
*/
class CCamC3GPDataSink : public CBase, public MCMRMediaSink
    {
    public:
        // File format
        enum TCamCSinkFileFormat
            {
            E3GPP,
            E3GPP2,
            EMPEG4
            };

    public: // Methods called internally or by the controller

        /**
        * Default constructor.
        */
		CCamC3GPDataSink() {};

        /**
        * Constructor.
        */
	    IMPORT_C static CCamC3GPDataSink* NewL(M3GPDataSinkObserver *aObserver);

        /**
        * Open a 3GP file for writing.
        * @since 2.1
        * @param aFileName File name
        * @param aAudioCodecType Four CC code of the used audio codec
        * @param aVideoCodecType Four CC code of the used video codec
        * @param aFileFormat Used file format TCamCSinkFileFormat
        * @return No return value
        */
        virtual void OpenFileL(TFileName aFileName, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat = E3GPP) = 0;

        /**
        * Open a 3GP file for writing.
        * @since 3.2
        * @param aMMFFile CMMFFile from MMF
        * @param aAudioCodecType Four CC code of the used audio codec
        * @param aVideoCodecType Four CC code of the used video codec
        * @param aFileFormat Used file format TCamCSinkFileFormat
        * @return No return value
        */
        virtual void OpenFileL(CMMFFile* aMMFFile, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat = E3GPP) = 0;

        /**
        * Order the sink to finalize and close the current 3GP file.
        * @since 2.1
        * @param None
        * @return No return value
        */
        virtual void SinkStopL() = 0;

        /**
        * Set size limit of the 3GP file to be recorded in bytes.
        * @since 2.1
        * @param aSize Limit
        * @return No return value
        */
        virtual void SetSizeLimit(TUint aSize) = 0;

        /**
        * Return the amount of time recording has been on in microseconds.
        * @since 2.1
        * @param None
        * @return Elapsed time
        */
        virtual TTimeIntervalMicroSeconds GetElapsedTime() = 0;

        /**
        * Return the estimated remaining time for the recording in microseconds.
        * @since 2.1
        * @param None
        * @return Remaining time
        */
        virtual TTimeIntervalMicroSeconds GetRemainingTimeL() = 0;


    public: // Methods called from MCMRMediaSink

        /**
        * Give video frame size to sink.
        * @since 2.1
        * @param aSize Frame size
        * @return Error code
        */
        virtual TInt SetVideoFrameSize(TSize aSize) = 0;

        /**
        * Give average video bitrate to sink.
        * @since 2.1
        * @param aBitRate Average bitrate
        * @return Error code
        */
        virtual TInt SetAverageVideoBitRate(TInt aBitRate) = 0;

        /**
        * Give maximum video bitrate to sink.
        * @since 2.1
        * @param aBitRate Maximum bitrate
        * @return Error code
        */
        virtual TInt SetMaxVideoBitRate(TInt aBitRate) = 0;

        /**
        * Give average audio bitrate to sink.
        * @since 2.1
        * @param aBitRate Average bitrate
        * @return Error code
        */
        virtual TInt SetAverageAudioBitRate(TInt aBitRate) = 0;

        /**
        * Write an audio/video buffer to the sink.
        * @since 2.1
        * @param aBuffer Buffer containing audio/video data
        * @return Errorcode
        */
        virtual void WriteBufferL(CCMRMediaBuffer* aBuffer) = 0;
   };

#endif
