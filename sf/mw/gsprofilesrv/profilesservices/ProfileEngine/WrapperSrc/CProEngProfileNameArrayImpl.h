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
* Description:  This class implements MProEngProfileNameArray 
*               interface.
*
*/



#ifndef CPROENGPROFILENAMEARRAYIMPL_H
#define CPROENGPROFILENAMEARRAYIMPL_H

// INCLUDES
#include <MProEngProfileNameArray.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MProEngProfileName;
class MProfilesNamesArray;

// CLASS DECLARATION

/**
* This class implements MProEngProfileNameArray interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngProfileNameArrayImpl )
    : public CBase,
      public MProEngProfileNameArray
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aProfilesNamesArray the internal interface to the profile name
        *        array. Ownership is transferred.
        */
        static CProEngProfileNameArrayImpl* NewL(
                MProfilesNamesArray* aProfilesNamesArray );

        /**
         * Destructor.
         */
        virtual ~CProEngProfileNameArrayImpl();

    public:

        /**
         * From MDesCArray
         */
        TInt MdcaCount() const;

        /**
         * From MDesCArray
         */
        TPtrC MdcaPoint( TInt aIndex ) const;

        /**
         * From MProEngProfileNameArray
         */
         TInt ProfileId( TInt aIndex ) const;

        /**
         * From MProEngProfileNameArray
         */
         TInt FindById( TInt aId ) const;

        /**
         * From MProEngProfileNameArray
         */
         TInt FindByName( const TDesC& aProfileName ) const;

    private:

        /**
         * C++ default constructor.
         */
        CProEngProfileNameArrayImpl( MProfilesNamesArray* aProfilesNamesArray );

    private:    // Data

        // Own: the real implementation of the profile name array
        MProfilesNamesArray* iProfilesNamesArray;

    };

#endif      //  CPROENGPROFILENAMEARRAYIMPL_H

// End of File

