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
* Description:  H.263 specific data
*
*/



// INCLUDE FILES
#include "CCMRVideoCodecDataH263.h"
#include "CCMRVideoSettings.h"
#include "CCMRMediaRecorder.h"

#include <mmf/devvideo/devvideorecord.h>
#include <mmf/devvideo/h263.h>

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// CONSTANTS based on ITU-T H.263 specification (04/2005)
// Annex X, profiles and levels definition
// Table X.2/H.263 - Levels of operation:

// Max framerates (fps) based on codec levels
const TReal32 KCMRH263Level10Fps = 15.0; // or actually 14.985
const TReal32 KCMRH263Level20Fps = 15.0; // or actually 14.985
const TReal32 KCMRH263Level20FpsQCIF = 30.0; // or actually 29.97
const TReal32 KCMRH263Level3040Fps = 30.0; // or actually 29.97
const TReal32 KCMRH263Level45Fps = 15.0; // or actually 14.985
const TReal32 KCMRH263Level50Fps = 50.0;
const TReal32 KCMRH263Level50FpsSIF = 60.0; // or actually 59.94

// Max bitrates (bps) based on codec levels
const TInt KCMRH263Level10Bps = 64000;
const TInt KCMRH263Level20Bps = 128000;
const TInt KCMRH263Level30Bps = 384000;
const TInt KCMRH263Level40Bps = 2048000;
const TInt KCMRH263Level45Bps = 128000;
const TInt KCMRH263Level50Bps = 4096000;

// max coded picture size (bytes; per H.263 requirements).
const TUint KCMRMaxCodedPictureSizeQCIF = 16384; // QCIF and smaller
const TUint KCMRMaxCodedPictureSizeCIF = 65536; // CIF and smaller

