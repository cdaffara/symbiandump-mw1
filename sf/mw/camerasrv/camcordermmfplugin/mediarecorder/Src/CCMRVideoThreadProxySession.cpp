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
* Description:  Video thread proxy classes
*
*/

//INCLUDES

#include "CCMRVideoThreadProxySession.h"
#include "CCMRMediaRecorder.h"  // for TCCMRVideoCodingOptions
#include "CCMRVideoRecorder.h"


// MACROS

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif


// ====================== CCMRVideoThreadProxySession MEMBER FUNCTIONS =====================


// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRVideoThreadProxySession* CCMRVideoThreadProxySession::NewL()
    {
    return new(ELeave) CCMRVideoThreadProxySession();
    }

// destructor
CCMRVideoThreadProxySession::~CCMRVideoThreadProxySession()
    {
    PRINT((_L("CCMRVideoThreadProxySession::~CCMRVideoThreadProxySession() in")));
    delete iRecorder;
    PRINT((_L("CCMRVideoThreadProxySession::~CCMRVideoThreadProxySession() out")));
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::ServiceL
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoThreadProxySession::ServiceL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRVideoThreadProxySession::ServiceL() in")));
    TBool complete = EFalse;
    switch(aMessage.Function())
        {
        case ECMRVideoThreadLoadVideoRecorder:
            complete = LoadVideoRecorderL(aMessage);
            break;
        case ECMRVideoThreadSetCameraHandle:
            complete = SetCameraHandleL(aMessage);
            break;
        case ECMRVideoThreadSetOutput:
            complete = SetOutputL(aMessage);
            break;
        case ECMRVideoThreadSetClockSource:
            complete = SetClockSource(aMessage);
            break;
        case ECMRVideoThreadSetOutputThreadId:
            complete = SetOutputThreadIdL(aMessage);
            break;
        case ECMRVideoThreadSetBitRate:
            complete = SetBitRateL(aMessage);
            break;
        case ECMRVideoThreadGetBitRate:
            complete = GetBitRateL(aMessage);
            break;
        case ECMRVideoThreadSetFrameSize:
            complete = SetFrameSizeL(aMessage);
            break;
        case ECMRVideoThreadGetFrameSize:
            complete = GetFrameSizeL(aMessage);
            break;
        case ECMRVideoThreadSetFrameRate:
            complete = SetFrameRateL(aMessage);
            break;
        case ECMRVideoThreadGetFrameRate:
            complete = GetFrameRateL(aMessage);
            break;
        case ECMRVideoThreadSetCodec:
            complete = SetCodecL(aMessage);
            break;
        case ECMRVideoThreadGetCodec:
            complete = GetCodecL(aMessage);
            break;
        case ECMRVideoThreadSetMiscOptions:
            complete = SetMiscOptionsL(aMessage);
            break;
        case ECMRVideoThreadSetVideoRateControlOptions:
            complete = SetVideoRateControlOptionsL(aMessage);
            break;
        case ECMRVideoThreadGetVideoRateControlOptions:
            complete = GetVideoRateControlOptionsL(aMessage);
            break;
        case ECMRVideoThreadSetPreferredVideoEncoder:
            complete = SetPreferredVideoEncoderL(aMessage);
            break;
        case ECMRVideoThreadSetPreferredVideoEncapsulation:
            complete = SetPreferredVideoEncapsulationL(aMessage);
            break;
        case ECMRVideoThreadSetSegmentTargetSize:
            complete = SetSegmentTargetSizeL(aMessage);
            break;            
        case ECMRVideoThreadAdjustTimeStamps:
            complete = AdjustTimeStampsL(aMessage);
            break;
        case ECMRVideoThreadPrepare:
            complete = PrepareL();
            break;
        case ECMRVideoThreadRecord:
            complete = RecordL();
            break;
        case ECMRVideoThreadStop:
            complete = StopL();
            break;
        case ECMRVideoRequestBuffersAndThreadWaitEOS:
            complete = RequestBuffersAndWaitEOSL(aMessage);
            break;            
        case ECMRVideoThreadPause:
            complete = PauseL();
            break;
        case ECMRVideoThreadResume:
            complete = ResumeL();
            break;
        case ECMRThreadReceiveEvents:
            complete = ReceiveEventsL(aMessage);//provided by baseclass
            break;
        case ECMRThreadCancelReceiveEvents:
            complete = CancelReceiveEvents();//provided by baseclass
            break;
        case ECMRThreadShutdown:
            complete = ShutDown();//provided by baseclass
            break;
        default:
            PRINT((_L("CCMRVideoThreadProxySession::ServiceL() unknown msg")));
            User::Leave(KErrNotSupported);
            break;
        }

    if (complete)
        {
        aMessage.Complete(KErrNone);
        }

    PRINT((_L("CCMRVideoThreadProxySession::ServiceL() out")));
    }


// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::LoadVideoRecorderL
// Load (create) video recorder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::LoadVideoRecorderL(const RMessage2& aMessage)
    {
	CCMRConfigManager* config = reinterpret_cast<CCMRConfigManager*>(aMessage.Int0());    
    iRecorder = CCMRVideoRecorder::NewL( *this, config);
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetCameraHandleL
// Set camera handle
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetCameraHandleL(const RMessage2& aMessage)
    {
    TInt handle = aMessage.Int0();
    iRecorder->SetCameraHandleL( handle );
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetOutputL
// Set output active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetOutputL(const RMessage2& aMessage)
    {
    CCMRActiveOutput* output = reinterpret_cast<CCMRActiveOutput*>(aMessage.Int0());
    iRecorder->SetOutputL( output );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetClockSource
// Set clock source
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetClockSource(const RMessage2& aMessage)
    {
    MMMFClockSource* clocksource = reinterpret_cast<MMMFClockSource*>(aMessage.Int0());
    iRecorder->SetClockSource( clocksource );
    return ETrue;
    }    
// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetOutputThreadIdL
// Set thread id of the client & output active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetOutputThreadIdL(const RMessage2& aMessage)
    {
    TUint id = (TUint)(aMessage.Int0());
    iRecorder->SetOutputThreadIdL( id );
    return ETrue;
    }
// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetBitRateL
// Set target bitrate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetBitRateL(const RMessage2& aMessage)
    {
    TInt rate = aMessage.Int0();
    iRecorder->SetTargetBitRateL( rate );
    return ETrue;
    }
// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::GetBitRateL
// Get current target bitrate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::GetBitRateL(const RMessage2& aMessage) const
    {
    TInt* rate = reinterpret_cast<TInt*>(aMessage.Int0());
    iRecorder->TargetBitRateL(*rate);
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetFrameSizeL
// Set video frame size (width&height)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetFrameSizeL(const RMessage2& aMessage)
    {
    TSize* size = reinterpret_cast<TSize*>(aMessage.Int0());
    iRecorder->SetFrameSizeL( *size );
    return ETrue;
    }
// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::GetFrameSizeL
// Get current video frame size (width&height)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::GetFrameSizeL(const RMessage2& aMessage) const
    {
    TSize* size = reinterpret_cast<TSize*>(aMessage.Int0());
    iRecorder->FrameSizeL( *size );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetFrameRateL
// Set video framerate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetFrameRateL(const RMessage2& aMessage)
    {
    TReal32* rate = reinterpret_cast<TReal32*>(aMessage.Int0());
    iRecorder->SetFrameRateL( *rate );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::GetFrameRateL
// Get current video framerate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::GetFrameRateL(const RMessage2& aMessage) const
    {
    TReal32* rate = reinterpret_cast<TReal32*>(aMessage.Int0());
    iRecorder->FrameRateL( *rate );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetCodecL
// Set video codec MIME-type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetCodecL(const RMessage2& aMessage)
    {
    TDesC8* codec = reinterpret_cast<TDesC8*>(aMessage.Int0());
    iRecorder->SetVideoCodecL( *codec );
    return ETrue;
    }
// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::GetCodecL
// Get current video codec MIME-type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::GetCodecL(const RMessage2& aMessage) const
    {
    TDes8* codec = reinterpret_cast<TDes8*>(aMessage.Int0());
    iRecorder->GetVideoCodecL( *codec );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetMiscOptionsL
// Set video options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetMiscOptionsL(const RMessage2& aMessage)
    {
    TCCMRVideoCodingOptions* options = reinterpret_cast<TCCMRVideoCodingOptions*>(aMessage.Int0());
    iRecorder->SetVideoCodingOptionsL( *options );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetVideoRateControlOptionsL
// Set video rate control options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetVideoRateControlOptionsL(const RMessage2& aMessage)
    {
    TRateControlOptions* options = reinterpret_cast<TRateControlOptions*>(aMessage.Int0());
    iRecorder->SetVideoRateControlOptionsL( *options );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::GetVideoRateControlOptionsL
// Get video rate control options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::GetVideoRateControlOptionsL(const RMessage2& aMessage) const
    {
    TRateControlOptions* options = reinterpret_cast<TRateControlOptions*>(aMessage.Int0());
    iRecorder->GetVideoRateControlOptionsL( *options );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetPreferredVideoEncoderL
// Set video encoder using its UID. Usage optional
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetPreferredVideoEncoderL(const RMessage2& aMessage)
    {
    TUid* uid = reinterpret_cast<TUid*>(aMessage.Int0());
    iRecorder->SetPreferredVideoEncoderL( *uid );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetPreferredVideoEncapsulationL
// Set video encoder output format encapsulation. Usage optional.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetPreferredVideoEncapsulationL(const RMessage2& aMessage)
    {
    TVideoDataUnitEncapsulation* encapsulation = reinterpret_cast<TVideoDataUnitEncapsulation*>(aMessage.Int0());
    iRecorder->SetPreferredVideoEncapsulationL( *encapsulation );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SetSegmentTargetSizeL
// Set video segment size. Usage optional.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::SetSegmentTargetSizeL(const RMessage2& aMessage)
    {
    TUint layer = aMessage.Int0();
    TUint sizeBytes = aMessage.Int1();
    TUint sizeMacroblocks = aMessage.Int2();
    iRecorder->SetSegmentTargetSizeL(layer, sizeBytes, sizeMacroblocks );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::AdjustTimeStampsL
// Adjust video time stamps (+ or -)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::AdjustTimeStampsL(const RMessage2& aMessage)
    {
    TInt adjustment = aMessage.Int0();
    iRecorder->AdjustTimeStampsL( adjustment );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::PrepareL
// Prepare video for recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::PrepareL()
    {
    PRINT((_L("CCMRVideoThreadProxySession::PrepareL() in")));    
    iRecorder->PrepareL();
    PRINT((_L("CCMRVideoThreadProxySession::PrepareL() out")));    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::RecordL
// Start recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::RecordL()
    {
    PRINT((_L("CCMRVideoThreadProxySession::RecordL() in")));    
    iRecorder->RecordL();
    PRINT((_L("CCMRVideoThreadProxySession::RecordL() out")));    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::StopL
// Stop recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::StopL()
    {
    PRINT((_L("CCMRVideoThreadProxySession::StopL() in")));    
    iRecorder->StopL();
    PRINT((_L("CCMRVideoThreadProxySession::StopL() out")));       
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::RequestBuffersAndWaitEOSL
// Wait until video thread is stopped and receives EOS marker
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::RequestBuffersAndWaitEOSL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRVideoThreadProxySession::RequestBuffersAndWaitEOSL() in")));    
    TInt* videoEOSReached = reinterpret_cast<TInt*>(aMessage.Int0());
    iRecorder->RequestBuffersAndWaitEOSL(*videoEOSReached);
    PRINT((_L("CCMRVideoThreadProxySession::RequestBuffersAndWaitEOSL() out")));
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::PauseL
// Pause recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::PauseL()
    {
    iRecorder->PauseL();
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::ResumeL
// Resume recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoThreadProxySession::ResumeL()
    {
    iRecorder->ResumeL();
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CCMRVideoThreadProxySession::SendEventToClient
// Catches completion events and sends other events to client using base class
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoThreadProxySession::SendEventToClient(const TMMFEvent& aEvent)
    {
    PRINT((_L("CCMRVideoThreadProxySession::SendEventToClient() in")));
    if ( aEvent.iEventType == KCMRRecordingComplete )
        {
        PRINT((_L("CCMRVideoThreadProxySession::SendEventToClient() EOS received = stop success or fatal error")));        
        // we were waiting for the recorder to stop, release now also the client from waiting (stop is sync for the client)
        iStopping = EFalse;
        PRINT((_L("CCMRVideoThreadProxySession::SendEventToClient() out, KCMRRecordingComplete sent forward")));     
        return CCMRThreadProxySession::SendEventToClient( aEvent );
        }
    else 
        {
        // call base class implementation
        PRINT((_L("CCMRVideoThreadProxySession::SendEventToClient() out")));     
        return CCMRThreadProxySession::SendEventToClient( aEvent );
        }
    }
