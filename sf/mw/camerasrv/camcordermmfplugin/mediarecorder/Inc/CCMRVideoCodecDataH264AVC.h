/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Header file for H.264 AVC video codec data class
*
*/


#ifndef CCMRVIDEOCODECDATAH264AVC_H
#define CCMRVIDEOCODECDATAH264AVC_H

// INCLUDES
#include "CCMRVideoCodecData.h"
#include <e32base.h>

// CLASS DECLARATION

/**
*  Base class for audio codec data
*
*  @lib 
*  @since 3.2
*/
class CCMRVideoCodecDataH264AVC : public CCMRVideoCodecData
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        inline CCMRVideoCodecDataH264AVC(TInt aVideoCodecLevel) : CCMRVideoCodecData( CCMRMediaBuffer::EVideoH264NAL, aVideoCodecLevel ) {};

        /**
        * Destructor.
        */
        virtual ~CCMRVideoCodecDataH264AVC() {};
       
    public: // functions from base classes

        /**                     
        * Get the max bitrate for the used level
        * @return TInt bitrate in bps
        */
        TInt MaxBitRate() const;

        /**                     
        * Get the max framerate for the used level
        * @return TReal32 framerate in fps
        */
        TReal32 MaxFrameRate(const TSize& aFrameSize) const;

        /**                     
        * Get the max frame size for the used level
        * @param TSize& framesize
        */
        TBool MaxFrameSize(const TSize& aFrameSize) const;

        /**                     
        * Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
        * @return TInt Max frame length in bytes
        */
        TInt MaxBufferLength(const TSize& aFrameSize) const;

        /**
        * Check if codec level indicates we are recording for MMS; then some tighter restrictions apply
        * @return ETrue if level is a typical MMS level
        */
        TBool LevelForMMS() const;

        /**
        * Check if decoder configuration info is needed in separate buffer
        * @return ETrue since this is MPEG-4
        */
        inline TBool DecoderConfigInfoUsed() const { return ETrue; };

        /**
        * Set misc video coding options
        * @param  aOptions      video coding options
        * @return void
        */
        void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions);
  
        /**
        * Set codec-specific video coding options to DevVideoRecord before initialize
        * @param  aDevVideoRecord
        * @return void
        */
        void SetPreInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const;
        
        /**
        * Set codec-specific video coding options to DevVideoRecord after initialize
        * @param  aDevVideoRecord
        * @return void
        */
        void SetPostInitParamsL(CMMFDevVideoRecord* aDevVideoRecord) const;
  
    };  
    


#endif      // CCMRVIDEOCODECDATAH264AVC_H
    
// End of File
