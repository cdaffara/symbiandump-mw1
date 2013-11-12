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
* Description:  Header file for audio codec data class which is specific to each codec type
*
*/


#ifndef CCMRAUDIOCODECDATA_H
#define CCMRAUDIOCODECDATA_H

// INCLUDES
#include <CCMRMediaSink.h>

#include <e32base.h>
#include <mmf/common/mmffourcc.h>


/**
* Audio codec type (SW/HW)
*/
enum TCMRCodecType
    {
        ECodecTypeSW,
        ECodecTypeHW,
        ECodecTypeNA
    };


// FORWARD DECLARATIONS
class MDataSource;


// CLASS DECLARATION

/**
*  Base class for audio codec data
*
*  @lib
*  @since 2.6
*/
class CCMRAudioCodecData : public CBase
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        inline CCMRAudioCodecData(TFourCC aFourCC, TInt aBitRate, CCMRMediaBuffer::TBufferType aBufferType, TCMRCodecType aType)
            : iFourCC(aFourCC), iBitRate(aBitRate), iSampleRate( 8000 ), iChannels( 1 ),
              iBufferType(aBufferType), iConfigData(NULL), iCodecSWHWType(aType) {};

        /**
        * Destructor.
        */
        virtual ~CCMRAudioCodecData();

    public: // New functions

        /**
        * Check if the codec is supported in this configuration
        * @return TBool ETrue if supported
        */
        virtual TBool Supported() = 0;

        /**
        * Set channel mode for the codec, leaves by default.
        */
        virtual inline void SetChannelModeL(TUint /*aChannelMode*/) { User::Leave(KErrNotSupported); };

        /**
        * Return the number of channels supported
        * @return TUint number of channels (1/2)
        */
        virtual inline TUint GetNumChannels() const { return iChannels; };

        /**
        * Set sample rate for the codec, leaves by default, but can be overridden, then leaves if not supported
        */
        virtual inline void SetSampleRateL(TUint /*aSampleRate*/) { User::Leave(KErrNotSupported); };
        /**
        * Return the supported sample rate for the codec
        * @return TUint sample rate (Hz)
        */
        virtual inline TUint GetSampleRate() const { return iSampleRate; };
        /**
        * Get codec's type/location: SW for CMMFCodecs and HW for HW Devices
        * @return ECodecTypeSW/ECodecTypeHW/ECodecTypeNA
        */
        inline TCMRCodecType GetCodecSWHWTypeL() { return iCodecSWHWType; };
        /**
        * Return frame duration in microseconds
        * @return TReal duration in microseconds
        */
        virtual inline TReal GetFrameDurationUs() const { return 20000; };

        /**
        * Return preferred duration of PCM capture in microseconds, relevant only for CMMFCodecs
        * @return TReal duration in milliseconds
        */
        virtual TReal GetPCMCaptureDurationMs() const = 0;

        /**
        * Set bitrate for the codec
        * @param aBitRate in bps
        * @return void
        */
        virtual void SetBitRateL(TInt aBitRate) = 0;
        /**
        * Get the current bitrate
        * @return TInt bitrate in bps
        */
        virtual TInt GetBitRateL() const = 0;

        /**
        * Get the configuration parameters for the codec
        * @param aDataSource    MMF audio input
        * @return TDes8* configuration parameter structure
        */
        virtual TDes8* GetCodecConfigParamL(MDataSource* aDataSource) = 0;

        /**
        * Get the FourCC of the codec
        * @return TFourCC
        */
        virtual TFourCC GetCodecFourCCL() const;

        /**
        * Reads the framelength from the header of the given buffer
        * @param const TUint8* aBuf
        * @return TInt frame length in bytes
        */
        virtual TInt FrameLength( const TUint8* aBuf, TInt aDataLen  ) const = 0;

        /**
        * Returns max coded frame length for the codec. Client can then e.g. allocate buffers based on it
        * @return TInt Max frame length in bytes
        */
        virtual TInt MaxFrameLengthL() const = 0;

        /**
        * Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
        * @return TInt Max frame length in bytes
        */
        virtual TInt MaxBufferLength() const = 0;

        /**
        * Returns the CCMRMediaSink/CCMRMediaBuffer compatible type of the codec buffer
        * @return CCMRMediaBuffer::TBufferType buffer type
        */
        virtual CCMRMediaBuffer::TBufferType BufferType() const;

        /**
        * Configures sample DevSound HW device
        * @since 2.1
        * @return TInt error code
        */
        virtual TInt AudioHWConfigL(MDataSource* aDataSource, const TDesC8& aParams ) = 0;

        /**
        * Get the decoder configuration info
        * @return HBufC8* info structure
        */
        virtual HBufC8* GetDecoderConfigInfoLC();

        /**
        * Get the Uid of the SW codec we support
        * @return TUid Uid
        */
        virtual TUid SWCodecUid() = 0;

        /**
        * Get the preferred samplecount per PCM inputbuffer.
        * @return TInt samplecount.
        */
        virtual TInt PreferredSampleCountPerInputBuffer() = 0;

        /**
        * Get the preferred framecount per PCM inputbuffer.
        * @return TInt samplecount.
        */
        virtual TInt PreferredFrameCountPerInputBuffer() = 0;

    protected: // member data
        // FourCC of the codec
        TFourCC iFourCC;

        // The selected bitrate of the codec
        TInt iBitRate;

        // The selected samplerate of the codec
        TInt iSampleRate;

        // The selected channel mode of the codec
        TInt iChannels;

        // CCMRMediaSink/CCMRMediaBuffer compatible type of the codec buffer
        CCMRMediaBuffer::TBufferType iBufferType;

        // Configuration data structure
        TDes8* iConfigData;

    private:

        // type/location of the codec: HW or SW
        TCMRCodecType iCodecSWHWType;

    };



#endif      // CCMRAUDIOCODECDATA_H

// End of File
