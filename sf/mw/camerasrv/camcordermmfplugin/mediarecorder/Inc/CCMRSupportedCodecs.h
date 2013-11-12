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
* Description:  Four CC and Mime types for (supported) video and audio codecs
*
*/



#ifndef CCMRSUPPORTEDCODECS_H
#define CCMRSUPPORTEDCODECS_H

#include <mmf/common/mmffourcc.h>

// FourCC types for video and audio

// These can't be TFourCC objects since TFourCC objects can't be constructed in compile time
// The ID is internal representation of TFourCC, and computed like follows:
// iFourCC =(aChar4<<24)+(aChar3<<16)+(aChar2<<8)+aChar1
// The users should construct a TFourCC object using these numbers

// Four CC for narrowband AMR (' ','A','M','R'), taken from Nokia MMF audio plugin source code
const TInt32 KCMRFourCCIdAMRNB = KMMFFourCCCodeAMR;
// Four CC for MPEG-4 AAC (' ','A','A','C')
const TInt32 KCMRFourCCIdMPEG4AAC = KMMFFourCCCodeAAC;


// Mime types for video

// H.263 with profiles & levels. Exact level can be added after this string has been copied to destination
_LIT8(KCMRMimeTypeH263, "video/H263-2000");
// Baseline Profile
_LIT8(KCMRMimeTypeH263BaselineProfile, "video/H263-2000; profile=0");
// Profile 3; Version 2 Interactive and Streaming Wireless Profile
_LIT8(KCMRMimeTypeH263Profile3, "video/H263-2000; profile=3");

// MPEG-4 profile & level ID added after ; e.g. "video/mp4v-es; profile-level-id=8"
_LIT8(KCMRMimeTypeMPEG4V, "video/mp4v-es");
_LIT8(KCMRMimeTypeMPEG4VSP, "video/mp4v-es; profile-level-id=");
_LIT8(KCMRMimeTypeMPEG4VSPL0, "video/mp4v-es; profile-level-id=8");
_LIT8(KCMRMimeTypeMPEG4VSPL0B, "video/mp4v-es; profile-level-id=9");
_LIT8(KCMRMimeTypeMPEG4VSPL1, "video/mp4v-es; profile-level-id=1");
_LIT8(KCMRMimeTypeMPEG4VSPL2, "video/mp4v-es; profile-level-id=2");
_LIT8(KCMRMimeTypeMPEG4VSPL3, "video/mp4v-es; profile-level-id=3");
_LIT8(KCMRMimeTypeMPEG4VSPL4A, "video/mp4v-es; profile-level-id=4");
_LIT8(KCMRMimeTypeMPEG4VSPL5, "video/mp4v-es; profile-level-id=5");
_LIT8(KCMRMimeTypeMPEG4VSPL6, "video/mp4v-es; profile-level-id=6");

// H.264 AVC 
_LIT8(KCMRMimeTypeH264AVC, "video/H264");
_LIT8(KCMRMimeTypeH264AVCProfileId, "video/H264; profile-level-id=");

// H.264 AVC Baseline profile, levels 1 - 4
_LIT8(KCMRMimeTypeH264AVCBPL10, "video/H264; profile-level-id=42800A");	// Level 1
_LIT8(KCMRMimeTypeH264AVCBPL10b, "video/H264; profile-level-id=42900B");// Level 1b
_LIT8(KCMRMimeTypeH264AVCBPL11, "video/H264; profile-level-id=42800B");	// Level 1.1
_LIT8(KCMRMimeTypeH264AVCBPL12, "video/H264; profile-level-id=42800C");	// Level 1.2
_LIT8(KCMRMimeTypeH264AVCBPL13, "video/H264; profile-level-id=42800D");	// Level 1.3
_LIT8(KCMRMimeTypeH264AVCBPL20, "video/H264; profile-level-id=428014");	// Level 2
_LIT8(KCMRMimeTypeH264AVCBPL21, "video/H264; profile-level-id=428015"); // Level 2.1
_LIT8(KCMRMimeTypeH264AVCBPL22, "video/H264; profile-level-id=428016"); // Level 2.2
_LIT8(KCMRMimeTypeH264AVCBPL30, "video/H264; profile-level-id=42801E"); // Level 3
_LIT8(KCMRMimeTypeH264AVCBPL31, "video/H264; profile-level-id=42801F"); // Level 3.1
_LIT8(KCMRMimeTypeH264AVCBPL32, "video/H264; profile-level-id=428020"); // Level 3.2
_LIT8(KCMRMimeTypeH264AVCBPL40, "video/H264; profile-level-id=428028"); // Level 4

