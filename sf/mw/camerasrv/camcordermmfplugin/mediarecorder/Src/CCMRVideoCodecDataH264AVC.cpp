/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  H.264 AVC specific data
*
*/



// INCLUDE FILES
#include "CCMRVideoCodecDataH264AVC.h"
#include "CCMRVideoSettings.h"
#include "CCMRMediaRecorder.h"

#include <mmf/devvideo/devvideorecord.h>
#include <mmf/devvideo/avc.h>

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// Max bitrates (bps) based on codec levels
// from ISO IEC 14496-10:2003 Annex A, Table A-1, Max video bit rate MaxBR.
const TInt KCMRH264AVCMaxBpsLevel10 = 64000;
const TInt KCMRH264AVCMaxBpsLevel10b = 128000;
const TInt KCMRH264AVCMaxBpsLevel11 = 192000;
const TInt KCMRH264AVCMaxBpsLevel12 = 384000;
const TInt KCMRH264AVCMaxBpsLevel13 = 768000;
const TInt KCMRH264AVCMaxBpsLevel20 = 2000000;
const TInt KCMRH264AVCMaxBpsLevel21 = 4000000;
const TInt KCMRH264AVCMaxBpsLevel22 = 4000000;
const TInt KCMRH264AVCMaxBpsLevel30 = 10000000;
const TInt KCMRH264AVCMaxBpsLevel31 = 14000000;
const TInt KCMRH264AVCMaxBpsLevel32 = 20000000;
const TInt KCMRH264AVCMaxBpsLevel40 = 20000000;

// Max coded picture size (in bytes; per H.264 AVC specification).
// from ISO/IEC 14496-10:2003 Annex A, Table A-1, Max CPB size MaxCPB (in 1000 bits units).
const TUint KCMRMaxCodedPictureSizeH264AVCLevel10 = 21875;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel10b = 43750;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel11 = 62500;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel12 = 125000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel13 = 250000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel20 = 250000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel21 = 500000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel22 = 500000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel30 = 1250000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel31 = 1750000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel32 = 2500000;
const TUint KCMRMaxCodedPictureSizeH264AVCLevel40 = 3125000;

// max macroblocks/second:
// from ISO/IEC 14496-10:2003 Annex A, Table A-1, Max macroblock processing rate MaxMBPS.
const TUint KCMRMaxMBPSH264AVCLevel10 = 1485;
const TUint KCMRMaxMBPSH264AVCLevel10b = 1485;
const TUint KCMRMaxMBPSH264AVCLevel11 = 3000;
const TUint KCMRMaxMBPSH264AVCLevel12 = 6000;
const TUint KCMRMaxMBPSH264AVCLevel13 = 11880;
const TUint KCMRMaxMBPSH264AVCLevel20 = 11880;
const TUint KCMRMaxMBPSH264AVCLevel21 = 19800;
const TUint KCMRMaxMBPSH264AVCLevel22 = 20250;
const TUint KCMRMaxMBPSH264AVCLevel30 = 40500;
const TUint KCMRMaxMBPSH264AVCLevel31 = 108000;
const TUint KCMRMaxMBPSH264AVCLevel32 = 216000;
const TUint KCMRMaxMBPSH264AVCLevel40 = 245760;

// max frame size (in macroblocks)
// from ISO/IEC 14496-10:2003 Table A-1, Max frame size MaxFS
const TUint KCMRMaxFrameSizeH264AVCLevel10 = 99;
const TUint KCMRMaxFrameSizeH264AVCLevel10b = 99;
const TUint KCMRMaxFrameSizeH264AVCLevel11 = 396;
const TUint KCMRMaxFrameSizeH264AVCLevel12 = 396;
const TUint KCMRMaxFrameSizeH264AVCLevel13 = 396;
const TUint KCMRMaxFrameSizeH264AVCLevel20 = 396;
const TUint KCMRMaxFrameSizeH264AVCLevel21 = 792;
const TUint KCMRMaxFrameSizeH264AVCLevel22 = 1620;
const TUint KCMRMaxFrameSizeH264AVCLevel30 = 1620;
const TUint KCMRMaxFrameSizeH264AVCLevel31 = 3600;
const TUint KCMRMaxFrameSizeH264AVCLevel32 = 5120;
const TUint KCMRMaxFrameSizeH264AVCLevel40 = 8192;

