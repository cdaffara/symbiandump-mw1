/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class implements MProEngProfile interface.
*
*/



#ifndef CPROENGPROFILEIMPL_H
#define CPROENGPROFILEIMPL_H

// INCLUDES
#include <MProEngProfile.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MProEngProfileName;
class MProEngTones;
class MProEngToneSettings;
class MProfileExtended;
class CProEngEngineImpl;
class CProEngProfileNameImpl;
class CProEngTonesImpl;
class CProEngToneSettingsImpl;

// CLASS DECLARATION

/**
* This class implements MProEngProfile interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngProfileImpl ) : public CBase,
                                          public MProEngProfile
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aProfileEngine wrapper for Profiles Engine.
         * @param aProfile private interface for Profiles data, ownership is
         *        transferred.
         */
        static CProEngProfileImpl* NewL( CProEngEngineImpl& aProfileEngine,
                                         MProfileExtended* aProfile );

        /**
         * Destructor.
         */
        virtual ~CProEngProfileImpl();

    public:

        /**
         * From MProEngProfile
         */
        void Release();

        /**
         * From MProEngProfile
         */
        const TArray<TContactItemId> AlertForL();

        /**
         * From MProEngProfile
         */
        TInt SetAlertForL( const TArray<TContactItemId>& aAlertFor );

        /**
         * From MProEngProfile
         */
        TBool IsSilent() const;

        /**
         * From MProEngProfile
         */
        MProEngProfileName& ProfileName() const;

        /**
         * From MProEngProfile
         */
        MProEngTones& ProfileTones() const;

        /**
         * From MProEngProfile
         */
        MProEngToneSettings& ToneSettings() const;

        /**
         * From MProEngProfile
         */
        void CommitChangeL();

    private:

        /**
         * C++ constructor.
         */
        CProEngProfileImpl( CProEngEngineImpl& aProfileEngine,
                            MProfileExtended* aProfile );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Ref: the Profiles Engine wrapper
        CProEngEngineImpl& iProfileEngine;

        // Own: the real implementation of the profile
        MProfileExtended* iProfile;

        // Own: profile name wrapper implementation
        CProEngProfileNameImpl* iProfileName;

        // Own: profile tones wrapper implementation
        CProEngTonesImpl* iTones;

        // Own: profile tone settings wrapper implementation
        CProEngToneSettingsImpl* iToneSettings;
    };

#endif      //  CPROENGPROFILEIMPL_H

// End of File

