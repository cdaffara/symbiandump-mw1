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
* Description:  Profile set PTT settings interface.
*
*/



#ifndef MPROFILESETPTTSETTINGS_H
#define MPROFILESETPTTSETTINGS_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Profile set PTT settings interface.
*  @since 3.1
*/
class MProfileSetPttSettings
    {
    protected:  // Destructor

        virtual ~MProfileSetPttSettings() {};

    public: // New functions

        /**
        * Set PTT ringing tone file name.
        * @since 3.1
        */
        virtual void SetPttRingingToneL( const TDesC& aPttRingingTone ) = 0;

        /**
         * Set PTT status of the profile.
         * @since 3.1
         */
        virtual void SetPttStatus( TProfilePttStatus aPttStatus ) = 0;

    };

#endif      //  MPROFILESETPTTSETTINGS_H

// End of File


