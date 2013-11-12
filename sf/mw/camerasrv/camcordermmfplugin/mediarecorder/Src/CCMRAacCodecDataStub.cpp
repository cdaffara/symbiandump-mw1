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
* Description:  AAC related stub data & functions
*
*/



// INCLUDE FILES
#include "CCMRAacCodecData.h"
#include "CCMRSupportedCodecs.h"


// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// CONSTANTS
const TInt KCMRDefAudioBitRate(32000);

const TUint KCMRSampleRate(16000);



// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCMRAACCodecData::CCMRAACCodecData
// -----------------------------------------------------------------------------
//
CCMRAACCodecData::CCMRAACCodecData(TCMRCodecType aType) : CCMRAudioCodecData(KCMRFourCCIdMPEG4AAC, KCMRDefAudioBitRate, CCMRMediaBuffer::EAudioMPEG4AAC, aType), iSampleRate(KCMRSampleRate), iChannels(1)
    {
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::~CCMRAACCodecData
// -----------------------------------------------------------------------------
//
CCMRAACCodecData::~CCMRAACCodecData()
    {
    }



// -----------------------------------------------------------------------------
// CCMRAACCodecData::Supported
// Is codec supported? No since this is a stub
// -----------------------------------------------------------------------------
//
TBool CCMRAACCodecData::Supported()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetPCMCaptureDurationMs
// Return preferred duration of PCM capture in milliseconds, relevant only for CMMFCodecs
// returns 0 since this is a stub
// -----------------------------------------------------------------------------
//
TReal CCMRAACCodecData::GetPCMCaptureDurationMs() const
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetCodecConfigParamL
// Converts bitrate to configuration parameters expected by the codec
// leaves since this is stub
// -----------------------------------------------------------------------------
//
TDes8* CCMRAACCodecData::GetCodecConfigParamL(MDataSource* /*aDataSource*/)
    {
    User::Leave( KErrNotSupported );
    return NULL;
    }


// -----------------------------------------------------------------------------
// CCMRAACCodecData::FrameLengthL
// Reads frame length from header in the given buffer
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::FrameLength( const TUint8* /*aBuf*/, TInt aDataLen ) const
    {
    return aDataLen;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetFrameDurationUs
// Get frame duration in microseconds, 0 for stub
// -----------------------------------------------------------------------------
//
TReal CCMRAACCodecData::GetFrameDurationUs() const
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::SetBitRateL
// Saves & converts bitrate to configuration parameters expected by the codec
// -----------------------------------------------------------------------------
//
void CCMRAACCodecData::SetBitRateL(TInt /*aBitRate*/)
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetBitRateL
// Get the used bitrate; leaves since this is stub
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::GetBitRateL() const
    {
    User::Leave( KErrNotSupported );
    return 0;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::SetSampleRateL
// Set sample rate for the codec, leaves since this is stub
// -----------------------------------------------------------------------------
//
void CCMRAACCodecData::SetSampleRateL(TUint /*aSampleRate*/)
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetSampleRate
// Get the used sample rate
// -----------------------------------------------------------------------------
//
TUint CCMRAACCodecData::GetSampleRate() const
    {
    return 0;
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
// Return max size of coded buffer in bytes; leaves since this is stub
// -----------------------------------------------------------------------------
//
TInt CCMRAACCodecData::MaxFrameLengthL() const
    {
    User::Leave( KErrNotSupported );
    return 0;
    }

// -----------------------------------------------------------------------------
// CCMRAACCodecData::GetDecoderConfigInfoLC
// Get the decoder configuration info for the codec to be saved into metadata etc
// leaves since this is stub
// -----------------------------------------------------------------------------
//
HBufC8* CCMRAACCodecData::GetDecoderConfigInfoLC()
    {
    User::Leave( KErrNotSupported );
    return NULL;
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
    return 1024;
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

