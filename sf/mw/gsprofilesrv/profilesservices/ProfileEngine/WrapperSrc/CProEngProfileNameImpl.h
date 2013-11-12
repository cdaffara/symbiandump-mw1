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
* Description:  This class implements MProEngProfileName interface.
*
*/



#ifndef CPROENGPROFILENAMEIMPL_H
#define CPROENGPROFILENAMEIMPL_H

// INCLUDES
#include <MProEngProfileName.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MProfileName;
class MProfileSetName;
class CProEngEngineImpl;

// CLASS DECLARATION

/**
* This class implements MProEngProfileName interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngProfileNameImpl ) : public CBase,
                                              public MProEngProfileName
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngProfileNameImpl* NewL(
                const MProfileName& aProfileName,
                MProfileSetName& aProfileSetName,
                CProEngEngineImpl& aEngine,
                TBool aModifiable );

        /**
         * Destructor.
         */
        virtual ~CProEngProfileNameImpl();

    public: // Functions from base classes

        /**
         * From MProEngProfileName
         */
        TInt Id() const;

        /**
         * From MProEngProfileName
         */
        const TDesC& Name() const;

        /**
         * From MProEngProfileName
         */
        TInt SetNameL( const TDesC& aName );

    public: // New functions

        /**
         * Checks if the name to be given for the profiles is already used by
         * another profile.
         * @param aName the name to be set to profile.
         * @return ETrue if there already is a profile with the given name.
         * @since 3.1
         */
        TBool AlreadyExistsL( const TDesC& aName );

    private:

        /**
         * C++ constructor.
         */
        CProEngProfileNameImpl(
                const MProfileName& aProfileName,
                MProfileSetName& aProfileSetName,
                CProEngEngineImpl& aEngine,
                TBool aModifiable );

    private:    // Data

        // the interface to the implementation of the profile name
        const MProfileName& iProfileName;

        // the interface to the implementation of the profile set name
        MProfileSetName& iProfileSetName;

        // Engine ref. needed for checking whether a profile name already exists
        CProEngEngineImpl& iEngine;

        // Is name modification allowed
        TBool iModifiable;
    };

#endif      //  CPROENGPROFILENAMEIMPL_H

// End of File



