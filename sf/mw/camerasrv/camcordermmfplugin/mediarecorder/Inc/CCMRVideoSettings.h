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
* Description:  Default settings and common types for video
*
*/



#ifndef CCMRVIDEOSETTINGS_H
#define CCMRVIDEOSETTINGS_H

// INCLUDES
#include "CCMRVideoSource.h"
#include <mmf/devvideo/devvideoconstants.h>


// GENERIC CONSTANTS

// Sub-QCIF picture size
const TInt KCMRSubQCIFHeight = 96;
const TInt KCMRSubQCIFWidth = 128;
// QCIF picture size
const TInt KCMRQCIFHeight = 144;
const TInt KCMRQCIFWidth = 176;
// CIF picture size
const TInt KCMRCIFHeight = 288;
const TInt KCMRCIFWidth = 352;
// QVGA picture size
const TInt KCMRQVGAHeight = 240;
const TInt KCMRQVGAWidth = 320;
// VGA picture size
const TInt KCMRVGAHeight = 480;
const TInt KCMRVGAWidth = 640;
// D1 / SDTV NTSC / 525 SD
const TInt KCMRNTSCHeight = 480;
const TInt KCMRNTSCWidth = 720;
// D1 / SDTV PAL / 625 SD
const TInt KCMRPALHeight = 576;
const TInt KCMRPALWidth = 720;
// HD 720p
const TInt KCMRHD720Height = 720;
const TInt KCMRHD720Width = 1280;
// HD 1080p
const TInt KCMRHD1080Height = 1088;
const TInt KCMRHD1080Width = 1920;

// special level id for MPEG-4; other levels are used as such since the levels are integers
const TInt KCMRMPEG4SPLevel0B = 9;

// special level ids for H.264/AVC
// CONSTANTS
const TUint KCMRH264AVCCodecLevel10 = 10;
const TUint KCMRH264AVCCodecLevel10b = 101;
const TUint KCMRH264AVCCodecLevel11 = 11;
const TUint KCMRH264AVCCodecLevel12 = 12;
const TUint KCMRH264AVCCodecLevel13 = 13;
const TUint KCMRH264AVCCodecLevel20 = 20;
const TUint KCMRH264AVCCodecLevel21 = 21;
const TUint KCMRH264AVCCodecLevel22 = 22;
const TUint KCMRH264AVCCodecLevel30 = 30;
const TUint KCMRH264AVCCodecLevel31 = 31;
const TUint KCMRH264AVCCodecLevel32 = 32;
const TUint KCMRH264AVCCodecLevel40 = 40;

// number of supported H.263 levels
const TInt KCMRNumSupportedH263Levels = 6; // 10, 20, 30, 40, 45, 50
// supported H.263 levels in descending order (support of level 45 implies support of level 10,
// and hence it is really between 10 and 20). Defined as macro since can't have const
// TBuf8 as static (requires run-time initialization => would be writable static data)
#define KCMRH263Levels \
        {_L8("50"), \
         _L8("40"), \
         _L8("30"), \
         _L8("20"), \
         _L8("45"), \
         _L8("10")}

// number of supported MPEG-4 profile-level ids
const TInt KCMRNumSupportedMPEG4ProfileLevels = 8;
// supported MPEG-4 profile-level id's in descending order; last two are 0b and 0
// Defined as macro since can't have const TBuf8 as static (requires run-time
// initialization => would be writable static data)
#define KCMRMPEG4Levels \
        {_L8("8"), \
         _L8("9"), \
         _L8("1"), \
         _L8("2"), \
         _L8("3"), \
         _L8("4"), \
         _L8("5"), \
         _L8("6")}

