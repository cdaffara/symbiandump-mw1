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
* Description:  Profile names array implementation.
*
*/



#ifndef CPROFILESNAMESARRAYIMPL_H
#define CPROFILESNAMESARRAYIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfilesNamesArray.h>

// FORWARD DECLARATIONS
class CProfileNameImpl;

// CLASS DECLARATION

/**
*  Profile names array implementation.
*  This class implements MProfilesNamesArray interface.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfilesNamesArrayImpl) : public CBase,
                                public MProfilesNamesArray
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfilesNamesArrayImpl* NewL();

        /**
        * Two-phased constructor.
        */
        static CProfilesNamesArrayImpl* NewLC();

        /**
        * Destructor.
        */
        virtual ~CProfilesNamesArrayImpl();

    public: // New functions

        /**
        * Insert profile name in array, sort by ID.
        * @since 2.0
        * @param aProfileName Profile name instance
        * @return System error code
        */
        TInt InsertInOrder( const CProfileNameImpl* aProfileName );

        /**
        * Append profile names in array.
        * @since 2.0
        * @param aProfileName Profile name instance
        * @return System error code
        */
        TInt Append( const CProfileNameImpl* aProfileName );

        /**
        * Find profile name by ID.
        * @since 2.0
        * @param aId Profile ID
        * @return Profile name
        */
        const TDesC& FindNameById( TInt aId );

    public: // Functions from base classes

        /**
        * From MProfilesNamesArray.
        */
        virtual const MProfileName* ProfileName( TInt aIndex ) const;

        /**
        * From MProfilesNamesArray.
        */
        virtual TInt FindById( TInt aId ) const;

        /**
        * From MProfilesNamesArray.
        */
        virtual TInt FindByName( const TDesC& aProfileName ) const;

        /**
        * From MDesCArray.
        */
        virtual TInt MdcaCount() const;

        /**
        * From MDesCArray.
        */
        virtual TPtrC MdcaPoint( TInt aIndex ) const;

    private:

        /**
        * C++ default constructor.
        */
        CProfilesNamesArrayImpl();

    private:    // Data

        // Own: Profiles names array
        RPointerArray<CProfileNameImpl> iProfilesNames;

        // Own: Empty name
        TBuf<1> iNullName;

    };

#endif      // CPROFILESNAMESARRAYIMPL_H

// End of File
