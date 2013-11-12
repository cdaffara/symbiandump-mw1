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
* Description:  Abstract base class definition for A/V recorders
*
*/


#ifndef CMRRECORDERBASE_H
#define CMRRECORDERBASE_H

//  INCLUDES
#include <e32base.h>
#include "CCMRConfigManager.h"

// CLASS DECLARATION

/**
*  Abstract base class for A/V recorders
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/

class CCMRRecorderBase : public CBase
    {
                 
    
    public: // Constants
    
        // Recorder states
        enum TRecorderState
            {
                EStateNone=0,
                EStateOpen,
                EStatePreparing,
                EStateReadyToRecord,
                EStateRecording,
                EStatePaused,
                EStateStopping
            };

    public:  // Constructors and destructor        

        /**
        * C++ default constructor.        
        */
        inline CCMRRecorderBase() : iTargetBitRate(-2) {}

        // CBase provides a virtual destructor

    public: // New functions
        
        /**
        * Set new target bitrate
        * @since 2.1
        * @param aBitrate bitrate
        * @return void
        */
        virtual inline void SetTargetBitRateL( TInt aBitRate ) { iTargetBitRate = aBitRate; }

        /**
        * Get current target bitrate
        * @since 2.1
        * @param void
        * @return TInt Bitrate     
        */
        virtual inline void TargetBitRateL(TInt& aTargetBitRate) { aTargetBitRate = iTargetBitRate; }

        /**
        * ?member_description.
        * @since 2.1
        * @param ?arg1 ?description
        * @return ?description
        */
        inline void SetState(TRecorderState aState) { iState = aState; }

        /**
        * Get current state of the recorder
        * @since 2.1
        * @param void
        * @return TRecorderState State
        */
        inline TRecorderState State() const { return iState; }

        /**
        * Checks if only dynamic recorder settings can be given. Other settings require prepare
        * @since 2.1
        * @param void
        * @return ETrue, if 
        */
        inline TBool StateRequiresDynamicSetting() const { return ( (iState == EStateReadyToRecord || iState == EStateRecording || iState == EStatePaused || iState == EStateStopping)? ETrue : EFalse); }

        /**
        * Prepares the recorder 
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void PrepareL() = 0;

        /**
        * Starts recording
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void RecordL() = 0;

        /**
        * Stops recording
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void StopL() = 0;

        /**
        * Pauses recording
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void PauseL() = 0;

        /**
        * Resumes recording
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void ResumeL() = 0; 
            
    protected:  // Data        

        // target bitrate in bits per second  
        TInt iTargetBitRate;     

    private:    // Data     

        // internal state
        TRecorderState iState;     

    };

            
// End of File

#endif      // CMRRECORDERBASE_H
