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
* Description:  Audio input class to handle uncompressed audio input from DevSound
*                and compress it using CMMFCodecs
*
*/

#ifndef CCMRAUDIOINPUTSW_H
#define CCMRAUDIOINPUTSW_H

// INCLUDES
#include "CCMRAudioInput.h"
#include <mmf/server/mmfcodec.h>

// FORWARD DECLARATIONS
class CCMRFifo;



// CLASS DECLARATIONS


/**
*  Audio input class to be used with CMMFCodec audio codecs
*/
class CCMRSWAudioInput : public CCMRAudioInput
    {


    public:  // Constructor and destructor
        
        /**
        * Two-phased constructor.
        * @param MDataSource *aMMFAudioInput
        * @param TMediaId aMediaId 
        * @return CCMRAudioInput* Pointer to constructed audioinput
        */
        static CCMRAudioInput* NewL(MDataSource *aMMFAudioInput, CCMRActiveOutput* aOutput, TUint aThreadId, MAsyncEventHandler& aEventHandler, CCMRConfigManager* aConfig);
        
        /**
        * Destructor.
        */
        virtual ~CCMRSWAudioInput();

    public: // from base classes

        /**
        * From CCMRAudioInput, informs that the given buffer is filled, called by MMFAudioInput
        */
        void BufferFilledL(CMMFBuffer* aBuffer);

        /**
        * From CCMRAudioInput, requests to fill the buffer
        */
        void FillBufferL(CMMFBuffer* aBuffer);

        /**
        * From CCMRAudioInput, stops recording
        */
        void SourceStopL();
        /**
        * From CCMRAudioInput, starts recording
        */
        void SourceRecordL();

        /**
        * From CCMRAudioInput, primes source
        */
        void SourcePrimeL();

        /**
        * From CCMRAudioInput, set codecdata-object
        */
        void SetCodecL( CCMRAudioCodecData* aCodecData );

        /**
        * From CCMRAudioInput, configures the codec
        */
        void ConfigureCodecL();

    protected: // new functions

        /**
        *  Encode the given PCM buffer, using EncodeL. This generates 0 or 1 output buffer and may need to be called 
        *  again to completely consume the input
        *  @return TBool ETrue if completed with the input, EFalse if must be called again
        */
        virtual TBool EncodeBufferL(CMMFDataBuffer* aInBuffer);

        /**
        *  Encode the given buffer from PCM to selected format, using CMMFCodec
        *  @return TCodecProcessResult::TCodecProcessResultStatus
        */
        virtual TCodecProcessResult::TCodecProcessResultStatus EncodeL(CMMFDataBuffer* aInBuffer, CMMFDataBuffer* aOutBuffer);

    protected: // from base classes

        /**
        * From CActive/CTimer, encodes buffer to wait for retrieval from output active object
        */
        void RunL();

        /**
        * From CActive, handles errors in RunL
        */
        TInt RunError(TInt aError);

        /**
        * From CActive, cancels the AO
        */
        void DoCancel();


    private:

        /**
        * C++ default constructor
        * @param MDataSource* aDataSource
        * @param TMediaId aMediaId
        */
        CCMRSWAudioInput(MDataSource* aDataSource, MAsyncEventHandler& aEventHandler) : 
            CCMRAudioInput(aDataSource, aEventHandler) {};

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(CCMRActiveOutput* aOutput, TUint aThreadId, CCMRConfigManager* aConfig);

        /**
        * Handles allocation of input buffers after audioinput has completed initialization.
        */                
        void AllocateInputBuffersL();

    private: // data

        // Whether or not AllocateInputBuffersL needs to clear buffers (happens if audiocodec is reset).
        TBool iDeleteOld;

        // if the last input buffer didn't fill the last splitted buffer completely, we need to copy more data next time 
        // to stay in sync with timer. This tells how much data
        TUint iRemainder;
        
        // Time when we got our first buffer from MMF audio input => sending of next ones is based on this
        TTime iStartTime;

        // Duration of output buffer in time => timeout. By default it is KCMRMaxAudioBufferTime*1000
        TUint iBufferDuration;

        // Encoder used to encode from PCM to given format
        CMMFCodec* iCodec;

        // Max length of coded buffer, specific to each codec
        TInt iCodedBufferLength;

        // Minimum size for PCM buffer
        TUint iMinPCMBufferSize;
    };

#endif      
            
// End of File



