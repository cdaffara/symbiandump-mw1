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
* Description:  AMR-NB related data & functions
*                Note!!! The custom interface class, Uid and type definitions must be
*                synchronized with the actual HW device custom interface
*                class before building the component, there is no
*                run-time checking!!
*
*/



// INCLUDE FILES
#include "CCMRAMRNBCodecData.h"
#include "CCMRSupportedCodecs.h"

#include <mmf/server/mmfaudioinput.h>
#include <SpeechEncoderConfig.h>

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// CONSTANTS
const TUint KCMRRawAmrFrameLength[16] = {13,14,16,18,20,21,27,32,6,0,0,0,0,0,0,1};

const TInt KCMRDefAudioBitRate(12200);
const TInt KCMRMinAudioBitRate(400);      // voice activity detection creates at least 1 byte per frame => 50 bytes/s = 400 bits/s
const TInt KCMRMaxAudioBitRate(12200);

const TInt KCMRAMRBufferSize(32);

// Buffer duration for splitted PCM16 buffers in ms
const TUint KCMRAMRMaxPCMBufferTime = 60;  // 60 ms; kept short to keep encoding times also short, which should minimize the effects to video framerate


//CLASS DECLARATIONS

/**
*    TCMRAMREncParams
*    Encoding parameters structure for SW AMR codec
*/
class TCMRAMREncParams
    {
public:
    // encoding mode (for AMR-NB: 0=MR475,1=MR515,...,7=MR122, default 7)
    TInt iMode;
    // DTX flag (TRUE or default FALSE)
    TInt iDTX;
    };


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::CCMRAMRNBCodecData
// -----------------------------------------------------------------------------
//
CCMRAMRNBCodecData::CCMRAMRNBCodecData(TCMRCodecType aType) : CCMRAudioCodecData(KCMRFourCCIdAMRNB, KCMRDefAudioBitRate, CCMRMediaBuffer::EAudioAMRNB, aType)
    {
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::~CCMRAMRNBCodecData
// -----------------------------------------------------------------------------
//
CCMRAMRNBCodecData::~CCMRAMRNBCodecData()
    {
    PRINT((_L("CCMRAMRNBCodecData::~CCMRAMRNBCodecData() in")));
    delete iConfigData;
    PRINT((_L("CCMRAMRNBCodecData::~CCMRAMRNBCodecData() out")));
    }


// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::Supported
// Return ETrue if the codec is supported in this configuration, EFalse otherwise
// -----------------------------------------------------------------------------
//
TBool CCMRAMRNBCodecData::Supported()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::GetPCMCaptureDurationMs
// Return preferred duration of PCM capture in milliseconds, relevant only for CMMFCodecs
// -----------------------------------------------------------------------------
//
TReal CCMRAMRNBCodecData::GetPCMCaptureDurationMs() const
    {
    return KCMRAMRMaxPCMBufferTime;
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::GetCodecConfigParamL
// Converts bitrate to configuration parameters expected by the AMR codec
// -----------------------------------------------------------------------------
//
TDes8* CCMRAMRNBCodecData::GetCodecConfigParamL(MDataSource* /*aDataSource*/)
    {
    PRINT((_L("CCMRAMRNBCodecData::GetCodecConfigParamL()")));
    if ( iConfigData == NULL )
        {
        // no exact bitrate given, use the default
        SetBitRateL(iBitRate);
        }
    return reinterpret_cast<TDes8*>(iConfigData);
    }


// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::FrameLengthL
// Reads frame length from header in the given buffer
// -----------------------------------------------------------------------------
//
TInt CCMRAMRNBCodecData::FrameLength( const TUint8* aBuf, TInt /*aDataLen*/ ) const
    {
    return KCMRRawAmrFrameLength[(TInt)(((*aBuf) & 0x78) >> 3)];
    }


// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::SetBitRateL
// Saves & converts bitrate to configuration parameters expected by the AMR-NB codec
// -----------------------------------------------------------------------------
//
void CCMRAMRNBCodecData::SetBitRateL(TInt aBitRate)
    {
    PRINT((_L("CCMRAMRNBCodecData::SetBitRateL(); aBitRate %d"),aBitRate));

    if ( (aBitRate < KCMRMinAudioBitRate) || (aBitRate > KCMRMaxAudioBitRate) )
        {
        PRINT((_L("CCMRAMRNBCodecData::SetBitRateL(); illegal bitrate")));
        User::Leave( KErrArgument );
        }

    iBitRate = aBitRate;

    TCMRAMREncParams* configData;
    if ( iConfigData == NULL )
        {
        configData = new (ELeave) TCMRAMREncParams;
        iConfigData = reinterpret_cast<TDes8*>(configData);
        }
    else
        {
        configData = reinterpret_cast<TCMRAMREncParams*>(iConfigData);
        }

    // the bitrates in the switch & if below are not magic numbers but AMR bitrates in bits per seconds and mode indices from TAmrEncParams

    switch ( aBitRate )
        {
        case 4750:
            configData->iMode = 0;
            configData->iDTX = EFalse;
            break;
        case 5150:
            configData->iMode = 1;
            configData->iDTX = EFalse;
            break;
        case 5900:
            configData->iMode = 2;
            configData->iDTX = EFalse;
            break;
        case 6700:
            configData->iMode = 3;
            configData->iDTX = EFalse;
            break;
        case 7400:
            configData->iMode = 4;
            configData->iDTX = EFalse;
            break;
        case 7950:
            configData->iMode = 5;
            configData->iDTX = EFalse;
            break;
        case 10200:
            configData->iMode = 6;
            configData->iDTX = EFalse;
            break;
        case 12200:
            configData->iMode = 7;
            configData->iDTX = EFalse;
            break;
        default :
            // Interprets now the bitrate proprietarily: bitrates that are not exactly AMR bitrates
            // mean that voice activity detection is used and the AMR bitrates is the given bitrate rounded upwards to the next AMR bitrate
            if ( aBitRate < 4750 )
                {
                configData->iMode = 0;
                configData->iDTX = ETrue;
                }
            else if ( aBitRate < 5150 )
                {
                configData->iMode = 1;
                configData->iDTX = ETrue;
                }
            else if ( aBitRate < 5900 )
                {
                configData->iMode = 2;
                configData->iDTX = ETrue;
                }
            else if ( aBitRate < 6700 )
                {
                configData->iMode = 3;
                configData->iDTX = ETrue;
                }
            else if ( aBitRate < 7400 )
                {
                configData->iMode = 4;
                configData->iDTX = ETrue;
                }
            else if ( aBitRate < 7950 )
                {
                configData->iMode = 5;
                configData->iDTX = ETrue;
                }
            else if ( aBitRate < 10200 )
                {
                configData->iMode = 6;
                configData->iDTX = ETrue;
                }
            else // must be: ( aBitRate < 12200 ) since checked earlier
                {
                configData->iMode = 7;
                configData->iDTX = ETrue;
                }
        }

    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::GetBitRateL
// Get the used bitrate
// -----------------------------------------------------------------------------
//
TInt CCMRAMRNBCodecData::GetBitRateL() const
    {
    return iBitRate;
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::MaxFrameLengthL
// Return max size of coded buffer in bytes
// -----------------------------------------------------------------------------
//
TInt CCMRAMRNBCodecData::MaxFrameLengthL() const
    {
    return KCMRAMRBufferSize;
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::CCMRAudioHWConfigL
// Configures sample DevSound AMR HW device
// Returns: KErrNone or one of the system wide error codes
// -----------------------------------------------------------------------------
//
TInt CCMRAMRNBCodecData::AudioHWConfigL(MDataSource* aDataSource, const TDesC8& aParams )
    {
    PRINT((_L("CCMRAMRNBCodecData::AudioHWConfig() in")));
    TInt err = KErrNone;

    CMMFAudioInput* audioInput = static_cast<CMMFAudioInput*>(aDataSource);
    CSpeechEncoderConfig* custInt = CSpeechEncoderConfig::NewL( audioInput->SoundDevice() );
    CleanupStack::PushL(custInt);

    if (custInt)
        {
        // set it to the HW device
        TInt err = custInt->SetBitrate( iBitRate );
        if ( err != KErrNone )
            {
            PRINT((_L("CCMRAMRNBCodecData::CCMRAudioHWConfig() setting new bitrate to HW device failed %d"), err));
            return err;
            }

        const TCMRAMREncParams* amrParams = reinterpret_cast<const TCMRAMREncParams*>(&aParams);
        // Voice activity detection, error not that relevant
        err = custInt->SetVadMode( amrParams->iDTX );
        }
    else
        {
        // no custom interface for the HW device with the given Uid / no HW device; use default settings
        }
    CleanupStack::PopAndDestroy(custInt);
    PRINT((_L("CCMRAMRNBCodecData::AudioHWConfig() out, err %d"), err));
    return err;
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::PreferredSampleCountPerInputBuffer
// Preferred samplecount per inputbuffer.
// Returns: samplecount per inputbuffer
// -----------------------------------------------------------------------------
//
TInt CCMRAMRNBCodecData::PreferredSampleCountPerInputBuffer()
    {
    return 8*KCMRAMRMaxPCMBufferTime;
    }

// -----------------------------------------------------------------------------
// CCMRAMRNBCodecData::PreferredFrameCountPerInputBuffer
// Get the preferred framecount per PCM inputbuffer.
// Returns: samplecount per inputbuffer
// -----------------------------------------------------------------------------
//
TInt CCMRAMRNBCodecData::PreferredFrameCountPerInputBuffer()
    {
    return 3;
    }

