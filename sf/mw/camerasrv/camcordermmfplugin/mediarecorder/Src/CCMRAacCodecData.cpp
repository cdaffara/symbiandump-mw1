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
* Description:  AAC related data & functions
*
*/



// INCLUDE FILES
#include "CCMRAacCodecData.h"

#include "CCMRSupportedCodecs.h"

#include <mmf/server/mmfaudioinput.h>

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// CONSTANTS
const TInt KCMRDefAudioBitRate(32000);
// const TInt KCMRMaxAudioBitRate(96000); // recommended is 2x max samplerate.
const TInt KCMRMinAudioBitRate(16000);

const TInt KCMRAudioType(2);    //AAC-LC
const TUint KCMRSampleRate(16000);
const TInt KCMRAACBufferSize(768);  // this is the max AAC framelength; 6*128 bytes


// Length of decoder configuration info in bytes. Is filled in CCMRAACCodecData::GetDecoderConfigInfoLC() and this should match with the method
const TInt KCMRAACDecoderConfigInfoLength = 2;

// list of supported sample rates
const TInt KCMRNumSupportedSampleRates = 5;
const TUint KCMRSupportedSampleRates[5] = {8000,16000,24000,32000,48000};





// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RateIndex: converts sample rate to rate index used in decoder config info
// -----------------------------------------------------------------------------
//
static TUint8 RateIndex(TInt aRate)
    {
    switch ( aRate )
        {
        case 96000:
            return 0x0;
        case 88200:
            return 0x1;
        case 64000:
            return 0x2;
        case 48000:
            return 0x3;
        case 44100:
            return 0x4;
        case 32000:
            return 0x5;
        case 24000:
            return 0x6;
        case 22050:
            return 0x7;
        case 16000:
            return 0x8;
        case 12000:
            return 0x9;
        case 11025:
            return 0xa;
        case 8000:
            return 0xb;
        case 7350:
            return 0xc;
        default:
            return 0xf;
        }
    }



// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCMRAACCodecData::CCMRAACCodecData
// -----------------------------------------------------------------------------
//
CCMRAACCodecData::CCMRAACCodecData(TCMRCodecType aType) : CCMRAudioCodecData(KCMRFourCCIdMPEG4AAC, KCMRDefAudioBitRate, CCMRMediaBuffer::EAudioMPEG4AAC, aType)
    {
    // this formula comes from AAC, the constants too
    iChannels = 1;
    iSampleRate = KCMRSampleRate;
    iMaxBitRate = 6144 * iChannels * iSampleRate / 1024;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::~CCMRAACCodecData
// -----------------------------------------------------------------------------
//
CCMRAACCodecData::~CCMRAACCodecData()
    {
    PRINT((_L("CCMRAACCodecData::~CCMRAACCodecData() in")));
    delete iConfigData;
    iAACConfigData.Close();
    PRINT((_L("CCMRAACCodecData::~CCMRAACCodecData() out")));
    }



// -----------------------------------------------------------------------------
// CCMRAACCodecData::Supported
// Return ETrue if the codec is supported in this configuration, EFalse otherwise
// -----------------------------------------------------------------------------
//
TBool CCMRAACCodecData::Supported()
    {
    PRINT((_L("CCMRAACCodecData::Supported()")));
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetPCMCaptureDurationMs
// Return preferred duration of PCM capture in milliseconds, relevant only for CMMFCodecs
// -----------------------------------------------------------------------------
//
TReal CCMRAACCodecData::GetPCMCaptureDurationMs() const
    {
    return GetFrameDurationUs()/1000;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetCodecConfigParamL
// Converts bitrate to configuration parameters expected by the codec
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TDes8* CCMRAACCodecData::GetCodecConfigParamL(MDataSource* /*aDataSource*/)
    {
    PRINT((_L("CCMRAACCodecData::GetCodecConfigParamL()")));

    // AAC codec interprets the input as array of TInts, not as a class
    iAACConfigData.Reset();
	iAACConfigData.AppendL (iBitRate);	//iBitRate
	iAACConfigData.AppendL (iSampleRate);	//iSamplingRate
	iAACConfigData.AppendL (0);	//iToolFlags
	iAACConfigData.AppendL (iChannels);	//iNumChan
	iAACConfigData.AppendL (0);	//iuseFormat 0=RAW; 1=ADTS; 2=ADIF
	iAACConfigData.AppendL (0);	// 0 = 1 Frame only; 1 = Full Buffer

    PRINT((_L("CCMRAACCodecData::GetCodecConfigParamL() out")));
    return reinterpret_cast<TDes8*>(&iAACConfigData);
    }


// -----------------------------------------------------------------------------
// CCMRAACCodecData::FrameLengthL
// Reads frame length from header in the given buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::FrameLength( const TUint8* /*aBuf*/, TInt aDataLen ) const
    {
    return aDataLen;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetFrameDurationUs
// Return frame duration in microseconds
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal CCMRAACCodecData::GetFrameDurationUs() const
    {
    return (TReal(1000)*1024/iSampleRate)*1000;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::SetBitRateL
// Saves & converts bitrate to configuration parameters expected by the codec
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAACCodecData::SetBitRateL(TInt aBitRate)
    {
    PRINT((_L("CCMRAACCodecData::SetBitRateL(); aBitRate %d"),aBitRate));

    if ( (aBitRate > iMaxBitRate) || (aBitRate < KCMRMinAudioBitRate))
        {
        PRINT((_L("CCMRAACCodecData::SetBitRateL(); illegal bitrate")));
        User::Leave( KErrArgument );
        }

    iBitRate = aBitRate;

    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetBitRateL
// Get the used bitrate
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::GetBitRateL() const
    {
    return iBitRate;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::SetSampleRateL
// Set sample rate for the codec, leaves if not supported
// -----------------------------------------------------------------------------
//
void CCMRAACCodecData::SetSampleRateL(TUint aSampleRate)
    {
    TInt i;
    TBool iSet = EFalse;
    for ( i = 0; i < KCMRNumSupportedSampleRates; i++ )
        {
        if ( aSampleRate == KCMRSupportedSampleRates[i] )
            {
            iSampleRate = aSampleRate;
            iSet = ETrue;
            }
        }
    if ( !iSet )
        {
        // given samplerate is not supported
        PRINT((_L("CCMRAACCodecData::SetSampleRateL(); unsupported samplerate")));
        User::Leave( KErrNotSupported );
        }

    // this formula comes from AAC, the constants too
    iMaxBitRate = 6144 * iChannels * iSampleRate / 1024;

    if ( iBitRate > iMaxBitRate )
        {
        PRINT((_L("CCMRAACCodecData::SetSampleRateL(); illegal bitrate & samplerate combination")));
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetSampleRate
// Get the used sample rate
// -----------------------------------------------------------------------------
//
TUint CCMRAACCodecData::GetSampleRate() const
    {
    return iSampleRate;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::SetChannelModeL
// Get the used channel mode
// -----------------------------------------------------------------------------
//
void CCMRAACCodecData::SetChannelModeL(TUint aChannelMode)
    {
    iChannels = aChannelMode;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::MaxFrameLengthL
// Return max size of coded buffer in bytes
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::MaxFrameLengthL() const
    {
    return KCMRAACBufferSize*iChannels;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetDecoderConfigInfoLC
// Get the decoder configuration info for the codec to be saved into metadata etc
// -----------------------------------------------------------------------------
//
HBufC8* CCMRAACCodecData::GetDecoderConfigInfoLC()
    {
    HBufC8* configData = 0;
    configData = (HBufC8*) HBufC8::NewLC(KCMRAACDecoderConfigInfoLength);

    TUint8 data[2];

    data[0] = KCMRAudioType<<3;
    TUint8 rate = RateIndex(iSampleRate);
    data[0] |= rate>>1;
    data[1] = TUint8(rate<<7);
    data[1] |= TUint8(iChannels<<3);

    configData->Des().Append( data[0] );
    configData->Des().Append( data[1] );

    return configData;

    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::CCMRAudioHWConfigL
// Configures sample DevSound AAC HW device
// Returns: KErrNone or one of the system wide error codes
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::AudioHWConfigL(MDataSource* /*aDataSource*/, const TDesC8& /*aParams*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::PreferredSampleCountPerInputBuffer
// Preferred samplecount per inputbuffer.
// Returns: samplecount per inputbuffer
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::PreferredSampleCountPerInputBuffer()
    {
    return 1024; //from AAC samplecount per frame.
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::PreferredFrameCountPerInputBuffer
// Get the preferred framecount per PCM inputbuffer.
// Returns: samplecount per inputbuffer
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::PreferredFrameCountPerInputBuffer()
    {
    return 1;
    }