// ---------------------------------------------------------
// CCMRVideoCodecDataH263::MaxBitRate
// Checks the maximum allowed bitrate for the selected codec
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCMRVideoCodecDataH263::MaxBitRate() const
    {
    switch ( iVideoCodecLevel )
        {
        case 20 :
            return KCMRH263Level20Bps;
        case 30 :
            return KCMRH263Level30Bps;
        case 40 :
            return KCMRH263Level40Bps;
        case 45 :
            return KCMRH263Level45Bps;
        case 50 :
            return KCMRH263Level50Bps;
        case 10 : // 10 is the default level
        default :
            return KCMRH263Level10Bps;
        }
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataH263::MaxFrameRate
// Checks the maximum allowed frame rate for the selected codec
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CCMRVideoCodecDataH263::MaxFrameRate(const TSize& aFrameSize) const
    {
    switch ( iVideoCodecLevel )
        {
        case 20 :
            if ( aFrameSize.iHeight > KCMRQCIFHeight || aFrameSize.iWidth > KCMRQCIFWidth )
                {
                return KCMRH263Level20Fps;
                }
            else
                {
                // smaller size (at most QCIF) => different max rate
                return KCMRH263Level20FpsQCIF;
                }
        case 30 :
        case 40 :
            return KCMRH263Level3040Fps;
        case 45 :
            return KCMRH263Level45Fps;
        case 50 :
            if ( aFrameSize.iHeight == KCMRCIFHeight )
                {
                return KCMRH263Level50Fps;
                }
            else
                {
                // smaller size (at most 352x240) => different max rate
                return KCMRH263Level50FpsSIF;
                }
        case 10 : // 10 is the default level
        default :
            return KCMRH263Level10Fps;
        }
    }

// ---------------------------------------------------------
// CCMRVideoCodecDataH263::MaxFrameSize
// Checks if the frame size for the selected codec is acceptable
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCMRVideoCodecDataH263::MaxFrameSize( const TSize& aSize ) const
    {
    TSize maxSize;
    TBool arbitrarySizeAllowed = ETrue;
    switch ( iVideoCodecLevel )
        {
        case 20 :
        case 30 :
        case 40 :
            arbitrarySizeAllowed = EFalse;
            maxSize.SetSize(KCMRCIFWidth,KCMRCIFHeight);
            break;
        case 50 :
            arbitrarySizeAllowed = ETrue;
            maxSize.SetSize(KCMRCIFWidth,KCMRCIFHeight);
            break;
        case 45 :
            arbitrarySizeAllowed = ETrue;
            maxSize.SetSize(KCMRQCIFWidth,KCMRQCIFHeight);
            break;
        case 10 : // 10 is the default level
        default: 
            arbitrarySizeAllowed = EFalse;
            maxSize.SetSize(KCMRQCIFWidth,KCMRQCIFHeight);
        }

    // subQCIF is always the smallest one we support. 
    // Max size depends on codec level and checked above
    if ( (aSize.iWidth < KCMRSubQCIFWidth) || (aSize.iHeight < KCMRSubQCIFHeight) 
        || (aSize.iWidth > maxSize.iWidth) || (aSize.iHeight > maxSize.iHeight) )
        {
        // size not ok
        return EFalse;
        }

    // next step, check if size is arbitrary
    if ( !arbitrarySizeAllowed )
        {
        // accept only subQCIF, QCIF or CIF
        if ( ((aSize.iWidth == KCMRSubQCIFWidth) && (aSize.iHeight == KCMRSubQCIFHeight)) ||
             ((aSize.iWidth == KCMRQCIFWidth) && (aSize.iHeight == KCMRQCIFHeight)) ||
             ((aSize.iWidth == KCMRCIFWidth) && (aSize.iHeight == KCMRCIFHeight)) 
            )
            {
            // size ok
            return ETrue;
            }
        else
            {
            // size not ok
            return EFalse;
            }
        }
    else
        {
        // size ok
        return ETrue;
        }
    }


// -----------------------------------------------------------------------------
// CCMRVideoCodecDataH263::LevelForMMS
// Check if codec level indicates we are recording for MMS; then some tighter restrictions apply
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoCodecDataH263::LevelForMMS() const
    {
    if ( iVideoCodecLevel == 10 )
        {
        // H.263 level 10 is the mandatory codec for MMS
        PRINT((_L("CCMRVideoCodecDataH263::LevelForMMS() using H.263 level 10, applying MMS restrictions")));
        return ETrue;
        }
    else
        {
        PRINT((_L("CCMRVideoCodecDataH263::LevelForMMS() using some non-MMS H.263 level")));
        return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CCMRVideoCodecDataH263::MaxBufferLength
// Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoCodecDataH263::MaxBufferLength(const TSize& aFrameSize) const
    {

    if ( (aFrameSize.iHeight <= KCMRQCIFHeight) && (aFrameSize.iWidth <= KCMRQCIFWidth) )
        {
        // QCIF and smaller
        // H.263, only depends on picture size
        return KCMRMaxCodedPictureSizeQCIF;
        }
    else if ( (aFrameSize.iHeight <= KCMRCIFHeight) && (aFrameSize.iWidth <= KCMRCIFWidth) )
        {
        // QCIF+ to CIF
        return KCMRMaxCodedPictureSizeCIF;
        }
    else
        {
        // larger picture sizes not supported
        return 0;
        }
    }
    
// ---------------------------------------------------------
// CCMRVideoCodecDataH263::SetVideoCodingOptionsL
// Set misc video coding options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataH263::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions)
    {
    iSyncIntervalInPicture = aOptions.iSyncIntervalInPicture;
    
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
// CCMRVideoCodecDataH263::SetPreInitParamsL
// Set codec-specific video coding options to DevVideoRecord before initialize
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataH263::SetPreInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const
    {
    if ( iSyncIntervalInPicture > 0 )
        {
        // Set Standard specific coding options
        TH263VideoMode h263Options;
        h263Options.iAllowedPictureTypes = EH263PictureTypeI | EH263PictureTypeP;
        h263Options.iForceRoundingTypeToZero = ETrue;
        h263Options.iPictureHeaderRepetition = 0;
        h263Options.iGOBHeaderInterval = iSyncIntervalInPicture;
        TPckgC<TH263VideoMode> h263pckg(h263Options);
        aDevVideoRecord->SetCodingStandardSpecificOptionsL( h263pckg );
        }
    }
    


// ---------------------------------------------------------
// CCMRVideoCodecDataH263::SetPostInitParamsL
// Set codec-specific video coding options to DevVideoRecord after initialize
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCodecDataH263::SetPostInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const
    {
    if ( iExpectedBitErrorRate > 0.0 )
        {
        // set expected bit error rate to encoder
        aDevVideoRecord->SetChannelBitErrorRate( 0, iExpectedBitErrorRate, 0);
        }
    }

