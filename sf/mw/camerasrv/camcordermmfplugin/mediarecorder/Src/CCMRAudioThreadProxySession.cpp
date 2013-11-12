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
* Description:  Audio thread proxy classes
*
*/



//INCLUDES
#include "CCMRAudioThreadProxySession.h"
#include "CCMRAudioCodecData.h"
#include "CCMRAudioInputSW.h"
#include "CCMRAudioInputHW.h"

#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfaudioinput.h>

//MACROS

// Assertion macro wrapper for code cleanup
#define APASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRAUDIOTHREADPROXYSESSION"), EInternalAssertionFailure)) 

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// Timeout for pausing audioinput
#if (defined __WINS__) || (defined __WINSCW__)
const TInt KCMRPauseTimeout = 2000000;  // 2 seconds
#else
const TInt KCMRPauseTimeout = 1000000;  // 1 second
#endif

// ====================== CCMRAudioThreadProxySession MEMBER FUNCTIONS =====================


// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRAudioThreadProxySession* CCMRAudioThreadProxySession::NewL()
    {
    return new(ELeave) CCMRAudioThreadProxySession();
    }

// destructor
CCMRAudioThreadProxySession::~CCMRAudioThreadProxySession()
    {
    PRINT((_L("CCMRAudioThreadProxySession::~CCMRAudioThreadProxySession() in")));
    delete iPauseTimer;
    iPauseTimer = NULL;

    if ( iMMFAudioInput )
        {
        iMMFAudioInput->SourceThreadLogoff();
        }

    delete iAudioInput;

    PRINT((_L("CCMRAudioThreadProxySession::~CCMRAudioThreadProxySession() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::ServiceL
// Service messages
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioThreadProxySession::ServiceL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRAudioThreadProxySession::ServiceL() in")));
    TBool complete = EFalse;
    switch(aMessage.Function())
        {
    case ECMRAudioThreadSetOutput:
        complete = SetOutputL(aMessage);
        break;
    case ECMRAudioThreadAddDataSource:
        complete = AddDataSourceL(aMessage);
        break;
    case ECMRAudioThreadSetConfigManager:
        complete = SetConfigManager(aMessage);
        break;        
    case ECMRAudioThreadSetPriority:
        complete = SetPriorityL(aMessage);
        break;
    case ECMRAudioThreadSetGain:
        complete = SetGainL(aMessage);
        break;
    case ECMRAudioThreadGetGain:
        complete = GetGainL(aMessage);
        break;
    case ECMRAudioThreadMaxGain:
        complete = MaxGainL(aMessage);
        break;
    case ECMRAudioThreadSetAudioCodec:
        iMessage = const_cast<RMessage2*>(&aMessage);        
        complete = SetAudioCodecL(aMessage);
        break;
    case ECMRAudioThreadPrime:
        complete = PrimeL();
        break;
    case ECMRAudioThreadPlay:
        complete = PlayL();
        break;
    case ECMRAudioThreadPause:
		iMessage = const_cast<RMessage2*>(&aMessage);
        complete = PauseL();
        break;
    case ECMRAudioThreadStop:
        complete = StopL();
        break;
    case ECMRAudioThreadWaitUntilStopped:
		iMessage = const_cast<RMessage2*>(&aMessage);
        complete = CheckIfStoppedL();
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
        PRINT((_L("CCMRAudioThreadProxySession::ServiceL() unknown msg")));
        User::Leave(KErrNotSupported);
        break;
        }

    if (complete)
        {
        aMessage.Complete(KErrNone);
        }

    PRINT((_L("CCMRAudioThreadProxySession::ServiceL() out")));
    }



// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::SetOutputL
// Set output object and threadid & create a timer for pause/stop
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::SetOutputL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRAudioThreadProxySession::SetOutputL() in")));


    if ( !iPauseTimer )
        {
        iPauseTimer = CCMRAudioPauseTimer::NewL(this);
        }

    iActiveOutput = reinterpret_cast<CCMRActiveOutput*>(aMessage.Int0());
    iOutputThreadId = static_cast<TUint>(aMessage.Int1());

    PRINT((_L("CCMRAudioThreadProxySession::SetOutputL() out")));
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::AddDataSourceL
// Add datasource
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::AddDataSourceL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRAudioThreadProxySession::AddDataSourceL() in")));
    iMMFAudioInput = reinterpret_cast<MDataSource*>(aMessage.Int0());

    PRINT((_L("CCMRAudioThreadProxySession::AddDataSourceL() logon to MMFAudioInput; loads DevSound")));
    iMMFAudioInput->SourceThreadLogon(*this);

    PRINT((_L("CCMRAudioThreadProxySession::AddDataSourceL() out")));
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::SetConfigManager
// Set config manager
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::SetConfigManager(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRAudioThreadProxySession::SetConfigManager() in")));
	iConfig = reinterpret_cast<CCMRConfigManager*>(aMessage.Int0());    
    PRINT((_L("CCMRAudioThreadProxySession::SetConfigManager() out")));
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::SetPriorityL
// Set audio priority
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::SetPriorityL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRAudioThreadProxySession::SetPriorityL()")));
    APASSERT( iMMFAudioInput );

    TMMFPrioritySettings* priority = reinterpret_cast<TMMFPrioritySettings*>(aMessage.Int0());
    // set priority settings
    iMMFAudioInput->SetSourcePrioritySettings(*priority);

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::SetGainL
// Set audio gain
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::SetGainL(const RMessage2& aMessage)
    {
    APASSERT( iMMFAudioInput );

    // Set gain of sound device
    MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFAudioInput);
    audioInput->SoundDevice().SetGain(aMessage.Int0());

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::GetGainL
// Get audio gain
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::GetGainL(const RMessage2& aMessage)
    {
    APASSERT( iMMFAudioInput );

    TInt* gain = reinterpret_cast<TInt*>(aMessage.Int0());
    MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFAudioInput);
    *gain = audioInput->SoundDevice().Gain();

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::MaxGainL
// Get max audio gain
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::MaxGainL(const RMessage2& aMessage)
    {
    APASSERT( iMMFAudioInput );

    TInt* gain = reinterpret_cast<TInt*>(aMessage.Int0());
    MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFAudioInput);
    *gain = audioInput->SoundDevice().MaxGain();

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::SetAudioCodecL
// Set audio codec
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::SetAudioCodecL(const RMessage2& aMessage)
    {
    PRINT((_L("CCMRAudioThreadProxySession::SetAudioCodecL() in")));

    iAudioCodec = reinterpret_cast<CCMRAudioCodecData *>(aMessage.Int0());

    APASSERT( iMMFAudioInput );

    iInitializingAudioCodec = ETrue;    
    
    if ( iAudioInput )
        {
        delete iAudioInput;
        iAudioInput = NULL;
        }

    if ( iAudioCodec->GetCodecSWHWTypeL() == ECodecTypeSW )
        {
        // SW codec in use; create our own source and give the MMFAudioInput source for it
        iAudioInput = CCMRSWAudioInput::NewL( iMMFAudioInput, iActiveOutput, iOutputThreadId, *this, iConfig );
        }
    else
        {
        // HW codec in use; create our own source and give the MMFAudioInput source for it
        iAudioInput = CCMRHWAudioInput::NewL( iMMFAudioInput, iActiveOutput, iOutputThreadId, *this, iConfig );
        }

    iAudioInput->SetCodecL( iAudioCodec );

    PRINT((_L("CCMRAudioThreadProxySession::SetAudioCodecL() out, iAudioCodec 0x%x, waiting InitializeComplete"), iAudioCodec));
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::PrimeL
// Prime thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::PrimeL()
    {
    PRINT((_L("CCMRAudioThreadProxySession::PrimeL() in")));
    APASSERT( iMMFAudioInput && iAudioInput && iAudioCodec );

    // config audio codec
    iAudioInput->ConfigureCodecL();

    iAudioInput->SourcePrimeL();
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::PlayL
// Start/continue playing (recording)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::PlayL()
    {
    PRINT((_L("CCMRAudioThreadProxySession::PlayL()")));
    APASSERT( iMMFAudioInput && iAudioInput && iAudioCodec );

    iAudioInput->SourceRecordL();
    iPaused = EFalse;
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::PauseL
// Pause audio thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::PauseL()
    {
    PRINT((_L("CCMRAudioThreadProxySession::PauseL()")));
    iPausingRecording = ETrue;
    iAudioInput->SourcePauseL();
    if ( iPausingRecording )    // could have been set to EFalse in a callback from SourcePauseL
        {
        // did not pause synchronously, set timer for pause; start by canceling possible previous timer
        iPauseTimer->Cancel();
        iPauseTimer->After(KCMRPauseTimeout); 
        }

    return EFalse;   

    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::StopL
// Stop audio thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::StopL()
    {
    PRINT((_L("CCMRAudioThreadProxySession::StopL()")));
    APASSERT( iMMFAudioInput && iAudioInput && iAudioCodec );

    iWaitingForStop = EFalse;
    if ( iPaused )
        {
        PRINT((_L("CCMRAudioThreadProxySession::StopL() already paused => also stopped")));
        // we are paused and hence also stopped => no callback coming => operation completed immediately
        iPaused = EFalse;

        // now we really stopped, not just paused, reset audio timestamp
        iAudioInput->ResetTimeStamp();

        return ETrue; // complete msg anyway
        }
    else
        {
        PRINT((_L("CCMRAudioThreadProxySession::StopL() starting with pause first")));
        iStoppingRecording = ETrue;
        iAudioInput->SourcePauseL();// stop will be called in the callback
        if ( iStoppingRecording ) // could have been set to EFalse in a callback from SourcePauseL
            {
            // did not stop synchronously, set timer for pause; start by canceling possible previous timer
            iPauseTimer->Cancel();
            iPauseTimer->After(KCMRPauseTimeout);
            }

        PRINT((_L("CCMRAudioThreadProxySession::StopL() out")));
        return ETrue; // complete msg anyway
        }
    }


// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::CheckIfStoppedL
// Check if we have stopped. If not, message is not completed and client keeps waiting. 
// Msg is in this case completed in SendEventToClient
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioThreadProxySession::CheckIfStoppedL()
    {
    if ( !iStoppingRecording )
        {
        PRINT((_L("CCMRAudioThreadProxySession::CheckIfStoppedL() stopped already")));
        return ETrue;
        }
    else
        {
        PRINT((_L("CCMRAudioThreadProxySession::CheckIfStoppedL() not stopped yet, start waiting")));
        iWaitingForStop = ETrue;
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxySession::SendEventToClient
// Catches completion events and sends other events to client using base class
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRAudioThreadProxySession::SendEventToClient(const TMMFEvent& aEvent)
    {
    if ( iInitializingAudioCodec )
        {
        iInitializingAudioCodec = EFalse;
        TInt errorToClient = aEvent.iErrorCode;
        iMessage->Complete( errorToClient );
        return KErrNone;        
        }
    else if ( iStoppingRecording )
        {
        // Normally we come here if aEvent.iEventType == KMMFEventCategoryPlaybackComplete, signaled by audioinput.
        // or if pausetimer timeouts
        // note that if aEvent.iEventType != KMMFEventCategoryPlaybackComplete we should stop still since it indicates problems

        // stopped
        iStoppingRecording = EFalse;

        // cancel the timer
        iPauseTimer->Cancel();

        TInt errorToClient = aEvent.iErrorCode;

        TRAPD(error,iAudioInput->SourceStopL());

        if ( (error != KErrNone) && (errorToClient == KErrNone) )
            {
            errorToClient = error;
            }
            // if errorToClient != KErrNone we are handling an error already

        // prime also just in case; to make sure that state is correct before new play
        TRAP(error,iAudioInput->SourcePrimeL());
        if ( (error != KErrNone) && (errorToClient == KErrNone) )
            {
            errorToClient = error;
            }
            // if errorToClient != KErrNone we are handling an error already
        
        // release the client from waiting
        if ( iWaitingForStop )
            {
            iMessage->Complete( errorToClient );
            iWaitingForStop = EFalse;
            }

        // now we really stopped, not just paused, reset audio timestamp
        iAudioInput->ResetTimeStamp();

        PRINT((_L("CCMRAudioThreadProxySession::SendEventToClient() stopping completed")));

        return KErrNone;
        }
    else if ( iPausingRecording )
        {
        // Normally we come here if aEvent.iEventType == KMMFEventCategoryPlaybackComplete, signaled by audioinput.
        // or if pausetimer timeouts
        // note that if aEvent.iEventType != KMMFEventCategoryPlaybackComplete we should stop still since it indicates problems

        // paused
        PRINT((_L("CCMRAudioThreadProxySession::SendEventToClient() pausing completed, informing client")));
        iPausingRecording = EFalse;
        iPaused = ETrue;

        // cancel the timer
        iPauseTimer->Cancel();

        TInt errorToClient = aEvent.iErrorCode;

        // stop & prime also just in case; to make sure that state is correct before resume
        TRAPD( error, iAudioInput->SourceStopL());
        if ( (error != KErrNone) && (errorToClient == KErrNone) )
            {
            errorToClient = error;
            }
            // if errorToClient != KErrNone we are handling an error already

        TRAP(error,iAudioInput->SourcePrimeL());
        if ( (error != KErrNone) && (errorToClient == KErrNone) )
            {
            errorToClient = error;
            }
            // if errorToClient != KErrNone we are handling an error already

        // release the client from waiting
        iMessage->Complete( errorToClient );
        return KErrNone;
        }
    else
        {
        if ( aEvent.iErrorCode != KErrNone )
            {
            // audioinput informed error & stopped operation, make sure also our CCMRAudioInput is stopped
            PRINT((_L("CCMRAudioThreadProxySession::SendEventToClient() error received, stop audioinput immediately before informing client, %d"),aEvent.iErrorCode));
            TRAPD(error, iAudioInput->SourceStopL());
            TRAP(error, iAudioInput->SourcePrimeL());
            // ignore error, we are already reporting an error here.

            // now we really stopped, not just paused, reset audio timestamp
            iAudioInput->ResetTimeStamp();
            }

        // call base class implementation
        return CCMRThreadProxySession::SendEventToClient( aEvent );
        }
    }




// destructor
CCMRAudioThreadProxySession::CCMRAudioPauseTimer::~CCMRAudioPauseTimer()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::CCMRAudioPauseTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRAudioThreadProxySession::CCMRAudioPauseTimer* CCMRAudioThreadProxySession::CCMRAudioPauseTimer::NewL(CCMRAudioThreadProxySession* aHost)
    {
    CCMRAudioPauseTimer* self = new(ELeave) CCMRAudioPauseTimer(aHost);
    CleanupStack::PushL(self);
    self->ConstructL ();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::CCMRAudioPauseTimer::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CCMRAudioThreadProxySession::CCMRAudioPauseTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::CCMRAudioPauseTimer::RunL
// Calls CCMRAudioThreadProxySession::SendEventToClient since audioinput has not called it in time
// -----------------------------------------------------------------------------
//
void CCMRAudioThreadProxySession::CCMRAudioPauseTimer::RunL()
    {
    PRINT((_L("CCMRAudioThreadProxySession::CCMRAudioPauseTimer::RunL() in")));
    TMMFEvent event(KMMFEventCategoryPlaybackComplete, KErrNone);
    iHost->SendEventToClient( event );
    PRINT((_L("CCMRAudioThreadProxySession::CCMRAudioPauseTimer::RunL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxySession::CCMRAudioPauseTimer::DoCancel
// Cancels the timer
// -----------------------------------------------------------------------------
//
void CCMRAudioThreadProxySession::CCMRAudioPauseTimer::DoCancel()
    {
    PRINT((_L("CCMRAudioThreadProxySession::CCMRAudioPauseTimer::DoCancel() in")));
    // cancel the timer
    CTimer::DoCancel();
    PRINT((_L("CCMRAudioThreadProxySession::CCMRAudioPauseTimer::DoCancel() out")));
    }
