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
* Description:  Header file for AMR audio codec data class
*
*/


#ifndef CCMRAACCODECDATA_H
#define CCMRAACCODECDATA_H

// INCLUDES
#include "CCMRAudioCodecData.h"
#include "CCMRSupportedCodecs.h"


// CONSTANTS
// this is the max buffer size that the corresponding HW codec implementation will output, so it is not necessarily == 1 framelength
const TInt KCMRMaxAACBufferLength = 240;

// UId of the AAC CMMFCodec
const TUid KCMRAACSWCodecUid = {0x1020382F};    //KAdvancedUidCodecPCM16ToAAC


// CLASS DECLARATION


/**
*  Base class for audio codec data
*
*  @lib
*  @since 2.6
*/
class CCMRAACCodecData : public CCMRAudioCodecData
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CCMRAACCodecData(TCMRCodecType aType);

        /**
        * Destructor.
        */
        virtual ~CCMRAACCodecData();

    public: // functions from baseclass

        /**
        * From CCMRAudioCodecData Check if the codec is supported in this configuration
        * @return TBool ETrue if supported
        */
        TBool Supported();

        /**
        * Return frame duration in microseconds
        * @return TReal duration in microseconds
        */
        TReal GetFrameDurationUs() const;

        /**
        * Return preferred duration of PCM capture in microseconds, relevant only for CMMFCodecs
        * @return TReal duration in milliseconds
        */
        TReal GetPCMCaptureDurationMs() const;

        /**
        * From CCMRAudioCodecData Set bitrate for the codec
        * @param aBitRate in bps
        * @return void
        */
        void SetBitRateL(TInt aBitRate);

        /**
        * From CCMRAudioCodecData Get the current bitrate
        * @return TInt bitrate in bps
        */
        TInt GetBitRateL() const;

        /**
        * Set sample rate for the codec, leaves by default, but can be overridden, then leaves if not supported
        */
        void SetSampleRateL(TUint aSampleRate);

        /**
        * Return the supported sample rate for the codec
        * @return TUint sample rate (Hz)
        */
        TUint GetSampleRate() const;

        /**
        * Set channel mode for the codec, leaves by default.
        */
        void SetChannelModeL(TUint aChannelMode);

        /**
        * From CCMRAudioCodecData Get the max frame length (single coded frame)
        * @return TInt framelength in bytes
        */
        TInt MaxFrameLengthL() const;

        /**
        * From CCMRAudioCodecData Get the configuration parameters for the codec
        * @param aDataSource    MMF audio input
        * @return TDes8* configuration parameter structure
        */
        TDes8* GetCodecConfigParamL(MDataSource* aDataSource);

        /**
        * From CCMRAudioCodecData Reads the framelength from the header of the given buffer
        * @param const TUint8* aBuf
        * @return TInt frame length in bytes
        */
        TInt FrameLength( const TUint8* aBuf, TInt aDataLen ) const;

        /**
        * From CCMRAudioCodecData Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
        * @return TInt Max frame length in bytes
        */
        TInt inline MaxBufferLength() const { return KCMRMaxAACBufferLength; };

        /**
        * From CCMRAudioCodecData Configures sample DevSound HW device
        * @since 2.1
        * @return TInt error code
        */
        TInt AudioHWConfigL(MDataSource* aDataSource, const TDesC8& aParams );

        /**
        * From CCMRAudioCodecData Get the decoder configuration info
        * @return HBufC8* info structure
        */
        HBufC8* GetDecoderConfigInfoLC();

        /**
        * From CCMRAudioCodecData Configures sample DevSound HW device
        * @return TUid Uid
        */
        inline TUid SWCodecUid() {return KCMRAACSWCodecUid;} ;

        /**
        * From CCMRAudioCodecData Preferred samplecount per inputbuffer.
        * @return TInt samplecount.
        */
        TInt PreferredSampleCountPerInputBuffer();

        /**
        * Get the preferred framecount per PCM inputbuffer.
        * @return TInt samplecount.
        */
        TInt PreferredFrameCountPerInputBuffer();

    private:

        // AAC codec interprets the input as array of TInts, not as a class like other codecs
    	RArray<TInt> iAACConfigData;

        // In AAC, the max allowed bitrate is variable, depends on sampling rate etc. Hence we need member variable instead of const
        TInt iMaxBitRate;

    };

#endif      // CCMRAMRNBCODECDATA_H

// End of File
