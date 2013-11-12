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
* Description:  Header file for video codec class
*                Codec variation is handled through this class: MPEG-4 video 
*                can be supported only if feature flag __MPEG4_VIDEO_ENCODING
*                is defined. Most of the MPEG-4 code is embedded, but the current
*                controller allows to set MPEG-4 iff GetSupportedVideoCodecsL
*                returns MPEG-4 codec as one of the supported codecs.
*                There are two implementations for this class, one that checks 
*                DevVideoRecord for basic codecs (H.263 only), and another one
*                that checks H.263 and MPEG-4. The basic one is used if the MPEG4
*                feature flag is not defined.
*
*/


#ifndef CCMRVIDEOCODECS_H
#define CCMRVIDEOCODECS_H

// INCLUDES
#include <e32base.h>
#include <badesca.h>    // CDesC8ArrayFlat



// CLASS DECLARATION

/**
*  Internal class to check supported video codecs
*
*  @lib 
*  @since 2.6
*/
class CCMRVideoCodecs : public CBase
                          

    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CCMRVideoCodecs* NewL();

        /**
        * Destructor.
        */
        virtual ~CCMRVideoCodecs();               

    public: // New functions
        
        /**
        * Get supported & installed video codecs.
        * @since 2.6
        * @param  aVideoMimeTypes   List of supported & installed video codecs as MIME type
        * @return void
        */
        void GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes ) const;

        /**
        * Check if the given codec is supported & installed.
        * @since 2.6
        * @param  aMimeType   Video codec MIME type
        * @return void
        */
        TBool DoWeSupportThisL(const TDesC8& aMimeType) const;

    protected:

        /**
        * Generate list of supported & installed video codecs.
        * Called internally in the construction
        */
        void GenerateListOfSupportedVideoCodecsL();

    private:
        /**
        * C++ default constructor.
        */
        inline CCMRVideoCodecs() {};
                        
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

    private: // Data

        // List of supported & installed video codecs
        CDesC8ArrayFlat* iVideoMimeTypes;


    };  
    
#endif      // CCMRVIDEOCODECS_H
    
// End of File
    
    
    
    
    

    
    
    
