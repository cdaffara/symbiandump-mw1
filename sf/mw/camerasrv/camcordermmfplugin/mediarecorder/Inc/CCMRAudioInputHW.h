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

#ifndef CCMRAUDIOINPUTHW_H
#define CCMRAUDIOINPUTHW_H

// INCLUDES
#include "CCMRAudioInput.h"

// FORWARD DECLARATIONS
class CCMRFifo;


// CLASS DECLARATIONS


/**
*  Audio input class to be used with HW device audio codecs
*/
class CCMRHWAudioInput : public CCMRAudioInput
    {

    public:  // Constructor and destructor
        
        /**
        * Two-phased constructor.
        * @param MDataSource *aMMFAudioInput
        * @param TMediaId aMediaId 
        * @return CCMRAudioInput* Pointer to constructed audioinput
        */
        static CCMRAudioInput* NewL(MDataSource *aMMFAudioInput, CCMRActiveOutput* aOutput, TUint aThreadId, MAsyncEventHandler& aEventHandler, CCMRConfigManager* aConfig );
        
        /**
        * Destructor.
        */
        virtual ~CCMRHWAudioInput();

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
        * From CCMRAudioInput, set codecdata-object
        */
        void SetCodecL( CCMRAudioCodecData* aCodecData );

        /**
        * From CCMRAudioInput, configures the codec
        */
        void ConfigureCodecL();

    protected: // from base classes

        /**
        * From CActive, moves buffer to wait for retrieval from output active object
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


    private: // constructors

        /**
        * C++ default constructor
        * @param MDataSource* aDataSource
        * @param TMediaId aMediaId
        */
        CCMRHWAudioInput(MDataSource* aDataSource, MAsyncEventHandler& aEventHandler) : 
            CCMRAudioInput(aDataSource, aEventHandler) {};

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(CCMRActiveOutput* aOutput, TUint aThreadId, CCMRConfigManager* aConfig);

        /**
        * Handles allocation of input buffers after audioinput has completed initialization.
        */        
        void AllocateInputBuffersL();

    };

#endif      
            
// End of File



