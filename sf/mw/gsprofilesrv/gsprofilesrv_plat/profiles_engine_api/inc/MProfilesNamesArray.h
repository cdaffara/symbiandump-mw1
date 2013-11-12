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
* Description:  Profile names array interface.
*
*/



#ifndef MPROFILESNAMESARRAY_H
#define MPROFILESNAMESARRAY_H

//  INCLUDES
#include <bamdesca.h> // MDesCArray

// FORWARD DECLARATIONS
class MProfileName;

// CLASS DECLARATION

/**
*  Profile names array interface.
*
*  How to use:
*  Get profile names array with MProfileEngine::ProfilesNamesArrayLC().
*      MProfilesNamesArray* nameArray = iProfileEngine->ProfilesNamesArrayLC();
*
*
*  @lib N/A
*  @since 2.0
*/
class MProfilesNamesArray : public MDesCArray
    {
    public: // New functions

        /**
        * Return pointer of the profile name interface.
        * @since 2.0
        * @param aIndex index of the profile.
        * @return Return pointer of the profile name interface.
        *         Pointer is NULL if profile doesn't found.
        */
        virtual const MProfileName* ProfileName( TInt aIndex ) const = 0;

        /**
        * Find profile index by ID of the profile.
        * @since 2.0
        * @param aId ID of the profile.
        * @return Return index of the profile. KErrNotFound if
        *         profile doesn't found.
        */
        virtual TInt FindById( TInt aId ) const = 0;

        /**
        * Find profile index by name of the profile.
        * @since 2.0
        * @param aProfileName name of the profile.
        * @return Return index of the profile. KErrNotFound if
        *         profile doesn't found.
        */
        virtual TInt FindByName( const TDesC& aProfileName ) const = 0;

    };

#endif      // MPROFILESNAMESARRAY_H

// End of File
