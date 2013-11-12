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
* Description:  Video thread proxy session class
*
*/

#ifndef CCMRVIDEOTHREADPROXYSESSION_H
#define CCMRVIDEOTHREADPROXYSESSION_H

// INCLUDES
#include "CCMRVideoThreadProxy.h"




// FORWARD DECLARATIONS
class CCMRVideoRecorder;
class CCMRVideoThreadProxyServer;

// CLASS DECLARATION


/**
*  Video thread proxy session class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRVideoThreadProxySession : public CCMRThreadProxySession
    {
    public: // constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CCMRVideoThreadProxySession* NewL();
        /**
        * Destructor.
        */
        ~CCMRVideoThreadProxySession();

    public: // from base classes
        /**                     
        * From CCMRThreadProxySession/CSharableSessions service a message.
        */
        void ServiceL(const RMessage2& aMessage);

        /**                     
        * From CCMRThreadProxySession/MAsyncEventHandler catches 
        * completion events before calling base class implementation
        */
        TInt SendEventToClient(const TMMFEvent& aEvent);

    private: // new functions
        
        /**
        * Loads video recorder
        * @since 2.1
        * @return TBool
        */
        TBool LoadVideoRecorderL(const RMessage2& aMessage);
        /**
        * Set camera handle
        * @since 2.1
        * @return TBool
        */
        TBool SetCameraHandleL(const RMessage2& aMessage);
        /**
        * Set output active object
        * @since 2.1
        * @return TBool
        */
        TBool SetOutputL(const RMessage2& aMessage);
        /**
        * Set clock source
        * @since 3.0
        * @return TBool
        */
        TBool SetClockSource(const RMessage2& aMessage);
        /**
        * Set thread id of the client & output active object
        * @since 2.1
        * @return TBool
        */
        TBool SetOutputThreadIdL(const RMessage2& aMessage);
        /**
        * Set target bitrate
        * @since 2.1
        * @return TBool
        */
        TBool SetBitRateL(const RMessage2& aMessage);
        /**
        * Get current target bitrate
        * @since 2.1
        * @return TBool
        */
        TBool GetBitRateL(const RMessage2& aMessage) const;
        /**
        * Set video frame size (width&height)
        * @since 2.1
        * @return TBool
        */
        TBool SetFrameSizeL(const RMessage2& aMessage);
        /**
        * Get current video frame size (width&height)
        * @since 2.1
        * @return TBool
        */
        TBool GetFrameSizeL(const RMessage2& aMessage) const;
        /**
        * Set video framerate
        * @since 2.1
        * @return TBool
        */
        TBool SetFrameRateL(const RMessage2& aMessage);
        /**
        * Get current video framerate
        * @since 2.1
        * @return TBool
        */
        TBool GetFrameRateL(const RMessage2& aMessage) const;
        /**
        * Set video codec MIME-type
        * @since 2.1
        * @return TBool
        */
        TBool SetCodecL(const RMessage2& aMessage);
        /**
        * Get current video codec MIME-type
        * @since 2.1
        * @return TBool
        */
        TBool GetCodecL(const RMessage2& aMessage) const;
        /**
        * Set video options
        * @since 2.1
        * @return TBool
        */
        TBool SetMiscOptionsL(const RMessage2& aMessage);
        
        /**                     
        * Set video rate control options
        * @since 3.2
        * @return TBool
        */        
        TBool SetVideoRateControlOptionsL(const RMessage2& aMessage);  
        
        /**                     
        * Get video rate control options
        * @since 3.2
        * @return TBool
        */
        TBool GetVideoRateControlOptionsL(const RMessage2& aMessage) const;

        /**
        * Set video encoder using its UID. Usage optional.
        * @since 3.2.3
        * @param    "aEncoder"  "Video encoder UID."
        * @return TBool
        */
        TBool SetPreferredVideoEncoderL(const RMessage2& aMessage);

        /**
        * Set video encoder output format encapsulation. Usage optional.
        * @since 3.2.3
        * @param    "aCapsulation"  "Encapsulation for coded video data units."
        * @return TBool
        */
        TBool SetPreferredVideoEncapsulationL(const RMessage2& aMessage);

        /**
        * Set video encoder target segment size. Usage optional.
        * @since 5.2
        * @param  TUint aLayer              Layer number
        * @param  TUint aSizeBytes          Segment target size in bytes
        * @param  TUint aSizeMacroblocks    Segment target size in number of macroblocks per segment
        * @return TBool
        */        
        TBool SetSegmentTargetSizeL(const RMessage2& aMessage);
        
        /**
        * Adjust video time stamps (+ or -)
        * @since 2.1
        * @return TBool
        */
        TBool AdjustTimeStampsL(const RMessage2& aMessage);
        /**
        * Prepare for recording (freeze settings)
        * @since 2.1
        * @return TBool
        */
        TBool PrepareL();
        /**                     
        * Start recording
        * @since 2.1
        * @return TBool
        */
        TBool RecordL();
        /**                     
        * Stop recording
        * @since 2.1
        * @return TBool
        */
        TBool StopL();
        /**                     
        * Used while stopping. Requests encoded video frames from adaptation and waits for the streamend / EOS notification.
        * @since 5.1
        * @return TBool
        */
        TBool RequestBuffersAndWaitEOSL(const RMessage2& aMessage);        
        /**                     
        * Pause recording
        * @since 2.1
        * @return TBool
        */
        TBool PauseL();
        /**                     
        * Resume recording
        * @since 2.1
        * @return TBool
        */
        TBool ResumeL();

    private: // constructor
        /**
        * C++ default constructor.
        */
        inline CCMRVideoThreadProxySession() {};

    private: // data
        // video recorder
        CCMRVideoRecorder* iRecorder;
        // ETrue if we are stopping and stopping hasn't completed yet.
        TBool iStopping;
        // Handle to the message, needed in async stopping
        RMessage2* iMessage;
    };

#endif      
            
// End of File



