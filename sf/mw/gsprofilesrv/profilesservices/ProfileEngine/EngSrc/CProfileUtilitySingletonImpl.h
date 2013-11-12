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
* Description:  This class encapsulates the implementation of MProfile-
*                UtilitySingleton.
*
*/



#ifndef __CPROFILEUTILITYSINGLETONIMPL_H__
#define __CPROFILEUTILITYSINGLETONIMPL_H__

//  INCLUDES
#include <e32base.h>
#include <MProfileUtilitySingleton.h>

// FORWARD DECLARATIONS
class MProfilesLocalFeatures;
class CProfilesLocalFeatures;

// CLASS DECLARATION

/**
* Implementation of MProfileUtility.
*/
NONSHARABLE_CLASS( CProfileUtilitySingletonImpl ) : public CBase,
                                     public MProfileUtilitySingleton
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProfileUtilitySingletonImpl* NewL();

        /**
         * Destructor.
         */
        ~CProfileUtilitySingletonImpl();

    public: // From base classes

        /**
        * From MProfileUtility
        */
        MProfilesLocalFeatures& ProfilesLocalFeatures();

    public: // New functions

        /**
         * Increments the counter of users of this singleton object.
         */
        void IncrementReferences();

        /**
         * Decrements the counter of users of this singleton object.
         * @return ETrue, when there are no users of this singleton anymore.
         */
        TBool DecrementReferences();

    private: // Constructors

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CProfileUtilitySingletonImpl();

    private: // data

        /// Profiles local features owned by this class.
        CProfilesLocalFeatures* iLocalFeatures;

        /// References to this CProfileUtilitySingletonImpl instance.
        TInt iReferences;
    };

#endif      //  __CPROFILEUTILITYSINGLETONIMPL_H__

// End of File

