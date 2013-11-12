/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for locally variated features of Profiles.
*
*/



#ifndef __MPROFILESLOCALFEATURES_H__
#define __MPROFILESLOCALFEATURES_H__

// CLASS DECLARATION

/**
*  Profiles local feature variation interface.
*  MProfilesLocalFeatures offers simple interface to get locally variated
*  features of Profiles.
*  How to use:
*  Get local features object with MProfileEngineExtended::LocalFeaturesL().
*     MProfilesLocalFeatures& features = iProfileEngine->LocalFeatures();
*  Check whether a feature is supported (use the ID:s defined in
*  ProfilesVariant.hrh):
*     if( features.IsFeatureSupported( KFeatureIdSomeFeature ) )
*         {
*         // do stuff
*         }
*
*  @lib N/A
*  @since 2.6
*/
class MProfilesLocalFeatures
    {
    protected:  // Destructor

        virtual ~MProfilesLocalFeatures() {};

    public: // New functions

        /**
        * Checks if the given feature is supported or not.
        * @since 2.6
        * @return ETrue if the feature is supported, EFalse otherwise.
        */
        virtual TBool IsFeatureSupported( TInt aFeatureId ) const = 0;

        /**
         * Checks if the given MIME-type is explicitly blocked from using as
         * an alert tone.
         * @since 3.1
         * @param aMimeType the MIME-type to be checked.
         * @return ETrue if the MIME-type is blocked, EFalse otherwise.
         */
        virtual TBool IsBlockedType( const TDesC& aMimeType ) = 0;

        /**
         * Checks if the given MIME-type is explicitly blocked from using as
         * an alert tone if the alert tone file of the type is unprotected.
         * @since 3.1
         * @param aMimeType the MIME-type to be checked.
         * @return ETrue if the MIME-type is blocked if the alert tone file is
         *         unprotected, EFalse otherwise.
         */
        virtual TBool IsBlockedUnprotectedType( const TDesC& aMimeType ) = 0;

        /**
         * Checks if the given MIME-type is explicitly blocked from using as
         * an alert tone if the alert tone file of the type is protected.
         * @since 3.1
         * @param aMimeType the MIME-type to be checked.
         * @return ETrue if the MIME-type is blocked if the alert tone file is
         *         protected, EFalse otherwise.
         */
        virtual TBool IsBlockedProtectedType( const TDesC& aMimeType ) = 0;

        /**
         * Checks if the given MIME-type is explicitly blocked from demo
         * playing.
         * @since 3.1
         * @param aMimeType the MIME-type to be checked.
         * @return ETrue if the MIME-type is blocked from demo-playing,
         *         EFalse otherwise.
         */
        virtual TBool IsBlockedDemoPlayType( const TDesC& aMimeType ) = 0;

        /**
         * Checks if the given MIME-type is explicitly excluded from the tone
         * selection list. Also when tried to be set as alert tones through
         * other means, should behave as the tone would be in blocked type list.
         * @since 3.1
         * @param aMimeType the MIME-type to be checked.
         * @return ETrue if the MIME-type is excluded from the tone selection
         *         list, EFalse otherwise.
         */
        virtual TBool IsExcludedType( const TDesC& aMimeType ) = 0;
    };

#endif      //  __MPROFILESLOCALFEATURES_H__

// End of File
