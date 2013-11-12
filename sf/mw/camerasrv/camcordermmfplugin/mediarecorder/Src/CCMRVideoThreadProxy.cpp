/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Video thread proxy
*
*/


// INCLUDES
#include "CCMRVideoThreadProxy.h"
#include "CCMRVideoThreadProxyServer.h"
#include "CCMRVideoSettings.h"
#include "CCMRMediaRecorder.h"  // for TCCMRVideoCodingOptions
#include "CCMRThreadPriorities.h"
#include <e32math.h>

// MACROS

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#define KCMRVideoThreadProxyVersion TVersion(1,0,0)



// ========================= RCMRVideoThreadProxy MEMBER FUNCTIONS =========================

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::CreateSubThread
// Create thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::CreateSubThread(TBool& aThreadCreated, CCMRConfigManager* aConfig )
    {
    PRINT((_L("RCMRVideoThreadProxy::CreateSubThread() in")));

    //start the subthread with no name & create session
    TInt error = DoCreateSubThread(&CCMRVideoThreadProxyServer::StartThread, KCMRVideoThreadProxyVersion, aThreadCreated);
    if (error)
        {
        return error;
        }
    iSessionCreated = ETrue;

    // load video recorder
    error = SendReceive( ECMRVideoThreadLoadVideoRecorder, TIpcArgs(aConfig) );
    if (error)
        {
        return error;
        }
    // set thread id
    TUint threadID = RThread().Id();
    error = SendReceive( ECMRVideoThreadSetOutputThreadId, TIpcArgs(threadID) );


    PRINT((_L("RCMRVideoThreadProxy::CreateSubThread() out")));
    return error;
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetCameraHandle
// Set camera handle
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetCameraHandle(TInt aCameraHandle) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetCameraHandle()")));
    return SendReceive( ECMRVideoThreadSetCameraHandle, TIpcArgs(aCameraHandle ));
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetOutput
// Set output active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetOutput(CCMRActiveOutput* aOutput) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetOutput()")));
    return SendReceive( ECMRVideoThreadSetOutput, TIpcArgs(aOutput) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetClockSource
// Set clock source
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetClockSource(MMMFClockSource* aClockSource) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetClockSource()")));
    return SendReceive( ECMRVideoThreadSetClockSource, TIpcArgs(aClockSource) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetBitRate
// Set target bitrate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetBitRate(TInt aBitRate) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetBitRate()")));
    return SendReceive( ECMRVideoThreadSetBitRate, TIpcArgs(aBitRate) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::GetBitRate
// Get current target bitrate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::GetBitRate(TInt& aBitRate) const
    {
    PRINT((_L("RCMRVideoThreadProxy::GetBitRate()")));
    return SendReceive( ECMRVideoThreadGetBitRate, TIpcArgs(&aBitRate) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetFrameSize
// Set video frame size (width&height)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetFrameSize(const TSize& aSize) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetFrameSize()")));
    return SendReceive( ECMRVideoThreadSetFrameSize, TIpcArgs(&aSize) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::GetFrameSize
// Get current video frame size (width&height)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::GetFrameSize(TSize& aSize) const
    {
    PRINT((_L("RCMRVideoThreadProxy::GetFrameSize()")));
    return SendReceive( ECMRVideoThreadGetFrameSize, TIpcArgs(&aSize) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetFrameRate
// Set video framerate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetFrameRate(TReal32 aFrameRate) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetFrameRate()")));
    return SendReceive( ECMRVideoThreadSetFrameRate, TIpcArgs(&aFrameRate) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::GetFrameRate
// Get current video framerate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::GetFrameRate(TReal32& aFrameRate) const
    {
    PRINT((_L("RCMRVideoThreadProxy::GetFrameRate()")));
    return SendReceive( ECMRVideoThreadGetFrameRate, TIpcArgs(&aFrameRate) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetCodec
// Set video codec MIME-type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetCodec(const TDesC8& aMimeType) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetCodec()")));
    return SendReceive( ECMRVideoThreadSetCodec, TIpcArgs(&aMimeType) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::GetCodec
// Get current video codec MIME-type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::GetCodec(TDes8& aMimeType) const
    {
    PRINT((_L("RCMRVideoThreadProxy::GetCodec()")));
    return SendReceive( ECMRVideoThreadGetCodec, TIpcArgs(&aMimeType) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetMiscOptions
// Set video options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetMiscOptions(const TCCMRVideoCodingOptions& aOptions) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetMiscOptions()")));
    return SendReceive( ECMRVideoThreadSetMiscOptions, TIpcArgs(&aOptions) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetVideoRateControlOptions
// Set video rate control options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetVideoRateControlOptions(const TRateControlOptions& aOptions) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetVideoRateControlOptions()")));
    return SendReceive( ECMRVideoThreadSetVideoRateControlOptions, TIpcArgs(&aOptions) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::GetVideoRateControlOptions
// Get video rate control options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::GetVideoRateControlOptions(TRateControlOptions& aOptions) const
    {
    PRINT((_L("RCMRVideoThreadProxy::GetVideoRateControlOptions()")));
    return SendReceive( ECMRVideoThreadGetVideoRateControlOptions, TIpcArgs(&aOptions) );
    }

// ---------------------------------------------------------
// RCMRVideoThreadProxy::SetPreferredVideoEncoderL
// Set video encoder using its UID. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetPreferredVideoEncoder(TUid& aEncoder)
    {
    PRINT((_L("RCMRVideoThreadProxy::SetPreferredVideoEncoder()")));
    return SendReceive( ECMRVideoThreadSetPreferredVideoEncoder, TIpcArgs(&aEncoder) );
    }

// ---------------------------------------------------------
// RCMRVideoThreadProxy::SetPreferredVideoEncapsulationL
// Set video encoder output format encapsulation. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetPreferredVideoEncapsulation(TVideoDataUnitEncapsulation aCapsulation)
    {
    PRINT((_L("RCMRVideoThreadProxy::SetPreferredVideoEncapsulation()")));
    return SendReceive( ECMRVideoThreadSetPreferredVideoEncapsulation, TIpcArgs(&aCapsulation) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetSegmentTargetSizeL
// Set video segment size. Usage optional.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool RCMRVideoThreadProxy::SetSegmentTargetSize(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks )
    {
    PRINT((_L("RCMRVideoThreadProxy::SetSegmentTargetSize()")));
    return SendReceive( ECMRVideoThreadSetSegmentTargetSize, TIpcArgs(aLayer, aSizeBytes, aSizeMacroblocks) );
    }


// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::AdjustTimeStamps
// Adjust video time stamps (+ or -)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::AdjustTimeStamps(const TInt aAdjustmentMs) const
    {
    PRINT((_L("RCMRVideoThreadProxy::AdjustTimeStamps()")));
    return SendReceive( ECMRVideoThreadAdjustTimeStamps, TIpcArgs(aAdjustmentMs ) );
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::SetThreadPriority
// Set video thread priority based on audio thread priority, video frame size, and
// defined constants
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::SetThreadPriority(const TThreadPriority& audioThreadPriority) const
    {
    PRINT((_L("RCMRVideoThreadProxy::SetThreadPriority()")));
    if ( audioThreadPriority > EPriorityNormal )
        {
        // audio is already having a higher priority, can't raise video above normal
        iSubThread.SetPriority(KCMRVideoThreadPriorityNormal);
        PRINT((_L("RCMRVideoThreadProxy::SetThreadPriority() set priority to %d since audio thread had a higher priority"), KCMRVideoThreadPriorityNormal));
        }
    else
        {
        TSize frameSize;
        TInt err = GetFrameSize( frameSize );

        if ( err != KErrNone )
            {
            PRINT((_L("RCMRVideoThreadProxy::SetThreadPriority() getting frame size failed")));
            return err;
            }

        if ( frameSize.iWidth > KCMRQCIFWidth )
            {
            // consider resolutions higher than QCIF as high performance which may require 
            // different thread priority. Please check the defined video and audio thread 
            // priorities in CCMRThreadPriorities.h
            // Having both video and audio as high priority may not make sense; in some
            // cases audio must have higher, and in some cases video, sometimes it doesn't matter
            iSubThread.SetPriority(KCMRVideoThreadPriorityHighPerf);
            PRINT((_L("RCMRVideoThreadProxy::SetThreadPriority() set priority to %d since video resolution is high"), KCMRVideoThreadPriorityHighPerf));
            }
        else
            {
            // normal performance
            iSubThread.SetPriority(KCMRVideoThreadPriorityNormal);
            PRINT((_L("RCMRVideoThreadProxy::SetThreadPriority() set priority to %d since video resolution is low"), KCMRVideoThreadPriorityNormal));
            }
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::Prepare
// Prepare for recording (freeze settings)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::Prepare() const
    {
    PRINT((_L("RCMRVideoThreadProxy::Prepare()")));
    return SendReceive(ECMRVideoThreadPrepare);
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::Record
// Start recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::Record() const
    {
    PRINT((_L("RCMRVideoThreadProxy::Record()")));
    return SendReceive(ECMRVideoThreadRecord);
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::Stop
// Stop video recorder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::Stop() const
    {
    PRINT((_L("RCMRVideoThreadProxy::Stop()")));
    return SendReceive(ECMRVideoThreadStop);
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::RequestBuffersAndWaitEOSL
// Wait until video recorder is stopped and receives EOS marker
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::RequestBuffersAndWaitEOSL(TInt& aVideoEOSReached) const
    {
    PRINT((_L("RCMRVideoThreadProxy::RequestBuffersAndWaitEOSL()")));
    return SendReceive( ECMRVideoRequestBuffersAndThreadWaitEOS, TIpcArgs(&aVideoEOSReached) );
    }




// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::Pause
// Pause video recorder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::Pause() const
    {
    PRINT((_L("RCMRVideoThreadProxy::Pause()")));
    return SendReceive(ECMRVideoThreadPause);
    }

// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::Resume
// Resume video recorder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRVideoThreadProxy::Resume() const
    {
    PRINT((_L("RCMRVideoThreadProxy::Resume()")));
    return SendReceive(ECMRVideoThreadResume);
    }


// -----------------------------------------------------------------------------
// RCMRVideoThreadProxy::Close
// Close thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCMRVideoThreadProxy::Close()
    {
    PRINT((_L("RCMRVideoThreadProxy::Close() in")));
    Shutdown();
    PRINT((_L("RCMRVideoThreadProxy::Close() out")));
    }








