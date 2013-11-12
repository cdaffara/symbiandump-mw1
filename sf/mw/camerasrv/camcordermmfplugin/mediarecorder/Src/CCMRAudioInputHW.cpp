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
* Description:  Audio input class to handle compressed audio input from codec
*                HW devices located under DevSound
*
*/


// INCLUDES
#include "CCMRAudioInput.h"
#include "CCMRAudioInputHW.h"
#include "CCMRActiveOutput.h"
#include "CCMRAudioCodecData.h"
#include "CCMRFifo.h"
#include <mmf/server/mmfaudioinput.h>

// CONSTANTS
// Initial number of buffers allocated
const TInt KCMRNumPassAudioBuffers = 2;     // # of buffers for passthrough mode

// MACROS
// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRAudioInputHWTraces.h"
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::NewL
// -----------------------------------------------------------------------------
//
CCMRAudioInput* CCMRHWAudioInput::NewL(MDataSource* aRealSource, CCMRActiveOutput* aOutput, TUint aThreadId, MAsyncEventHandler& aEventHandler, CCMRConfigManager* aConfig )
    {
    PRINT((_L("CCMRHWAudioInput::NewL() MMFAudioInput 0x%x"), aRealSource ));
    CCMRHWAudioInput* self = new(ELeave) CCMRHWAudioInput(aRealSource, aEventHandler);
    CleanupStack::PushL(self);
    self->ConstructL(aOutput, aThreadId, aConfig );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::~CCMRAudioInput
// -----------------------------------------------------------------------------
//
CCMRHWAudioInput::~CCMRHWAudioInput()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRHWAUDIOINPUT_CCMRHWAUDIOINPUT, "e_CCMRHWAudioInput::~CCMRHWAudioInput 1" );
    // allocated bufs can be either in iInputEmpty, iInputFilled, iOutputFilled, or iOutputEmptied
    CMMFDataBuffer* tmp;
    TInt i = 0;
    if ( iInputEmpty )
        {
        while ( !iInputEmpty->IsEmpty() )
            {
            tmp = reinterpret_cast<CMMFDataBuffer*>(iInputEmpty->Get());
            delete tmp;
            tmp = NULL;
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::~CCMRHWAudioInput() deleted %d bufs from iInputEmpty queue"), i ));
        }
    i = 0;
    if ( iInputFilled )
        {
        while ( !iInputFilled->IsEmpty() )
            {
            tmp = reinterpret_cast<CMMFDataBuffer*>(iInputFilled->Get());
            delete tmp;
            tmp = NULL;
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::~CCMRHWAudioInput() deleted %d bufs from iInputFilled queue"), i ));
        }
    i = 0;
    if ( iOutputFilled )
        {
        while ( !iOutputFilled->IsEmpty() )
            {
            tmp = reinterpret_cast<CMMFDataBuffer*>(iOutputFilled->Get());
            delete tmp;
            tmp = NULL;
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::~CCMRHWAudioInput() deleted %d bufs from iOutputFilled queue"), i ));
        }
    i = 0;
    if ( iOutputEmptied )
        {
        while ( !iOutputEmptied->IsEmpty() )
            {
            tmp = reinterpret_cast<CMMFDataBuffer*>(iOutputEmptied->Get());
            delete tmp;
            tmp = NULL;
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::~CCMRHWAudioInput() deleted %d bufs from iOutputEmptied queue"), i ));
        }
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRHWAUDIOINPUT_CCMRHWAUDIOINPUT, "e_CCMRHWAudioInput::~CCMRHWAudioInput 0" );
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::ConstructL
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::ConstructL(CCMRActiveOutput* aOutput, TUint aThreadId, CCMRConfigManager* aConfig )
    {
    PRINT((_L("CCMRHWAudioInput::ConstructL() in") ));

    CCMRAudioInput::ConstructL(aOutput, aThreadId, aConfig );
    CActiveScheduler::Add( this );

    PRINT((_L("CCMRHWAudioInput::ConstructL() out") ));
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::SourceRecordL
// Start recording
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::SourceRecordL()
    {
    CCMRAudioInput::SourceRecordL();

    FillBufferL( NULL );
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::SourceStopL
// Stops recording
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::SourceStopL()
    {
    PRINT((_L("CCMRHWAudioInput::SourceStopL() in")));
    // stop also sending our data; please remember that we are paused first also in stop => all data should have been flushed already when coming here
    Cancel();

    if ( iState != EStateStopped )
        {
        CCMRAudioInput::SourceStopL();

        PRINT((_L("CCMRHWAudioInput::SourceStopL() start moving buffers from full to empty queue")));
        // this is a real stop, no need to flush buffers to sink; that was done in pause
        TInt i = 0;

        // move all buffers from all fifos to iInputEmpty


        CMMFDataBuffer* tmp;
        while ( !iInputFilled->IsEmpty() )
            {
            // move bufs from filled to empty
            tmp = reinterpret_cast<CMMFDataBuffer*>(iInputFilled->Get());
            tmp->SetLastBuffer( EFalse );
            tmp->Data().SetLength(0);
            CleanupStack::PushL( tmp );
            iInputEmpty->PutL(reinterpret_cast<TAny*>(tmp) );
            CleanupStack::Pop( tmp );
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::SourceStopL() moved %d bufs from iInputFilled to iInputEmpty queue"), i ));

        // iOutput-fifos are being handled by output thread also, mutexes are needed

        // enter restricted area
        iMutexObj.Wait();
        TInt error = KErrNone;
        i = 0;
        while ( (!iOutputFilled->IsEmpty()) && (error == KErrNone) )
            {
            // move bufs from filled to empty
            tmp = reinterpret_cast<CMMFDataBuffer*>(iOutputFilled->Get());
            tmp->SetLastBuffer( EFalse );
            tmp->Data().SetLength(0);
            CleanupStack::PushL( tmp );
            TRAP(error, iInputEmpty->PutL(reinterpret_cast<TAny*>(tmp) ));
            CleanupStack::Pop( tmp );
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::SourceStopL() moved %d bufs from iOutputFilled to iInputEmpty queue"), i ));

        i = 0;
        while ( (!iOutputEmptied->IsEmpty()) && (error == KErrNone) )
            {
            // move bufs from filled to empty
            tmp = reinterpret_cast<CMMFDataBuffer*>(iOutputEmptied->Get());
            tmp->SetLastBuffer( EFalse );
            tmp->Data().SetLength(0);
            CleanupStack::PushL( tmp );
            TRAP(error,iInputEmpty->PutL(reinterpret_cast<TAny*>(tmp) ));
            CleanupStack::Pop( tmp );
            i++;
            }
        PRINT((_L("CCMRHWAudioInput::SourceStopL() moved %d bufs from iOutputEmptied to iInputEmpty queue"), i ));

        // move also iProcessingBuffer
        if ( (iProcessingBuffer) && ( error == KErrNone) )
            {
            tmp = iProcessingBuffer;
            iProcessingBuffer = NULL;
            tmp->SetLastBuffer( EFalse );
            tmp->Data().SetLength(0);
            TRAP(error,iInputEmpty->PutL(reinterpret_cast<TAny*>(tmp) ));
            PRINT((_L("CCMRHWAudioInput::SourceStopL() moved iProcessingBuffer to iInputEmpty queue")));
            }

        iNumFramesWaiting = 0;

        // leave restricted area
        iMutexObj.Signal();

        if ( error != KErrNone )
            {
            // leave postponed here to get out of mutex
            PRINT((_L("CCMRHWAudioInput::SourceStopL() leave with error code %d"), error));
            User::Leave( error );
            }
        }
    PRINT((_L("CCMRHWAudioInput::SourceStopL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::SetCodecL
// Set codec
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::SetCodecL( CCMRAudioCodecData* aCodecData )
    {
    PRINT((_L("CCMRHWAudioInput::SetCodecL() in")));

    CCMRAudioInput::SetCodecL( aCodecData );

    // passing the codec-FourCC tells DevSound to create the hardware device
	MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
	audioInput->SoundDevice().InitializeL(*this, aCodecData->GetCodecFourCCL(), EMMFStateRecording);
    PRINT((_L("CCMRHWAudioInput::SetCodecL() DevSound initializing - waiting for InitializeComplete.")));
    PRINT((_L("CCMRHWAudioInput::SetCodecL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::AllocateInputBuffersL
// Allocated input buffers.
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::AllocateInputBuffersL()
    {
    PRINT((_L("CCMRHWAudioInput::AllocateInputBuffersL() in")));
    // allocate buffers for coded data
    iInputBufferSize = iCodecData->MaxBufferLength();
    TInt i = 0;
    CMMFDataBuffer* tmp;
    TInt numBufs = KCMRNumPassAudioBuffers;
    while ( !iInputEmpty->IsEmpty() )
        {
        tmp = reinterpret_cast<CMMFDataBuffer*>(iInputEmpty->Get());
        delete tmp;
        tmp = NULL;
        i++;
        }
    PRINT((_L("CCMRHWAudioInput::AllocateInputBuffersL() deleted %d bufs from iOutputEmptied queue"), i ));

    for ( i = 0; i < numBufs; i++ )
        {
        tmp = CMMFDataBuffer::NewL(iInputBufferSize);
        CleanupStack::PushL( tmp );
        iInputEmpty->PutL( reinterpret_cast<TAny*>(tmp) );
        CleanupStack::Pop( tmp );
        }
    PRINT((_L("CCMRHWAudioInput::AllocateInputBuffersL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::BufferFilledL
// Catches the call from MDataSource when it has filled the buffer and copies
// the data to internal buffers and returns the buffer back to MDataSource so
// that it can continue filling it with new data
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::BufferFilledL(CMMFBuffer* aBuffer)
    {
    PRINT((_L("CCMRHWAudioInput::BufferFilledL() in") ));

    // copy the src buffer to internal buffers

    CMMFDataBuffer* dBuffer = static_cast<CMMFDataBuffer*>(aBuffer);

    // determine the number of bytes to copy,
    // abs max is the KCMRMaxSplittedAudioBufferSize unless the buffer
    // has less data
    TUint length = dBuffer->BufferSize();

    if ( length > 0 )
        {
        // take empty buffer
        CMMFDataBuffer* tmp = NULL;
        if ( iInputEmpty->IsEmpty() )
            {
            //out of buffers!! Create a new buffer
            PRINT((_L("CCMRHWAudioInput::BufferFilledL(), out of buffers, create a new") ));
            // determine the size
            if ( length > iInputBufferSize )
                {
                // we need to increase iInputBufferSize
                PRINT((_L("CCMRHWAudioInput::BufferFilledL(), increase iInputBufferSize to %d"), length ));
                iInputBufferSize = length;
                }

            TRAPD(err, (tmp = CMMFDataBuffer::NewL(iInputBufferSize)));
            PRINT((_L("CCMRHWAudioInput::BufferFilledL(), created a new") ));
            if ( err != KErrNone )
                {
                PRINT((_L("CCMRHWAudioInput::BufferFilledL(), allocation failed, error %d"), err));
                // allocation failed

                // send the given buffer back
                CCMRAudioInput::FillBufferL();

                // stop & send event to client
                TMMFEvent event( KMMFEventCategoryPlaybackComplete, err);
                SendEventToClient(event);
                return;
                }
            }
        else
            {
            // try to use an older buffer from the queue
            tmp = reinterpret_cast<CMMFDataBuffer*>(iInputEmpty->Get());
            if ( static_cast<TInt>(length) > tmp->Data().MaxLength() )
                {
                PRINT((_L("CCMRHWAudioInput::BufferFilledL(), too small buffer, create a new") ));
                iInputBufferSize = length;
                delete tmp;
                TRAPD(err, (tmp = CMMFDataBuffer::NewL(iInputBufferSize)));
                if ( err != KErrNone )
                    {
                    PRINT((_L("CCMRHWAudioInput::BufferFilledL(), allocation failed, error %d"), err));
                    // allocation failed

                    // send the given buffer back
                    CCMRAudioInput::FillBufferL();

                    // stop & send event to client
                    TMMFEvent event( KMMFEventCategoryPlaybackComplete, err);
                    SendEventToClient(event);
                    return;
                    }
                }
            }
        CleanupStack::PushL(tmp);

        TPtr8 tmpPtr(static_cast<TPtr8&>(tmp->Data()));

        // copy data
        tmpPtr.Copy( (dBuffer->Data().Ptr()), length );

        tmp->Data().SetLength( length );
        PRINT((_L("CCMRHWAudioInput::BufferFilledL() copied %d"), length ));

        iNumBytesStored += length;

        if ( dBuffer->LastBuffer() )
            {
            PRINT((_L("CCMRHWAudioInput::BufferFilledL(), last buffer set") ));
            tmp->SetLastBuffer( ETrue );
            iState = EStateStopping;
            }
        // copy the timestamp
        tmp->SetTimeToPlay( aBuffer->TimeToPlay() );

        // save filled buffer to iInputFilled
        iInputFilled->PutL( reinterpret_cast<TAny*>(tmp) );
        CleanupStack::Pop(tmp);

        if ( !IsActive() )
            {
            SetActive();
            TRequestStatus* tmpStatus = &iStatus;
            User::RequestComplete(tmpStatus, KErrNone);
            }
        }
    else
        {
        // empty buffer received
        if ( dBuffer->LastBuffer() )
            {
            PRINT((_L("CCMRHWAudioInput::BufferFilledL(), last buffer set") ));
            iState = EStateLastReceived;
            }
        }


#if (defined (__WINS__) || defined (__WINSCW__) )
    // save the buffer handle, to be returned to source when the first splitted buffer has been encoded
    iDevSoundBuffer = aBuffer;
#else
    // reset buf information
	aBuffer->SetFrameNumber(++iCurrentSourceFrameNumber); //so source knows which data to load buffer with
	aBuffer->SetStatus(EBeingFilled);
	aBuffer->SetLastBuffer(EFalse);
    // return the original buffer to source
	CCMRAudioInput::FillBufferL();
    //iMMFDataSource->FillBufferL( aBuffer, this, (TMediaId)KUidMediaTypeAudio );
#endif

    // update time by asking from DevSound
    UpdateTimeL();

    PRINT((_L("CCMRHWAudioInput::BufferFilledL() out") ));
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::FillBufferL
//
//
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::FillBufferL(CMMFBuffer* aBuffer)
    {
    PRINT((_L("CCMRHWAudioInput::FillBufferL() in") ));

    // this buffer is used through all the fifos in this class, save it to the iInputEmpty-fifo
    iUnderProcessing = EFalse;
    if ( aBuffer )
        {
        aBuffer->SetLastBuffer(EFalse);
        (static_cast<CMMFDataBuffer*>(aBuffer))->Data().SetLength(0);
        (static_cast<CMMFDataBuffer*>(aBuffer))->SetPosition(0);
        iInputEmpty->PutL( reinterpret_cast<TAny*>(aBuffer) );
        }

    if ( iInputFilled->IsEmpty() )
        {
        // no more data waiting to be processed, request new
        // we come here always also when doing the first recording request

#if (defined (__WINS__) || defined (__WINSCW__) )
        // In WINS source buffer is returned only here, to avoid too fast buffer exchange
        // which causes problems due to low performance

        if ( iState == EStateRecording )
            {
            if ( iFirstTime )
                {
                iFirstTime = EFalse;
                }
            else
                {
                // reset buf information
		        iDevSoundBuffer->SetFrameNumber(++iCurrentSourceFrameNumber); //so source knows which data to load buffer with
		        iDevSoundBuffer->SetStatus(EBeingFilled);
		        iDevSoundBuffer->SetLastBuffer(EFalse);
                }
            PRINT((_L("CCMRHWAudioInput::FillBufferL() request new data") ));
            CCMRAudioInput::FillBufferL();
            iDevSoundBuffer = NULL;
            }
#else
        // HW
        if ( iFirstTime )
            {
            // iDevSoundBuffer is NULL here, this is just used to prime devsound
            iFirstTime = EFalse;
            CCMRAudioInput::FillBufferL();
            // iMMFDataSource->FillBufferL( iDevSoundBuffer, this, (TMediaId)KUidMediaTypeAudio );
            PRINT((_L("CCMRHWAudioInput::FillBufferL() requested new data") ));
            iDevSoundBuffer = NULL;
            }
        else
            {
            // In HW source buffer was already returned
            PRINT((_L("CCMRHWAudioInput::FillBufferL() all buffered data processed, waiting for new") ));
            }
#endif
        }
    PRINT((_L("CCMRHWAudioInput::FillBufferL() out") ));
    }


// -----------------------------------------------------------------------------
// CCMRHWAudioInput::RunL
//
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::RunL()
    {
    PRINT((_L("CCMRHWAudioInput::RunL() in")));
    // move waiting buffers from iInputFilled to iOutputFilled. Handling of iOutputFilled is done in NewBufferL which takes care of mutexes too
    CMMFDataBuffer* buffer = reinterpret_cast<CMMFDataBuffer*>(iInputFilled->Get());
    while ( buffer )
        {
        iNumBytesStored -= buffer->BufferSize();
        if ( ( iState == EStateStopping ) && iInputFilled->IsEmpty() )
            {
            buffer->SetLastBuffer( ETrue );
            }
        // just in case, check that the buffer is not empty
        if ( buffer->BufferSize() > 0 )
            {
            iSendBufCount++;
            // inform sink
            NewBufferL(buffer);
            PRINT((_L("CCMRHWAudioInput::RunL(), sent a buffer to sink, data left in buffers %d"), iNumBytesStored ));
            }
        else
            {
            // buffer is empty, store it back to the queue of empty buffers
            FillBufferL(buffer);
            }
        buffer = reinterpret_cast<CMMFDataBuffer*>(iInputFilled->Get());
        }

    // move used buffers from iOutputEmptied to input. That fifo is used by 2 threads so we need mutexes
    // enter restricted area
    iMutexObj.Wait();

    TInt error = KErrNone;
    buffer = reinterpret_cast<CMMFDataBuffer*>(iOutputEmptied->Get());
    while ( buffer )
        {
        TRAP( error, FillBufferL(buffer));
        if ( error != KErrNone )
            {
            // must break and not leave since we are inside mutex
            break;
            }
        buffer = reinterpret_cast<CMMFDataBuffer*>(iOutputEmptied->Get());
        }

    // leave restricted area
    iMutexObj.Signal();

    if ( error != KErrNone )
        {
        // leave postponed here to get out of mutex
        PRINT((_L("CCMRHWAudioInput::RunL() leave with error code %d"), error));
        User::Leave( error );
        }

    PRINT((_L("CCMRHWAudioInput::RunL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::RunError
// Handle leave from RunL
// -----------------------------------------------------------------------------
//
TInt CCMRHWAudioInput::RunError(TInt aError)
    {
    PRINT((_L("CCMRHWAudioInput::RunError() with error code %d"), aError));
    TMMFEvent event (KMMFEventCategoryPlaybackComplete, aError);

    SendEventToClient( event );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCMRHWAudioInput::ConfigureCodecL
// Configure HW device.
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::ConfigureCodecL()
    {
    // HW codec
    // configure codec
//    ADASSERT( iCodecData );
    PRINT((_L("CCMRHWAudioInput::ConfigureCodecL() calls CCMRAudioAMRHWConfig()") ));
    TDesC8* configData = iCodecData->GetCodecConfigParamL(iMMFDataSource);
    User::LeaveIfError(iCodecData->AudioHWConfigL( iMMFDataSource, const_cast<TDesC8&>(*configData) ));
    PRINT((_L("CCMRHWAudioInput::ConfigureCodecL() call to CCMRAudioAMRHWConfig() succeed") ));
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::DoCancel
// Cancels the active object
// -----------------------------------------------------------------------------
//
void CCMRHWAudioInput::DoCancel()
    {
    }

// End of file


