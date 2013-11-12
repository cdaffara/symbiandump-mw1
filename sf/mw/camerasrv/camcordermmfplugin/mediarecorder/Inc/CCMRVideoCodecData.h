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
* Description:  Header file for video codec data class which is specific to each codec type
*
*/


#ifndef CCMRVIDEOCODECDATA_H
#define CCMRVIDEOCODECDATA_H

// INCLUDES
#include "CCMRMediaSink.h"

#include <e32base.h>

// FORWARD DECLARATION
class CMMFDevVideoRecord;
class TCCMRVideoCodingOptions;


// CLASS DECLARATION

/**
*  Base class for audio codec data
*
*  @lib 
*  @since 2.7
*/
class CCMRVideoCodecData : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        inline CCMRVideoCodecData(CCMRMediaBuffer::TBufferType aCodecType, TInt aVideoCodecLevel) : iCodecType(aCodecType), iVideoCodecLevel(aVideoCodecLevel) {};

        /**
        * Destructor.
        */
        virtual ~CCMRVideoCodecData() {};
       
    public: // New functions

        /**                     
        * Get the max bitrate for the used level
        * @return TInt bitrate in bps
        */
        virtual TInt MaxBitRate() const = 0;

        /**                     
        * Get the max framerate for the used level
        * @param TSize& aFrameSize
        * @return TReal32 framerate in fps
        */
        virtual TReal32 MaxFrameRate(const TSize& aFrameSize) const = 0;

        /**                     
        * Get the max frame size for the used level
        * @param TSize& aFrameSize
        */
        virtual TBool MaxFrameSize(const TSize& aFrameSize) const = 0;

        /**                     
        * Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
        * @param TSize& aFrameSize
        * @return TInt Max frame length in bytes
        */
        virtual TInt MaxBufferLength(const TSize& aFrameSize) const = 0;

        /**
        * Check if codec level indicates we are recording for MMS; then some tighter restrictions apply
        * @return ETrue if level is a typical MMS level
        */
        virtual TBool LevelForMMS() const = 0;

        /**
        * Check if decoder configuration info is needed in separate buffer
        * @return ETrue if decoder configuration info is needed
        */
        virtual TBool DecoderConfigInfoUsed() const = 0;

        /**
        * Set misc video coding options
        * @param  aOptions      video coding options
        * @return void
        */
        virtual void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions) = 0;
        
        /**
        * Set codec-specific video coding options to DevVideoRecord before initialize
        * @param  CMMFDevVideoRecord* aDevVideoRecord
        * @return void
        */
        virtual void SetPreInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const = 0;
        
        /**
        * Set codec-specific video coding options to DevVideoRecord after initialize
        * @param  CMMFDevVideoRecord* aDevVideoRecord
        * @return void
        */
        virtual void SetPostInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const = 0;
        
        /**                     
        * Returns the CCMRMediaSink/CCMRMediaBuffer compatible type of the codec buffer 
        * @return CCMRMediaBuffer::TBufferType buffer type
        */
        inline CCMRMediaBuffer::TBufferType CodecType() const { return iCodecType; };

    protected:

        // Codec type in highest level: H.263 or MPEG-4; using CCMRMediaBuffer types since it is the only existing available enum for codecs
        CCMRMediaBuffer::TBufferType iCodecType;

        // Codec level; actual values depend on the codec used, e.g for H.263 the values are 10,45,20, ... and for MPEG-4 0,9 (=0b),1,2,3
        TInt iVideoCodecLevel;
        
        // Indicates if client expects any bit errors => should video codec be configured for error resilience
        TReal iExpectedBitErrorRate;
    
    };  
    


#endif      // CCMRVIDEOCODECDATA_H
    
// End of File
