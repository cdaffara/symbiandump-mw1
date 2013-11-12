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
* Description:  Implementaion of MProEngEngine interface.
*
*/



#ifndef CPROENGENGINEIMPL_H
#define CPROENGENGINEIMPL_H

// INCLUDES
#include <MProEngEngine.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class RFs;
class MProEngProfile;
class MProEngProfileNameArray;
class MProfileEngineExtended;
class CProEngToneHandler;
class CProEngProfileImpl;

// CLASS DECLARATION

/**
* This class implements MProEngEngine interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngEngineImpl ) : public CBase,
                                         public MProEngEngine
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngEngineImpl* NewL();

        /**
         * Two-phased constructor.
         */
        static CProEngEngineImpl* NewLC();

        /**
         * Two-phased constructor.
         */
        static CProEngEngineImpl* NewL( RFs& aFs );

        /**
         * Two-phased constructor.
         */
        static CProEngEngineImpl* NewLC( RFs& aFs );

        /**
         * Destructor.
         */
        virtual ~CProEngEngineImpl();

    public:

        /**
         * From MProEngEngine
         */
        void Release();

        /**
         * From MProEngEngine
         */
        MProEngProfile* ActiveProfileLC();

        /**
         * From MProEngEngine
         */
        MProEngProfile* ActiveProfileL();

        /**
         * From MProEngEngine
         */
        TInt ActiveProfileId();

        /**
         * From MProEngEngine
         */
        MProEngProfileNameArray* ProfileNameArrayLC();

        /**
         * From MProEngEngine
         */
        void SetActiveProfileL( TInt aId );

        /**
         * From MProEngEngine
         */
        MProEngProfile* ProfileLC( TInt aId );

        /**
         * From MProEngEngine
         */
        MProEngProfile* ProfileL( TInt aId );

    public:  // New functions

        /**
         * Gets the data of the given profile.
         * @since 3.1
         * @param aId the ID of the profile which data is retrieved.
         * @return Pointer to the implementation of profile data wrapper.
         */
        CProEngProfileImpl* ProfilePrivateLC( TInt aId );

        /**
         * Returns a reference to the underlying Profiles Engine Extended.
         * @since 3.1
         * @return Reference to the underlying Profiles Engine Extended.
         */
        MProfileEngineExtended& ProfileEngineExtended();

        /**
         * Returns a reference to the tone handler needed for checking the
         * legality of the alert tones set by the client.
         * @since 3.1
         * @return Reference to the tone handler.
         */
        CProEngToneHandler& ToneHandler();

    private:  // New functions

        /**
         * Checks if the given profile ID is in the range reserved for dynamic
         * profiles. However, the method does not check whether a profile with
         * the given ID really exists.
         * @since 3.1
         * @param aId the profile ID to be checked
         * @return ETrue if the ID is in the range of dynamic profile IDs,
         *         EFalse otherwise
         */
        TBool IsDynamicProfileId( TInt aId );

    private:

        /**
         * C++ default constructor.
         */
        CProEngEngineImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RFs& aFs );

    private:    // Data

        // Own: the actual Profiles Engine
        MProfileEngineExtended* iProfileEngine;

        // Own: tone handler for DRM checkups etc.
        CProEngToneHandler* iToneHandler;
    };

#endif      //  CPROENGENGINEIMPL_H

// End of File