// ---------------------------------------------------------
// CCMRVideoCodecDataH264AVC::MaxBitRate
// Checks the maximum allowed bitrate for the selected codec
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCMRVideoCodecDataH264AVC::MaxBitRate() const
    {
    // H.264 AVC maximum bitrates per level (baseline profile).
    switch ( iVideoCodecLevel )
        {
        case KCMRH264AVCCodecLevel10 :
            return KCMRH264AVCMaxBpsLevel10;
        case KCMRH264AVCCodecLevel10b :
            return KCMRH264AVCMaxBpsLevel10b;
        case KCMRH264AVCCodecLevel11 :
            return KCMRH264AVCMaxBpsLevel11;
        case KCMRH264AVCCodecLevel12 :
            return KCMRH264AVCMaxBpsLevel12;
        case KCMRH264AVCCodecLevel13 :
            return KCMRH264AVCMaxBpsLevel13;
        case KCMRH264AVCCodecLevel20 :
            return KCMRH264AVCMaxBpsLevel20;
        case KCMRH264AVCCodecLevel21 :
            return KCMRH264AVCMaxBpsLevel21;
        case KCMRH264AVCCodecLevel22 :
            return KCMRH264AVCMaxBpsLevel22;
        case KCMRH264AVCCodecLevel30 :
            return KCMRH264AVCMaxBpsLevel30;
        case KCMRH264AVCCodecLevel31 :
            return KCMRH264AVCMaxBpsLevel31;
        case KCMRH264AVCCodecLevel32 :
            return KCMRH264AVCMaxBpsLevel32;
        case KCMRH264AVCCodecLevel40 :
            return KCMRH264AVCMaxBpsLevel40;
        default :
            return KCMRH264AVCMaxBpsLevel10;
        }
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataH264AVC::MaxFrameRate
// Checks the maximum allowed frame rate for the selected codec
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CCMRVideoCodecDataH264AVC::MaxFrameRate(const TSize& aFrameSize) const
    {
    TReal32 macroblocksPerSecond = 0;
    switch ( iVideoCodecLevel )
        {
        case KCMRH264AVCCodecLevel10 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel10;
            break;
        case KCMRH264AVCCodecLevel10b :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel10b;
            break;
        case KCMRH264AVCCodecLevel11 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel11;
            break;
        case KCMRH264AVCCodecLevel12 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel12;
            break;
        case KCMRH264AVCCodecLevel13 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel13;
            break;
        case KCMRH264AVCCodecLevel20 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel20;
            break;
        case KCMRH264AVCCodecLevel21 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel21;
            break;
        case KCMRH264AVCCodecLevel22 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel22;
            break;
        case KCMRH264AVCCodecLevel30 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel30;
            break;
        case KCMRH264AVCCodecLevel31 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel31;
            break;
        case KCMRH264AVCCodecLevel32 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel32;
            break;
        case KCMRH264AVCCodecLevel40 :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel40;
            break;
        default :
            macroblocksPerSecond = KCMRMaxMBPSH264AVCLevel10;
            break;
        }
    return  (macroblocksPerSecond / ( (aFrameSize.iWidth/16)*(aFrameSize.iHeight/16))); // macroblocks are 16x16 pixels
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataH264AVC::MaxFrameSize
// Checks if the frame size for the selected codec is acceptable
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCMRVideoCodecDataH264AVC::MaxFrameSize( const TSize& aSize ) const
    {
    PRINT((_L("CCMRVideoCodecDataH264AVC::MaxFrameSize() in, finding max frame size for level: %d"), iVideoCodecLevel));
    TInt maxFrameSizeInMacroblocks;

    // H.264 AVC
    switch ( iVideoCodecLevel )
        {
        case KCMRH264AVCCodecLevel10 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel10;
            break;
        case KCMRH264AVCCodecLevel10b :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel10b;
            break;
        case KCMRH264AVCCodecLevel11 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel11;
            break;
        case KCMRH264AVCCodecLevel12 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel12;
            break;
        case KCMRH264AVCCodecLevel13 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel13;
            break;
        case KCMRH264AVCCodecLevel20 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel20;
            break;
        case KCMRH264AVCCodecLevel21 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel21;
            break;
        case KCMRH264AVCCodecLevel22 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel22;
            break;
        case KCMRH264AVCCodecLevel30 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel30;
            break;
        case KCMRH264AVCCodecLevel31 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel31;
            break;
        case KCMRH264AVCCodecLevel32 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel32;
            break;
        case KCMRH264AVCCodecLevel40 :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel40;
            break;
        default :
            maxFrameSizeInMacroblocks = KCMRMaxFrameSizeH264AVCLevel10;
            break;
        }
	PRINT((_L("CCMRVideoCodecDataH264AVC::MaxFrameSize() max size in macroblocks: %d"), maxFrameSizeInMacroblocks ));

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
// CCMRVideoCodecDataH264AVC::LevelForMMS
// Check if codec level indicates we are recording for MMS; then some tighter restrictions apply
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoCodecDataH264AVC::LevelForMMS() const
    {
    // H.264 AVC no MMS support.
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecDataH264AVC::MaxBufferLength
// Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoCodecDataH264AVC::MaxBufferLength(const TSize& /*aFrameSize*/) const
    {
    switch ( iVideoCodecLevel )
        {
        case KCMRH264AVCCodecLevel10 :
            return KCMRMaxCodedPictureSizeH264AVCLevel10;
        case KCMRH264AVCCodecLevel10b :
            return KCMRMaxCodedPictureSizeH264AVCLevel10b;
        case KCMRH264AVCCodecLevel11 :
            return KCMRMaxCodedPictureSizeH264AVCLevel11;
        case KCMRH264AVCCodecLevel12 :
            return KCMRMaxCodedPictureSizeH264AVCLevel12;
        case KCMRH264AVCCodecLevel13 :
            return KCMRMaxCodedPictureSizeH264AVCLevel13;
        case KCMRH264AVCCodecLevel20 :
            return KCMRMaxCodedPictureSizeH264AVCLevel20;
        case KCMRH264AVCCodecLevel21 :
            return KCMRMaxCodedPictureSizeH264AVCLevel21;
        case KCMRH264AVCCodecLevel22 :
            return KCMRMaxCodedPictureSizeH264AVCLevel22;
        case KCMRH264AVCCodecLevel30 :
            return KCMRMaxCodedPictureSizeH264AVCLevel30;
        case KCMRH264AVCCodecLevel31 :
            return KCMRMaxCodedPictureSizeH264AVCLevel31;
        case KCMRH264AVCCodecLevel32 :
            return KCMRMaxCodedPictureSizeH264AVCLevel32;
        case KCMRH264AVCCodecLevel40 :
            return KCMRMaxCodedPictureSizeH264AVCLevel40;
        default :
            return KCMRH264AVCMaxBpsLevel10;
        }
    }


// ---------------------------------------------------------
// CCMRVideoCodecDataH264AVC::SetVideoCodingOptionsL
// Set misc video coding options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataH264AVC::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions)
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
// CCMRVideoCodecDataH264AVC::SetPreInitParamsL
// Set codec-specific video coding options to DevVideoRecord before initialize
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataH264AVC::SetPreInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const
    {
    // Set standard specific coding options
    TAvcVideoMode h264avcOptions;
    h264avcOptions.iAllowedPictureTypes = EAvcPictureTypeI | EAvcPictureTypeP;
    h264avcOptions.iFlexibleMacroblockOrder = EFalse;
    h264avcOptions.iRedundantPictures = EFalse;
    h264avcOptions.iDataPartitioning = EFalse;
    h264avcOptions.iFrameMBsOnly = ETrue;
    h264avcOptions.iMBAFFCoding = EFalse;
    h264avcOptions.iEntropyCodingCABAC = EFalse;
    h264avcOptions.iWeightedPPrediction = EFalse;
    h264avcOptions.iWeightedBipredicitonMode = 0;
    h264avcOptions.iDirect8x8Inference = EFalse;
    TPckgC<TAvcVideoMode> h264avcpckg(h264avcOptions);
    aDevVideoRecord->SetCodingStandardSpecificOptionsL( h264avcpckg );
    }



// ---------------------------------------------------------
// CCMRVideoCodecDataH264AVC::SetPostInitParamsL
// Set codec-specific video coding options to DevVideoRecord after initialize
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataH264AVC::SetPostInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const
    {
    if ( iExpectedBitErrorRate > 0.0 )
        {
        // set expected bit error rate to encoder
        aDevVideoRecord->SetChannelBitErrorRate( 0, iExpectedBitErrorRate, 0);
        }
    }

