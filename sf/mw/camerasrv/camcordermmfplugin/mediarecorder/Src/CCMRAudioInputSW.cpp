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
* Description:  Audio input class to handle uncompressed audio input from DevSound
*                and compress it using CMMFCodecs
*
*/


// INCLUDES
#include "CCMRAudioInput.h"
#include "CCMRAudioInputSW.h"
#include "CCMRActiveOutput.h"
#include "CCMRAudioCodecData.h"
#include "CCMRFifo.h"

#include <mmf/server/mmfcodec.h>
#include <mmf/server/mmfaudioinput.h>
#include <mmf/plugin/mmfcodecimplementationuids.hrh>

// CONSTANTS

// Initial number of buffers allocated
const TInt KCMRNumSplitAudioBuffers = 10;   // # of buffers for splitting mode

// Default timeout for the timer
#if (defined (__WINS__) || defined (__WINSCW__) )
const TUint KCMRWinsTimeOut = 100000;    // 100 ms; in WINS the performance is too slow to run this real-time, has to skip data
#endif


// MACROS

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ============================ MEMBER FUNCTIONS ===============================



// CCMRSWAudioInput


// -----------------------------------------------------------------------------
// CCMRSWAudioInput::NewL
// -----------------------------------------------------------------------------
//
CCMRAudioInput* CCMRSWAudioInput::NewL(MDataSource* aRealSource, CCMRActiveOutput* aOutput, TUint aThreadId, MAsyncEventHandler& aEventHandler, CCMRConfigManager* aConfig )
    {
    CCMRSWAudioInput* self = new(ELeave) CCMRSWAudioInput(aRealSource, aEventHandler);
    CleanupStack::PushL(self);
    self->ConstructL(aOutput, aThreadId, aConfig );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::~CCMRSWAudioInput
// -----------------------------------------------------------------------------
//
CCMRSWAudioInput::~CCMRSWAudioInput()
    {
    // cancel the timer
    Cancel();

    delete iCodec;
    iCodec = NULL;

    // there can be allocated bufs in all queueu: iInputEmpty, iInputFilled, iOutputFilled, and iOutputEmptied
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
        PRINT((_L("CCMRSWAudioInput::~CCMRSWAudioInput() deleted %d bufs from iInputEmpty queue"), i ));
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
        PRINT((_L("CCMRSWAudioInput::~CCMRSWAudioInput() deleted %d bufs from iInputFilled queue"), i ));
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
        PRINT((_L("CCMRSWAudioInput::~CCMRSWAudioInput() deleted %d bufs from iOutputFilled queue"), i ));
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
        PRINT((_L("CCMRSWAudioInput::~CCMRSWAudioInput() deleted %d bufs from iOutputEmptied queue"), i ));
        }

    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::ConstructL
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::ConstructL(CCMRActiveOutput* aOutput, TUint aThreadId, CCMRConfigManager* aConfig )
    {
    PRINT((_L("CCMRSWAudioInput::ConstructL() in") ));
    // construct the timer
    CTimer::ConstructL();
    CCMRAudioInput::ConstructL(aOutput, aThreadId, aConfig );
    CActiveScheduler::Add( this );

    PRINT((_L("CCMRSWAudioInput::ConstructL() out") ));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SetCodecL
// Set codec
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::SetCodecL( CCMRAudioCodecData* aCodecData )
    {
    PRINT((_L("CCMRSWAudioInput::SetCodecL() in")));
    CCMRAudioInput::SetCodecL( aCodecData );
    iBufferDuration = iCodecData->GetFrameDurationUs();

    iDeleteOld = EFalse;
    if ( iCodec )
        {
		delete iCodec;
		iCodec = NULL;
        iDeleteOld = ETrue;
		}
    // instantiate the codec with default Uid. For security reasons, only tested codecs are supported and hence Uid is hardcoded
    PRINT((_L("CCMRSWAudioInput::SetCodecL() create CMMFCodec with Uid %d"), iCodecData->SWCodecUid().iUid ));
    iCodec = CMMFCodec::NewL ( iCodecData->SWCodecUid() );

    PRINT((_L("CCMRSWAudioInput::SetCodecL() codec created, initialize & configure DevSound")));
    // passing the srcFourCC tells DevSound NOT to create the hardware device
	MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    // PCM16 is the default input
    TFourCC srcFourCC = KMMFFourCCCodePCM16;
	audioInput->SoundDevice().InitializeL(*this, srcFourCC, EMMFStateRecording);
    PRINT((_L("CCMRSWAudioInput::SetCodecL() Devsound Initializing - waiting for InitializeComplete.")));
    PRINT((_L("CCMRSWAudioInput::SetCodecL() out"))); 
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::AllocateInputBuffersL
// Allocated input buffers.
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::AllocateInputBuffersL()
    {
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() in")));
    // allocate input buffers for PCM
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() PCM Channels: %d"), iCodecData->GetNumChannels() ));
    iInputBufferSize = 2*iCodecData->GetNumChannels()*iCodecData->PreferredSampleCountPerInputBuffer();   
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() PCM input buffer size: %d"), iInputBufferSize ));

    // 2 comes from PCM16 => 2 bytes per sample
    // If sample rate is 8 kHz => the multiplier is 2*8=16; for 16 kHz the multiplier is 2*16; 
    // E.g. a 8kHz codec with 20 ms capture this gives 320 bytes

    // this is in practice the size of PCM buffer for one frame. It can be exactly the same as iInputBufferSize. Typecast due to signed/unsigned mismatch
    iMinPCMBufferSize = TInt(iInputBufferSize) / iCodecData->PreferredFrameCountPerInputBuffer();
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() PCM min buffer size: %d"), iMinPCMBufferSize ));

    TInt numBufs = KCMRNumSplitAudioBuffers;
    TInt i;
    CMMFDataBuffer* tmp;
    for ( i = 0; i < numBufs; i++ )
        {
        tmp = CMMFDataBuffer::NewL(iInputBufferSize);
        CleanupStack::PushL( tmp );
        iInputEmpty->PutL( reinterpret_cast<TAny*>(tmp) );
        CleanupStack::Pop( tmp );
        }

    // allocate buffers for coded data
    iCodedBufferLength = iCodecData->PreferredFrameCountPerInputBuffer() * iCodecData->MaxFrameLengthL();
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() PCM iCodedBufferLength: %d"), iCodedBufferLength ));    
    numBufs = KCMRNumSplitAudioBuffers;
    if ( iDeleteOld )
        {
        iDeleteOld = EFalse;
        while ( !iOutputEmptied->IsEmpty() )
            {
            tmp = reinterpret_cast<CMMFDataBuffer*>(iOutputEmptied->Get());
            delete tmp;
            tmp = NULL;
            i++;
            }
        PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() deleted %d bufs from iOutputEmptied queue"), i ));
        }
    for ( i = 0; i < numBufs; i++ )
        {
        tmp = CMMFDataBuffer::NewL(iCodedBufferLength);
        CleanupStack::PushL( tmp );
        iOutputEmptied->PutL( reinterpret_cast<TAny*>(tmp) );
        CleanupStack::Pop( tmp );
        }
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() created %d bufs for iOutputEmptied queue"), i )); 
    PRINT((_L("CCMRSWAudioInput::AllocateInputBuffersL() out")));    
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SourcePrimeL
// Primes the source
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::SourcePrimeL()
    {
    PRINT((_L("CCMRSWAudioInput::SourcePrimeL()")));
    CCMRAudioInput::SourcePrimeL();
    iStartTime = TInt64(0);
    iRemainder = 0;
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::BufferFilledL
// Catches the call from MDataSource when it has filled the buffer and if 
// buffering is needed, copies the buffer possibly to many smaller buffers
// and saves them to be encoded one at a time in timed intervals
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::BufferFilledL(CMMFBuffer* aBuffer)
    {
    PRINT((_L("CCMRSWAudioInput::BufferFilledL() in") ));

    // take the start time, used as a basis for the timer for splitted buffers
    if ( iSendBufCount == 0 )
        {
        iStartTime.HomeTime();
        }

    // copy the src buffer to internal buffers, splitting it to many if it is large

    CMMFDataBuffer* dBuffer = static_cast<CMMFDataBuffer*>(aBuffer);
    PRINT((_L("CCMRSWAudioInput::BufferFilledL() Data.length %d"), dBuffer->Data().Length() ));

    // determine the number of bytes to copy, 
    // abs max is the KCMRMaxSplittedAudioBufferSize unless the buffer
    // has less data
    TUint length = dBuffer->BufferSize();
    TUint copyLength = iInputBufferSize;
    if ( (iSendBufCount == 0) && (length < iInputBufferSize) )
        {
        // looks like DevSound outputs data in shorter buffers than we expect, must adjust timeout etc. 
        // this is done only in the 1st round
        iBufferDuration = length / (2*iCodecData->GetSampleRate()/1000); // 2 comes from PCM 16 (2 bytes per sample), the rest depends on sampling rate. No stereo support currently
        iInputBufferSize = length;
        copyLength = length;    // adjust this one here also to skip the following checks
        PRINT((_L("CCMRSWAudioInput::BufferFilledL() smaller input buffers, copyLength set to %d"), copyLength));
        }

    if ( iRemainder > 0 )
        {
        // the last input buffer didn't fill the last splitted buffer completely. To stay in sync with timer, we need to copy now more data
        copyLength += iRemainder;
        PRINT((_L("CCMRSWAudioInput::BufferFilledL() copyLength %d increased to include iRemainder %d"), copyLength, iRemainder));
        }
    if ( copyLength > length )
        {
        // there was less data in input than should be in output
        if ((iRemainder > 0 ) || (length < iInputBufferSize))
            {
            // more data in iRemainder
            iRemainder = copyLength - length;
            }

        copyLength = length;
        PRINT((_L("CCMRSWAudioInput::BufferFilledL() copyLength was larger than input length, reduced to %d"), copyLength));
        }

    // check if we have any buffers now in queue or in output
    TBool notOldBuffers = EFalse;
    if ( (iUnderProcessing == EFalse) && (iInputFilled->IsEmpty()) )
        {
        // no bufs neither in output nor in filled queue, can take the newest one into use right away
        PRINT((_L("CCMRSWAudioInput::BufferFilledL() no old buffers, taken into use immediately after copy") ));
        notOldBuffers = ETrue;
        }

    TInt readIndex = 0;

    if ( length > 0 )
        {
        while ( length > 0 )
            {
            // take empty buffer
            CMMFDataBuffer* tmp = NULL;
            if ( iInputEmpty->IsEmpty() )
                {
                //out of buffers!! Create a new buffer
                PRINT((_L("CCMRSWAudioInput::BufferFilledL(), out of buffers, create a new") ));
                // determine the size
                if ( copyLength > iInputBufferSize )
                    {
                    // we need to increase iInputBufferSize
                    PRINT((_L("CCMRSWAudioInput::BufferFilledL(), increase iInputBufferSize to %d"), copyLength ));
                    iInputBufferSize = copyLength;
                    if ( (iInputBufferSize % iMinPCMBufferSize) != 0 )
                        {
                        // align the buffer size with smallest buffer size accepted by AMR, since AMR encoder increases the size by this number and may cause buffer overflow otherwise
                        iInputBufferSize += iMinPCMBufferSize - (iInputBufferSize % iMinPCMBufferSize);
                        }
                    }

                TRAPD(err, (tmp = CMMFDataBuffer::NewL(iInputBufferSize)));
                PRINT((_L("CCMRSWAudioInput::BufferFilledL(), created a new") ));
                if ( err != KErrNone )
                    {
                    PRINT((_L("CCMRSWAudioInput::BufferFilledL(), allocation failed, error %d"), err));
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
                if ( static_cast<TInt>(copyLength) > tmp->Data().MaxLength() )
                    {
                    PRINT((_L("CCMRSWAudioInput::BufferFilledL(), too small buffer, create a new") ));
                    iInputBufferSize = copyLength;
                    if ( (iInputBufferSize % iMinPCMBufferSize) != 0 )
                        {
                        // align the buffer size with smallest buffer size accepted by AMR, since AMR encoder increases the size by this number and may cause buffer overflow otherwise
                        iInputBufferSize += iMinPCMBufferSize - (iInputBufferSize % iMinPCMBufferSize);
                        }
                    delete tmp;
                    TRAPD(err, (tmp = CMMFDataBuffer::NewL(iInputBufferSize)));
                    if ( err != KErrNone )
                        {
                        PRINT((_L("CCMRSWAudioInput::BufferFilledL(), allocation failed, error %d"), err));
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
            tmpPtr.Copy( (dBuffer->Data().Ptr()+readIndex), copyLength );

            tmp->Data().SetLength( copyLength );
            length -= copyLength;
            readIndex += copyLength;
            PRINT((_L("CCMRSWAudioInput::BufferFilledL() copied %d"), copyLength ));

            iNumBytesStored += copyLength;

            if ( dBuffer->LastBuffer() && (length == 0) )
                {
                PRINT((_L("CCMRSWAudioInput::BufferFilledL(), last buffer set") ));
                tmp->SetLastBuffer( ETrue );
                iState = EStateStopping;
                }

            // save filled buffer
            iInputFilled->PutL( reinterpret_cast<TAny*>(tmp) );
            CleanupStack::Pop(tmp);

            if ( (iRemainder > 0) && (length > 0) )
                {
                // there was some remainder from the previous buffer, and there are still data in the input buffer
                // if length == 0, iRemainder was set in the previous round for this buffer, and value should not be changed
                // if input had less data than required, iRemainder was set in the beginning and it forces length to be 0 at this point
                // we are in sync again
                // go back to the normal length
                iRemainder = 0;
                copyLength = iInputBufferSize;
                }

            if ( (length < copyLength) && (length > 0) )
                {
                // last buf will get less data, mark down how much less. 
                // if iRemainder >> input length, this limits it since copyLength is reduced above. 
                // It may be even better to have such limit to avoid too large buffers; such case most likely indicates a problem
                iRemainder = copyLength - length;
                PRINT((_L("CCMRSWAudioInput::BufferFilledL(), iRemainder set to %d"), iRemainder ));

                copyLength = length;
                }

#if (defined (__WINS__) || defined (__WINSCW__) )
            //skip the rest; in WINS the performance is too slow to run this real-time, has to skip data
            length = 0;
#endif
            } // end of while-loop

        PRINT((_L("CCMRSWAudioInput::BufferFilledL(), data processed.")));

        if ( notOldBuffers )
            {
#if (defined (__WINS__) || defined (__WINSCW__) )
            // in WINS the performance is too slow to run this real-time, has to wait longer
            Cancel();
            CTimer::After( KCMRWinsTimeOut );
            PRINT((_L("CCMRSWAudioInput::BufferFilledL(), timer set") ));
#else
            // we didn't have any old buffers queued or in output, send the 1st new one right away
            CTimer::After( 0 );
#endif
            }

        }
    else
        {
        // empty buffer received
        if ( dBuffer->LastBuffer() )
            {
            PRINT((_L("CCMRSWAudioInput::BufferFilledL(), last buffer set") ));
            iState = EStateLastReceived;
            }
        }


#if (defined (__WINS__) || defined (__WINSCW__) )
    // save the buffer handle, to be returned to source when the first splitted buffer has been encoded
    iDevSoundBuffer = aBuffer;
#else
    // reset buf information
    PRINT((_L("CCMRSWAudioInput::BufferFilledL(), resetting data buffer.")));    
	aBuffer->SetFrameNumber(++iCurrentSourceFrameNumber); //so source knows which data to load buffer with
	aBuffer->SetStatus(EBeingFilled);
	aBuffer->SetLastBuffer(EFalse);
    // return the original buffer to source
    PRINT((_L("CCMRSWAudioInput::BufferFilledL(), calling CCMRAudioInput::FillBufferL()")));	
	CCMRAudioInput::FillBufferL();
#endif

    // update time by asking from DevSound
    UpdateTimeL();
    PRINT((_L("CCMRSWAudioInput::BufferFilledL() out") ));
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::FillBufferL
// Returns buffer from RunL to iInputEmpty and depending on state & mode, either starts 
// a new timer or requests the source to fill the buffer
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::FillBufferL(CMMFBuffer* aBuffer)
    {
    PRINT((_L("CCMRSWAudioInput::FillBufferL() in") ));

    // this buffer is used between output and this class, save it to fifo
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
        // no more data to process, request new
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
            else if ( iDevSoundBuffer )
                {
                // reset buf information
		        iDevSoundBuffer->SetFrameNumber(++iCurrentSourceFrameNumber); //so source knows which data to load buffer with
		        iDevSoundBuffer->SetStatus(EBeingFilled);
		        iDevSoundBuffer->SetLastBuffer(EFalse);
                }
            PRINT((_L("CCMRSWAudioInput::FillBufferL() request new data") ));
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
            PRINT((_L("CCMRSWAudioInput::FillBufferL() requested new data") ));
            iDevSoundBuffer = NULL;
            }
        else
            {
            // In HW source buffer was already returned
            PRINT((_L("CCMRSWAudioInput::FillBufferL() all buffered data encoded, waiting for new") ));
            }
#endif
        }
    else
        {
        // continue feeding new data after a timeout
        // more data in iInputFilled than supposed to be => we are late, no time to sleep
        CTimer::After( 0 );
        PRINT((_L("CCMRSWAudioInput::FillBufferL(), timer with timeout 0 set") ));
        }
    PRINT((_L("CCMRSWAudioInput::FillBufferL() out") ));
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::SourceRecordL
// Start recording
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::SourceRecordL()
    {
    CCMRAudioInput::SourceRecordL();

    FillBufferL( NULL );
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::SourceStopL
// Stops playing (recording)
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::SourceStopL()
    {
    PRINT((_L("CCMRSWAudioInput::SourceStopL() in")));
    // stop also sending our data; please remember that we pause first also in stop => all data should have been flushed already when coming here
    Cancel();

    if ( iState != EStateStopped )
        {
        CCMRAudioInput::SourceStopL();

        PRINT((_L("CCMRSWAudioInput::SourceStopL() start moving buffers from full to empty queue")));
        // this is a real stop, no need to flush buffers to sink; that was done in pause
        // move all buffers to empty-fifos: from iInputFilled to iInputEmpty and from iOutputFilled to iOutputEmpty
        TInt i = 0;
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
        PRINT((_L("CCMRSWAudioInput::SourceStopL() moved %d bufs from iInputFilled to iInputEmpty queue"), i ));


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
            TRAP( error, iOutputEmptied->PutL(reinterpret_cast<TAny*>(tmp) ));
            CleanupStack::Pop( tmp );
            i++;
            }
        PRINT((_L("CCMRSWAudioInput::SourceStopL() moved %d bufs from iOutputFilled to iOutputEmptied queue"), i ));

        // move also iProcessingBuffer
        if ( (iProcessingBuffer) && ( error == KErrNone) )
            {
            tmp = iProcessingBuffer;
            iProcessingBuffer = NULL;
            tmp->SetLastBuffer( EFalse );
            tmp->Data().SetLength(0);
            TRAP(error,iOutputEmptied->PutL(reinterpret_cast<TAny*>(tmp) ));
            PRINT((_L("CCMRSWAudioInput::SourceStopL() moved iProcessingBuffer to iOutputEmptied queue")));
            }

        iNumFramesWaiting = 0;
        // leave restricted area
        iMutexObj.Signal();

        if ( error != KErrNone )
            {
            // leave postponed here to get out of mutex
            PRINT((_L("CCMRSWAudioInput::SourceStopL() leave with error code %d"), error));
            User::Leave( error );
            }

        }
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::RunL
// Encodes a buffer and informs sink about it
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::RunL()
    {
    // iStatus may be KErrUnderflow but it doesn't matter, timer was just set on too late, and we can't do anything about it
    PRINT((_L("CCMRSWAudioInput::RunL() with timer in iStatus = %d"), iStatus.Int() ));


    // take oldest from fifo, cast it to correct type, and encode it
    CMMFDataBuffer* pcmBuffer = reinterpret_cast<CMMFDataBuffer*>(iInputFilled->Get());
    if ( pcmBuffer )
        {
        CleanupStack::PushL( pcmBuffer );

        iNumBytesStored -= pcmBuffer->BufferSize();
        if ( ( iState == EStateStopping ) && iInputFilled->IsEmpty() )
            {
            pcmBuffer->SetLastBuffer( ETrue );
            }
        iUnderProcessing = ETrue;

        // loop until all data from the pcmBuffer is consumed
        while ( !EncodeBufferL( pcmBuffer ) )
            {
            }

		
        // Give PCM16 input buffer back
        FillBufferL(pcmBuffer);
        // pcmBuffer was saved into fifo, can be popped out now (but not destroy!)
        CleanupStack::Pop( pcmBuffer );

        PRINT((_L("CCMRSWAudioInput::RunL(), encoded buffer, data left in buffers %d"), iNumBytesStored ));
        }

    // Else the timer was useless; should not be possible but is not a fatal error case either. 
    //When new data arrives, it will set a new timer
    }


// -----------------------------------------------------------------------------
// CCMRSWAudioInput::RunError
// Handle leave from RunL
// -----------------------------------------------------------------------------
//
TInt CCMRSWAudioInput::RunError(TInt aError)
    {
    PRINT((_L("CCMRSWAudioInput::RunError() with error code %d"), aError));
    TMMFEvent event (KMMFEventCategoryPlaybackComplete, aError);

    SendEventToClient( event );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCMRSWAudioInput::EncodeBufferL
// Encode the given PCM buffer, using EncodeL. This generates 0 or 1 output 
// buffer and may need to be called again to completely consume the input.
// -----------------------------------------------------------------------------
//
TBool CCMRSWAudioInput::EncodeBufferL(CMMFDataBuffer* aInBuffer)
    {
    PRINT((_L("CCMRSWAudioInput::EncodeBufferL() in")));
    // take buffer for encoding
    // enter restricted area
    iMutexObj.Wait();
    CMMFDataBuffer* codedBuffer = reinterpret_cast<CMMFDataBuffer*>(iOutputEmptied->Get());
    // leave restricted area
    iMutexObj.Signal();    
    if ( codedBuffer == NULL )
        {
        // must create a new one - better to store data in compressed buffers than PCM16 buffers
        codedBuffer = CMMFDataBuffer::NewL(iCodedBufferLength);
        }

    CleanupStack::PushL( codedBuffer );

    TBool completed = ETrue;

    // encode and check the result
    switch ( EncodeL(aInBuffer, codedBuffer) )
        {
        case TCodecProcessResult::EProcessIncomplete:
            // Not all data from input was consumed (EncodeL updated buffer members), but output was generated
            PRINT((_L("CCMRSWAudioInput::EncodeBufferL() not all data consumed from input")));

            completed = EFalse;
			// purposely fall through

        case TCodecProcessResult::EProcessComplete:
            // all data from input was used and output was generated


            // check the length, just in case
            if ( codedBuffer->BufferSize() > 0 )
                {
                PRINT((_L("CCMRSWAudioInput::EncodeBufferL() generated output")));
                iSendBufCount++;
                // inform sink
                NewBufferL(codedBuffer);
                }
            else
                {
                PRINT((_L("CCMRSWAudioInput::EncodeBufferL() codec returned an empty buffer with EProcessComplete")));
    			// enter restricted area
    			iMutexObj.Wait();                
                iOutputEmptied->PutL( reinterpret_cast<TAny*>(codedBuffer) );
    			// leave restricted area
    			iMutexObj.Signal();                 
                }
            break;

        case TCodecProcessResult::EDstNotFilled:
            // need more input data, can't fill the output yet; put it back to the empty queue
            PRINT((_L("CCMRSWAudioInput::EncodeBufferL() not enough input data to generate output")));
    		// enter restricted area
    		iMutexObj.Wait();               
            iOutputEmptied->PutL( reinterpret_cast<TAny*>(codedBuffer) );
    		// leave restricted area
    		iMutexObj.Signal();               
            break;

        default:
            // EEndOfData, EProcessError, EProcessIncompleteRepositionRequest, EProcessCompleteRepositionRequest
            PRINT((_L("CCMRSWAudioInput::EncodeBufferL() unknown status from CMMFCodec")));
            User::Leave( KErrUnknown );
        }

    // codedBuffer was saved into fifo (here or in NewBufferL), can be popped out now (but not destroy!)
    CleanupStack::Pop( codedBuffer );

    PRINT((_L("CCMRSWAudioInput::EncodeBufferL() out")));
    return completed;
    }

// -----------------------------------------------------------------------------
// CCMRSWAudioInput::EncodeL
// Encodes a buffer using CMMFCodec
// -----------------------------------------------------------------------------
//
TCodecProcessResult::TCodecProcessResultStatus CCMRSWAudioInput::EncodeL(CMMFDataBuffer* aInBuffer, CMMFDataBuffer* aOutBuffer)
    {
    PRINT((_L("CCMRSWAudioInput::EncodeL() in")));
    TCodecProcessResult result;

    result = iCodec->ProcessL (*aInBuffer, *aOutBuffer);

	switch (result.iStatus)
		{
		case TCodecProcessResult::EProcessComplete:
			// finished processing source data, all data in sink buffer
            PRINT((_L("CCMRSWAudioInput::EncodeL() EProcessComplete")));
			aOutBuffer->SetPosition(0);
            if ( aInBuffer->LastBuffer() )
                {
                aOutBuffer->SetLastBuffer(ETrue);
                }
			break;

		case TCodecProcessResult::EDstNotFilled:
			// the destination is not full, we need more data. Handled in caller
            PRINT((_L("CCMRSWAudioInput::EncodeL() EDstNotFilled")));
            break;

        case TCodecProcessResult::EProcessIncomplete:
			// the sink was filled before all the source was processed
            PRINT((_L("CCMRSWAudioInput::EncodeL() EProcessIncomplete, bytes processed: %d"), result.iSrcBytesProcessed));
			aOutBuffer->SetPosition(0);
            aInBuffer->SetPosition( aInBuffer->Position() + result.iSrcBytesProcessed);
			break;

		default:
			break;
		}

    PRINT((_L("CCMRSWAudioInput::EncodeL() out")));
    return result.iStatus;
    }



// -----------------------------------------------------------------------------
// CCMRHWAudioInput::ConfigureCodecL
// Configure the used CMMFCodec.
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::ConfigureCodecL()
    {
    PRINT((_L("CCMRSWAudioInput::ConfigureCodecL() in") ));
    TDesC8* configData = iCodecData->GetCodecConfigParamL(iMMFDataSource);
    if ( configData )
        {
        TUid uid ={KUidMmfCodecAudioSettings}; // Use Uid reserved for codec configurations
        TRAPD(err,iCodec->ConfigureL( uid, const_cast<TDesC8&>(*configData)));
        if ( err ) {}
        // some codecs may not support, ignore error and hope it is configured properly
        }
    PRINT((_L("CCMRSWAudioInput::ConfigureCodecL() out") ));
    }



// -----------------------------------------------------------------------------
// CCMRSWAudioInput::DoCancel
// Cancels the timer
// -----------------------------------------------------------------------------
//
void CCMRSWAudioInput::DoCancel()
    {
    PRINT((_L("CCMRSWAudioInput::DoCancel() timer in")));
    // cancel the timer
    CTimer::DoCancel();
    PRINT((_L("CCMRSWAudioInput::DoCancel() timer out")));
    }






// End of file


