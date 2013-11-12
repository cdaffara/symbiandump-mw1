/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Header file for H.263 video codec data class
*
*/


#ifndef CCMRVIDEOCODECDATAH263_H
#define CCMRVIDEOCODECDATAH263_H

// INCLUDES
#include "CCMRVideoCodecData.h"
#include "CCMRVideoSettings.h"

#include <e32base.h>



// CLASS DECLARATION

/**
*  Base class for audio codec data
*
*  @lib 
*  @since 2.7
*/
class CCMRVideoCodecDataH263 : public CCMRVideoCodecData
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        inline CCMRVideoCodecDataH263(TInt aVideoCodecLevel) : CCMRVideoCodecData( CCMRMediaBuffer::EVideoH263, aVideoCodecLevel ), iSyncIntervalInPicture(KCMRSyncIntervalInPicture) {};

        /**
        * Destructor.
        */
        virtual ~CCMRVideoCodecDataH263() {};
       
    public: // functions from base classes

        /**                     
        * Get the max bitrate for the used level
        * @return TInt bitrate in bps
        */
        TInt MaxBitRate() const;

        /**                     
        * Get the max framerate for the used level
        * @param TSize& aFrameSize
        * @return TReal32 framerate in fps
        */
        TReal32 MaxFrameRate(const TSize& aFrameSize) const;

        /**                     
        * Get the max frame size for the used level
        * @param TSize& aFrameSize
        * @return TBool 
        */
        TBool MaxFrameSize(const TSize& aFrameSize) const;

        /**                     
        * Returns the max used buffer length for this codec. Client can then e.g. allocate buffers based on it
        * @param TSize& aFrameSize
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
        * @return EFalse since this is H.263
        */
        inline TBool DecoderConfigInfoUsed() const { return EFalse; };

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
  
    private:

        // Interval of GOB headers in picture
        TInt iSyncIntervalInPicture;
    
    };  
    


#endif      // CCMRVIDEOCODECDATAH263_H
    
// End of File