// number of supported H.264 AVC profile-level ids
const TInt KCMRNumSupportedH264AVCProfileLevels = 36;
// supported H.264 AVC profile-level id's
// Defined as macro since can't have const TBuf8 as static (requires run-time
// initialization => would be writable static data)
#define KCMRH264AVCLevels \
        {_L8("42800A"), \
         _L8("42900B"), \
         _L8("42800B"), \
         _L8("42800C"), \
         _L8("42800D"), \
         _L8("428014"), \
         _L8("428015"), \
         _L8("428016"), \
         _L8("42801E"), \
         _L8("42801F"), \
         _L8("428020"), \
         _L8("428028"), \
         _L8("4D400A"), \
         _L8("4D500B"), \
         _L8("4D400B"), \
         _L8("4D400C"), \
         _L8("4D400D"), \
         _L8("4D4014"), \
         _L8("4D4015"), \
         _L8("4D4016"), \
         _L8("4D401E"), \
         _L8("4D401F"), \
         _L8("4D4020"), \
         _L8("4D4028"), \
         _L8("64400A"), \
         _L8("644009"), \
         _L8("64400B"), \
         _L8("64400C"), \
         _L8("64400D"), \
         _L8("644014"), \
         _L8("644015"), \
         _L8("644016"), \
         _L8("64401E"), \
         _L8("64401F"), \
         _L8("644020"), \
         _L8("644028")}

const TInt KCMRMinAcceptedBitRate = 10000; // 10000 bps is considered the smallest reasonable bitrate

// OUTPUT PARAMETERS (default)

// default target bitrate
const TUint KCMRTargetBitRate = 64000;

// default frame size = QCIF
const TInt KCMRFrameWidth = 176;
const TInt KCMRFrameHeight = 144;

// default frame rate
const TReal32 KCMRFrameRate = 15;

// SOURCE OPTIONS

// number of camera buffers to use; 1st one is default & used with HW accelerated encoder, the 2nd one is for ARM encoder
const TUint KCMRNumCameraBuffers = 2;
const TUint KCMRNumCameraBuffersARM = 1;

// number of frames per one camera buffer
const TUint KCMRNumFramesInCameraBuffer = 1;



// ENCODER OPTIONS

// max. number of pictures in the buffer before encoder
const TUint KCMRMaxPreEncoderBufferPictures = KCMRNumCameraBuffers;

// min number of output (bitstream) buffers
const TUint KCMRMinNumOutputBuffers = 4;

// One random access point every 5 seconds
const TInt KCMRMinRandomAccessPeriod = 5;
const TInt KCMRMinRandomAccessPeriodHighRes = 1;

// If == 0 no other sync codes in picture than picture start code.
// For H.263 baseline, value > 0 refers to the number of GOBs in segment, in MB-based segments nr of MBs per segment
const TInt KCMRSyncIntervalInPicture = 0;

// Encoding in real-time or not? Non-real-time probably needed only in debugging
const TBool KCMREncodingRealTime = ETrue;

// input format parameters to video codec
// Aspect ratio, in H.263 default is 12:11, but camera probably provides 1:1
const TInt KCMRAspectRatioNum = 12;
const TInt KCMRAspectRatioDenom = 11;
// Aspect ratio MPEG-4
const TInt KCMRMPEG4AspectRatioNum = 1;
const TInt KCMRMPEG4AspectRatioDenom = 1;

//   Rate control

// Quality is more important in camcorder than delay
const TReal KCMRLatencyQyalityTradeoff = 1.0;

// range is [0...100]
const TUint KCMRPictureQuality = 50;

// Try to maintain the frame-rate
// (1.0 means the picture quality is sacrificed
// "all the way" to match the target fps
const TReal KCMRQualityTemporalTradeoff = 0.8;

// Error control

// Very strong simplification for the thresholds to map between packet loss rates and bit error rates; used to set
// adaptive intra refresh parameters for the encoder, which currently supports only bit error rate based setting
const TReal KCMRBadBitErrorRate = 0.001;
const TReal KCMRTolerableBitErrorRate = 0.0001;
const TReal KCMRNotNoticeableBitErrorRate = 0.00001;

const TReal KCMRBadPacketLossRate = 0.05;
const TReal KCMRNotNoticeablePacketLossRate = 0.01;

#endif // CCMRVIDEOSETTINGS_H

// End of file
