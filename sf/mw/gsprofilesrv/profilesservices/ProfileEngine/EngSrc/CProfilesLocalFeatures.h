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
* Description:  This class encapsulates the locally variated settings of
*                Profiles.
*
*/



#ifndef __CPROFILESLOCALFEATURES_H__
#define __CPROFILESLOCALFEATURES_H__

//  INCLUDES
#include <e32base.h>
#include <MProfilesLocalFeatures.h>

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
* Implementation of MProfilesLocalFeatures.
*/
NONSHARABLE_CLASS(CProfilesLocalFeatures) : public CBase, public MProfilesLocalFeatures
    {
    public: // Constructors and destructor

        static CProfilesLocalFeatures* NewL();

        static CProfilesLocalFeatures* NewL(
                CProfilesLocalFeatures& aLocalFeatures );

        ~CProfilesLocalFeatures();

    public: // Functions from base classes

        /**
         * From MProfilesLocalFeatures
         */
        TBool IsFeatureSupported( TInt aLocalFeatureId ) const;

        /**
         * From MProfilesLocalFeatures
         */
        TBool IsBlockedType( const TDesC& aMimeType );

        /**
         * From MProfilesLocalFeatures
         */
        TBool IsBlockedUnprotectedType( const TDesC& aMimeType );

        /**
         * From MProfilesLocalFeatures
         */
        TBool IsBlockedProtectedType( const TDesC& aMimeType );

        /**
         * From MProfilesLocalFeatures
         */
        TBool IsBlockedDemoPlayType( const TDesC& aMimeType );

        /**
         * From MProfilesLocalFeatures
         */
        TBool IsExcludedType( const TDesC& aMimeType );

    private: // New methods:

        void ReadListKeyL( CRepository& aCenRep,
                           TUint32 aKey,
                           RBuf& aValue );

        TBool ContainsMimeType( const TDesC& aList,
                                const TDesC& aMimeType );

    private: // By default symbian default constructor is private:

        void ConstructL();

    private: // Constructors:

        CProfilesLocalFeatures();

        CProfilesLocalFeatures( TInt aFeatureBitMask );

    private: // data

        // the key containing boolean type features
        TInt iFeatureBitMask;

        // Own: contains space separated list of blocked MIME-types
        RBuf iBlockedTones;

        // Own: contains space separated list of blocked unprotected MIME-types
        RBuf iBlockedUnprotectedTones;

        // Own: contains space separated list of blocked protected MIME-types
        RBuf iBlockedProtectedTones;

        // Own: contains space separated list of blocked demo-play MIME-types
        RBuf iBlockedDemoPlayTones;

        // Own: contains space separated list of excluded MIME-types
        RBuf iExcludedTones;

        // Own: helper buffer for mime-type matching
        RBuf iMimeBuf;
    };

#endif      //  __CPROFILESLOCALFEATURES_H__

// End of File


