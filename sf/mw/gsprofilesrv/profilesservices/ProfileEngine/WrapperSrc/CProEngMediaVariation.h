/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is used to read the variated features concerning 
* 				different mime types of media files.
*
*/


#ifndef CPROENGMEDIAVARIATION_H
#define CPROENGMEDIAVARIATION_H


// INCLUDE FILES
#include    <e32base.h>
#include    <apgcli.h> // RApaLsSession

// FORWARD DECLARATIONS
class MProfilesLocalFeatures;

// CLASS DECLARATION

/**
* This class is used to read the variated features concerning different mime
* types of media files.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngMediaVariation ) : public CBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngMediaVariation* NewL();

        /**
         * Destructor.
         */
        virtual ~CProEngMediaVariation();

    public:  // New functions

        /**
         * Returns the MIME type of the given file.
         * @param aFileName the file the MIME type of which should be found out.
         * @param aDataType contains the MIME type of aFileName on return.
         */
        void GetDataTypeL( const TDesC& aFileName, TDes16& aDataType ) const;

        /**
         * Checks if files of the given MIME type are supported to be used as
         * alert tones.
         * @param aMimeType the MIME type to be checked
         * @return ETrue if aMimeType is supported, EFalse otherwise.
         */
        TBool IsSupported( const TDesC16& aMimeType );

        /**
         * Checks if files of the given MIME type are allowed to be used as
         * alert tones if they are DRM-protected.
         * @param aMimeType the MIME type to be checked
         * @return ETrue if files of the given type are allowed,
         * EFalse otherwise.
         */
        TBool IsAllowedProtected( const TDesC16& aMimeType ) const;

        /**
         * Checks if files of the given MIME type are allowed to be used as
         * alert tones if they are NOT DRM-protected.
         * @param aMimeType the MIME type to be checked
         * @return ETrue if files of the given type are allowed,
         * EFalse otherwise.
         */
        TBool IsAllowedUnProtected( const TDesC16& aMimeType ) const;

    private:

        /**
         * C++ default constructor.
         */
        CProEngMediaVariation();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        /// Needed to get the MIME-type of the files
        RApaLsSession iApaLsSession;

        /// Not owned: Profiles local features
        MProfilesLocalFeatures* iLocalFeatures;

        /// Array holding the MIME types supported by MMF (and DevSound)
        CDesC8ArrayFlat* iSupportedMimeTypes;

        /// 8-bit helper buffer needed in IsSupported()
        RBuf8 iMimeTypeBuf;
    };

#endif      //  CPROENGMEDIAVARIATION_H

//  End of File

