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
* Description:  Header file for audio codecs class which keeps information 
*                about audio codec types and their location (SW/HW)
*
*/


#ifndef CCMRAUDIOCODECS_H
#define CCMRAUDIOCODECS_H

// INCLUDES
#include "CCMRAudioCodecData.h"
#include <e32base.h>
#include <mmf/common/mmffourcc.h>


// CLASS DECLARATION

/**
*  A helper class to find out the supported audio codecs
*
*  @lib 
*  @since 2.6
*/
class CCMRAudioCodecs : public CBase
                          

    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CCMRAudioCodecs();

        /**
        * Two-phased constructor.
        * @return CCMRAudioCodecs* Pointer to constructed audiocodecs
        */
        static CCMRAudioCodecs* NewL();

        
    public: // New functions
        
        /**
        * Get list of supported & installed audio codecs.
        * @since 2.6
        * @param  aAudioTypes   List of supported & installed audio codecs as FourCC type
        * @return void
        */
        virtual void GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes );

        /**
        * Get codec's type: SW (CMMFCodec) or HW (HW Device)
        * @since 2.7
        * @param  aCodec    FourCC codec in question
        * @return ECodecTypeSW/ECodecTypeHW/ECodecTypeNA
        */
        TCMRCodecType GetCodecSWHWTypeL( const TFourCC& aCodec );

        /**
        * Check if we support the given codec
        * @since 2.7
        * @param  aCodec    FourCC codec in question
        * @return ETrue if supported
        */
        TBool CheckIfSupportedL( const TFourCC& aAudioCodec );

        /**
        * Set used audio codec
        * @since 2.7
        * @param  aCodec    FourCC codec in question
        */
        virtual void SetAudioCodecL( const TFourCC& aAudioCodec );

        /**
        * Get codecdata-object for the selected codec
        * @since 2.7
        * @return CCMRAudioCodecData*
        */
        virtual CCMRAudioCodecData* GetCodecDataL();

    protected:

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL();

        /**
        * Internal helper function to check if given codec FourCC is supported in the system, either HW or SW
        */
        TBool CheckIfInstalledL( const TFourCC& aFourCC, const TInt aUidMmfPluginInterfaceCodec, const TUid& aCodecUid );
        
        /**
        * Internal helper function to check if given codec FourCC is supported in devsound HW
        */
        TBool CheckIfHWInstalledL( const TFourCC& aFourCC );

    private :
        /**
        * C++ default constructor.
        */
        inline CCMRAudioCodecs() {};

    protected: // member data
      
        // List of supported & installed SW audio codecs (CMMFCodec)
        RArray<TFourCC> iAudioTypesSW;

        // List of supported & installed HW audio codecs (HW device)
        RArray<TFourCC> iAudioTypesHW;

        // Data for the selected audio codec
        CCMRAudioCodecData* iAudioCodec;
    };  
    
#endif      // CCMRAUDIOCODECS_H
    
// End of File
    
    
    
    
    

    
    
    
