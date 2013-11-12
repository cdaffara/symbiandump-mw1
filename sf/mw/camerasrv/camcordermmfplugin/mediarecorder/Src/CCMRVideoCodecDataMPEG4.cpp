/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPEG4 specific data
*
*/


// INCLUDE FILES
#include "CCMRVideoCodecDataMPEG4.h"
#include "CCMRVideoSettings.h"
#include "CCMRMediaRecorder.h"
#include <mmf/devvideo/devvideorecord.h>
#include <mmf/devvideo/mpeg4visual.h>

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// CONSTANTS

// Max bitrates (bps):
// from ISO/IEC 14496-2:2003 Annex N Table N-1, Max Bitrate
const TInt KCMRMpeg4Level0Bps = 64000;
const TInt KCMRMpeg4Level0bBps = 128000;
const TInt KCMRMpeg4Level1Bps = 64000;
const TInt KCMRMpeg4Level2Bps = 128000;
const TInt KCMRMpeg4Level3Bps = 384000;
const TInt KCMRMpeg4Level4aBps = 4000000;
const TInt KCMRMpeg4Level5Bps = 8000000;
const TInt KCMRMpeg4Level6Bps = 12000000;

// Max coded picture size (in bytes):
// from ISO/IEC 14496-2:2003 Annex N Table N-1, Max vbv buffer size (units of 16384 bits)
const TUint KCMRMaxCodedPictureSizeLevel0 = 20480;
const TUint KCMRMaxCodedPictureSizeLevel0b = 40960;
const TUint KCMRMaxCodedPictureSizeLevel1 = 20480;
const TUint KCMRMaxCodedPictureSizeLevel2 = 81920;
const TUint KCMRMaxCodedPictureSizeLevel3 = 81920;
const TUint KCMRMaxCodedPictureSizeLevel4a = 163840;
const TUint KCMRMaxCodedPictureSizeLevel5 = 229376;
const TUint KCMRMaxCodedPictureSizeLevel6 = 507904;

// Max frame size (in macroblocks)
// from ISO/IEC 14496-2:2003 Annex N Table N-1, Max VCV buffer size (MB)
const TUint KCMRMaxFrameSizeMpeg4Level0 = 99;
const TUint KCMRMaxFrameSizeMpeg4Level0b = 99;
const TUint KCMRMaxFrameSizeMpeg4Level1 = 99;
const TUint KCMRMaxFrameSizeMpeg4Level2 = 396;
const TUint KCMRMaxFrameSizeMpeg4Level3 = 396;
const TUint KCMRMaxFrameSizeMpeg4Level4a = 1200;
const TUint KCMRMaxFrameSizeMpeg4Level5 = 1620;
const TUint KCMRMaxFrameSizeMpeg4Level6 = 3600;

// Max frame rate ( macroblocks/second ):
// from ISO/IEC 14496-2:2003 Annex N Table N-1, VCV decoder rate (MB/s)
const TUint KCMRMaxMBPSMpeg4Level0 = 1485;
const TUint KCMRMaxMBPSMpeg4Level0b = 1485;
const TUint KCMRMaxMBPSMpeg4Level1 = 1485;
const TUint KCMRMaxMBPSMpeg4Level2 = 5940;
const TUint KCMRMaxMBPSMpeg4Level3 = 11880;
const TUint KCMRMaxMBPSMpeg4Level4a = 36000;
const TUint KCMRMaxMBPSMpeg4Level5 = 40500;
const TUint KCMRMaxMBPSMpeg4Level6 = 108000;

