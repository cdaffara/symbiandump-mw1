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
* Description:  Profile 3D tone settings implementation.
*
*/



#ifndef __CPROFILE3DTONESETTINGSIMPL_H
#define __CPROFILE3DTONESETTINGSIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfile3DToneSettings.h>
#include <MProfileSet3DToneSettings.h>

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
*  Profile extra tones implementation.
*
*  @lib ProfileEng.lib
*  @since 3.1
*/
NONSHARABLE_CLASS(CProfile3DToneSettingsImpl) : public CBase,
                               public MProfile3DToneSettings,
                               public MProfileSet3DToneSettings
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfile3DToneSettingsImpl* NewL();
        static CProfile3DToneSettingsImpl* NewL(
                const MProfile3DToneSettings& aProfile3DToneSettings );

        /**
        * Destructor.
        */
        virtual ~CProfile3DToneSettingsImpl();

    public: // New functions

        /**
        * Internalize data from the central repository.
        * @since 3.1
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to Central Repository.
        * @since 3.1
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfile3DToneSettings.
        */
        TProfile3DToneEffect Effect() const;

        /**
        * From MProfile3DToneSettings.
        */
        TProfile3DToneEcho Echo() const;

        /**
        * From MProfileSet3DToneSettings.
        */
        void SetEffect( TProfile3DToneEffect aEffect );

        /**
        * From MProfileSet3DToneSettings.
        */
        void SetEcho( TProfile3DToneEcho aEcho );

    private:    // Data

        // 3D effect
        TProfile3DToneEffect iEffect;

        // 3D echo
        TProfile3DToneEcho iEcho;
    };

#endif      // __CPROFILE3DTONESETTINGSIMPL_H

// End of File