// H.264 AVC Main profile, levels 1 - 4
_LIT8(KCMRMimeTypeH264AVCMPL10, "video/H264; profile-level-id=4D400A");	// Level 1
_LIT8(KCMRMimeTypeH264AVCMPL10b, "video/H264; profile-level-id=4D500B");// Level 1b
_LIT8(KCMRMimeTypeH264AVCMPL11, "video/H264; profile-level-id=4D400B");	// Level 1.1
_LIT8(KCMRMimeTypeH264AVCMPL12, "video/H264; profile-level-id=4D400C");	// Level 1.2
_LIT8(KCMRMimeTypeH264AVCMPL13, "video/H264; profile-level-id=4D400D");	// Level 1.3
_LIT8(KCMRMimeTypeH264AVCMPL20, "video/H264; profile-level-id=4D4014");	// Level 2
_LIT8(KCMRMimeTypeH264AVCMPL21, "video/H264; profile-level-id=4D4015"); // Level 2.1
_LIT8(KCMRMimeTypeH264AVCMPL22, "video/H264; profile-level-id=4D4016"); // Level 2.2
_LIT8(KCMRMimeTypeH264AVCMPL30, "video/H264; profile-level-id=4D401E"); // Level 3
_LIT8(KCMRMimeTypeH264AVCMPL31, "video/H264; profile-level-id=4D401F"); // Level 3.1
_LIT8(KCMRMimeTypeH264AVCMPL32, "video/H264; profile-level-id=4D4020"); // Level 3.2
_LIT8(KCMRMimeTypeH264AVCMPL40, "video/H264; profile-level-id=4D4028"); // Level 4

// H.264 AVC High profile, levels 1 - 4
_LIT8(KCMRMimeTypeH264AVCHPL10, "video/H264; profile-level-id=64400A");	// Level 1
_LIT8(KCMRMimeTypeH264AVCHPL10b, "video/H264; profile-level-id=644009");// Level 1b
_LIT8(KCMRMimeTypeH264AVCHPL11, "video/H264; profile-level-id=64400B");	// Level 1.1
_LIT8(KCMRMimeTypeH264AVCHPL12, "video/H264; profile-level-id=64400C");	// Level 1.2
_LIT8(KCMRMimeTypeH264AVCHPL13, "video/H264; profile-level-id=64400D");	// Level 1.3
_LIT8(KCMRMimeTypeH264AVCHPL20, "video/H264; profile-level-id=644014");	// Level 2
_LIT8(KCMRMimeTypeH264AVCHPL21, "video/H264; profile-level-id=644015"); // Level 2.1
_LIT8(KCMRMimeTypeH264AVCHPL22, "video/H264; profile-level-id=644016"); // Level 2.2
_LIT8(KCMRMimeTypeH264AVCHPL30, "video/H264; profile-level-id=64401E"); // Level 3
_LIT8(KCMRMimeTypeH264AVCHPL31, "video/H264; profile-level-id=64401F"); // Level 3.1
_LIT8(KCMRMimeTypeH264AVCHPL32, "video/H264; profile-level-id=644020"); // Level 3.2
_LIT8(KCMRMimeTypeH264AVCHPL40, "video/H264; profile-level-id=644028"); // Level 4

#endif // CCMRSUPPORTEDCODECS_H

// End of file
