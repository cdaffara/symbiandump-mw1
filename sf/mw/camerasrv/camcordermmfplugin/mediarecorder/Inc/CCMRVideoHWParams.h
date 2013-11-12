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
* Description:  Video parameters optimized for reference HW
*                when porting to another HW these may need to be changed
*
*/



#ifndef CCMRVIDEOHWPARAMS_H
#define CCMRVIDEOHWPARAMS_H

// INCLUDES
#include "CCMRVideoSettings.h"


// GENERIC CONSTANTS

// lipsync: 
// give larger number if video is ahead/audio is late (more audio recorded than video)
// give smaller number if video is behind/audio is early
// if value is negative, video should "pause" for a while, if positive, video should fast forward
// first values for HW codecs (DevSound outputs compressed data)
const TInt KCMRInitialVideoAudioTimeSyncHW = -100;   
const TInt KCMRPauseVideoAudioTimeSyncHW = -100;   
// the same values for SW codecs (DevSound outputs PCM)
const TInt KCMRInitialVideoAudioTimeSyncSW = 0;  
const TInt KCMRPauseVideoAudioTimeSyncSW = -60;  
// values for DirectCapture encoders (camcorderMMFPlugin doestn't provide raw image data, but encoder
// uses camera directly).
const TInt KCMRInitialVideoAudioTimeSyncDC = -200;
const TInt KCMRPauseVideoAudioTimeSyncDC = -65;
const TInt KCMRPauseVideoAudioTimeSyncDC_LowSampleRate = 0;
const TInt KCMRPauseVideoAudioTimeSyncDC_LowSampleRateValue = 8000;
const TInt KCMRPauseVideoAudioTimeSyncDC_MidSampleRate = -137;
const TInt KCMRPauseVideoAudioTimeSyncDC_MidSampleRateValue = 16000;

// default video complexity settings. If HW device doesn't support given level, 0 is used
const TInt KCMRVideoComplexityARMSubQCIF = 0; // 0 is the highest
const TInt KCMRVideoComplexityARMQCIF = 1; // 0 is the highest, but use 1 

// night mode framerate, assumes camera HW 
const TReal32 KCMRNightModeFrameRate = 3.75;//This is assumed to be KCMRFrameRate/4 but some compilers don't like if const is divided

// Average video bitrate is assumed to be 90% of the target. This depends on the HW performance and encoder's rate control etc, 
// but may also depend on the content of video and may hence be difficult to hardcode. By default the encoder is asked to use
// "semi-variable bitrate" meaning that it is asked prefer quality over constant rate (KCMRLatencyQyalityTradeoff in CCMRVideoSettings.h)
// but it depends on the encoder if it can follow the setting. This settings has direct impact to the estimated remaining recording time
// in the beginning of recording
const TReal KCMRAvgVideoBitRateScaler = 0.9;

// Preferred AAC audio samplerates based on video framesize.
const TInt KCMRAACAudioSampleRateWithVGA = 48000;
const TInt KCMRAACAudioSampleRateWithQVGA = 48000;
const TInt KCMRAACAudioSampleRateWithCIF = 16000;

// Preferred AAC audio channelmode based on video framesize (mono=1, stereo=2).
const TInt KCMRAACAudioChannelModeWithVGA = 2;
const TInt KCMRAACAudioChannelModeWithQVGA = 2;
const TInt KCMRAACAudioChannelModeWithCIF = 1;

#endif // CCMRVIDEOSETTINGS_H

// End of file