// ---------------------------------------------------------
// CCMRVideoCodecDataMPEG4::MaxBitRate
// Checks the maximum allowed bitrate for the selected codec
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCMRVideoCodecDataMPEG4::MaxBitRate() const
    {
    // MPEG-4
    switch ( iVideoCodecLevel )
        {
        case 0 :
            return KCMRMpeg4Level0Bps;
        case KCMRMPEG4SPLevel0B :
            return KCMRMpeg4Level0bBps;
        case 2 :
            return KCMRMpeg4Level2Bps;
        case 3 :
            return KCMRMpeg4Level3Bps;
        case 4 :
            return KCMRMpeg4Level4aBps;
        case 5 :
            return KCMRMpeg4Level5Bps;
        case 6 :
            return KCMRMpeg4Level6Bps;
        case 1 : // 1 is the default level
        default :
            return KCMRMpeg4Level1Bps;
        }
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataMPEG4::MaxFrameRate
// Checks the maximum allowed frame rate for the selected codec
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CCMRVideoCodecDataMPEG4::MaxFrameRate(const TSize& aFrameSize) const
    {
    TReal32 macroblocksPerSecond = 0;
    switch ( iVideoCodecLevel )
        {
        case 6 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level6;
            break;
        case 5 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level5;
            break;
        case 4 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level4a;
            break;
        case 3 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level3;
            break;
        case 2 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level2;
            break;
        case 1 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level1;
            break;
        case KCMRMPEG4SPLevel0B :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level0b;
            break;
        case 0 :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level0;
            break;
        default :
            macroblocksPerSecond = KCMRMaxMBPSMpeg4Level1;
            break;
        }
    return  (macroblocksPerSecond / ( (aFrameSize.iWidth/16)*(aFrameSize.iHeight/16))); // macroblocks are 16x16 pixels
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataMPEG4::MaxFrameSize
// Checks if the frame size for the selected codec is acceptable
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCMRVideoCodecDataMPEG4::MaxFrameSize( const TSize& aSize ) const
    {
    PRINT((_L("CCMRVideoCodecDataMPEG4::MaxFrameSize() in, finding max frame size for level: %d"), iVideoCodecLevel));
    TInt maxFrameSizeInMacroblocks;

    // MPEG-4
    switch ( iVideoCodecLevel )
        {
        case 6 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level6;
            break;
        case 5 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level5;
            break;
        case 4 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level4a;
            break;
        case 3 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level3;
            break;
        case 2 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level2;
            break;
        case 1 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level1;
            break;
        case KCMRMPEG4SPLevel0B : // 0b
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level0b;
            break;
        case 0 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level0;
            break;
        default:
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeMpeg4Level1;
            break;
        }
    PRINT((_L("CCMRVideoCodecDataMPEG4::MaxFrameSize() max size in macroblocks: %d"), maxFrameSizeInMacroblocks ));

    // subQCIF is always the smallest one we support.
    // Max size depends on codec level and checked above
    if ( (aSize.iWidth < KCMRSubQCIFWidth) || (aSize.iHeight < KCMRSubQCIFHeight)
        || (((aSize.iWidth/16)*(aSize.iHeight/16)) > maxFrameSizeInMacroblocks) ) //macroblocks are 16x16 pixels
        {
        // size not ok
        return EFalse;
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecDataMPEG4::LevelForMMS
// Check if codec level indicates we are recording for MMS; then some tighter restrictions apply
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoCodecDataMPEG4::LevelForMMS() const
    {
    // MPEG-4
    if ( iVideoCodecLevel == 0 )
        {
        // MPEG-4 SP Level 0 is for MMS, although not the primary codec for MMS
        PRINT((_L("CCMRVideoCodecDataMPEG4::LevelForMMS() using MPEG-4 level 0, applying MMS restrictions")));
        return ETrue;
        }
    else
        {
        PRINT((_L("CCMRVideoCodecDataMPEG4::LevelForMMS() using some non-MMS MPEG-4 level")));
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecDataMPEG4::MaxBufferLength
// Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoCodecDataMPEG4::MaxBufferLength(const TSize& /*aFrameSize*/) const
    {
    // MPEG-4
    switch ( iVideoCodecLevel )
        {
        case 6 :
            return KCMRMaxCodedPictureSizeLevel6;
        case 5 :
            return KCMRMaxCodedPictureSizeLevel5;
        case 4 :
            return KCMRMaxCodedPictureSizeLevel4a;
        case 3 :
            return KCMRMaxCodedPictureSizeLevel3;
        case 2 :
            return KCMRMaxCodedPictureSizeLevel2;
        case 1 :
            return KCMRMaxCodedPictureSizeLevel1;
        case KCMRMPEG4SPLevel0B : // 0b
            return KCMRMaxCodedPictureSizeLevel0b;
        case 0 :
            return KCMRMaxCodedPictureSizeLevel0;
        default:
            return KCMRMaxCodedPictureSizeLevel1;
        }
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataMPEG4::SetVideoCodingOptionsL
// Set misc video coding options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataMPEG4::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions)
    {
    if ( (aOptions.iExpectedBitErrorRate == 0.0) && (aOptions.iExpectedPacketLossRate > 0.0) )
        {
        // currently only BitErrorRate has an effect, map also PacketLossRate to BitErrorRate; a very strong simplification
        if ( aOptions.iExpectedPacketLossRate > KCMRBadPacketLossRate )
            {
            // bad packet loss rate
            iExpectedBitErrorRate = KCMRBadBitErrorRate;
            }
        else if ( aOptions.iExpectedPacketLossRate > KCMRNotNoticeablePacketLossRate )
            {
            // between bad and not noticeable
            iExpectedBitErrorRate = KCMRTolerableBitErrorRate;
            }
        else
            {
            // not noticeable
            iExpectedBitErrorRate = KCMRNotNoticeableBitErrorRate;
            }
        }
    else
        {
        iExpectedBitErrorRate = aOptions.iExpectedBitErrorRate;
        }


    }

// ---------------------------------------------------------
// CCMRVideoCodecDataMPEG4::SetPreInitParamsL
// Set codec-specific video coding options to DevVideoRecord before initialize
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataMPEG4::SetPreInitParamsL(CMMFDevVideoRecord* /*aDevVideoRecord*/) const
    {
    // Set Standard specific coding options; none at the moment
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataMPEG4::SetPostInitParamsL
// Set codec-specific video coding options to DevVideoRecord after initialize
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataMPEG4::SetPostInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const
    {
    if ( iExpectedBitErrorRate > 0.0 )
        {
        // set expected bit error rate to encoder
        aDevVideoRecord->SetChannelBitErrorRate( 0, iExpectedBitErrorRate, 0);
        }
    }

