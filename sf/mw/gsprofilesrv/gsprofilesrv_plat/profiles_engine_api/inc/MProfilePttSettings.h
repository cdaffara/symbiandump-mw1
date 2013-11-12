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
* Description:  Profile PTT settings interface.
*
*/



#ifndef MPROFILEPTTSETTINGS_H
#define MPROFILEPTTSETTINGS_H

//  INCLUDES
#include <e32def.h>
#include <ProfileInternal.hrh>

// CLASS DECLARATION

/**
*  Profile PTT settings interface.
*  @since 3.1
*/
class MProfilePttSettings
    {
    protected:  // Destructor

        virtual ~MProfilePttSettings() {};

    public: // New functions

        /**
        * Returns PTT ringing tone file name.
        * @since 3.1
        * @return Returns PTT ringing tone file name.
        */
        virtual const TDesC& PttRingingTone() const = 0;

        /**
         * Returns PTT status of the profile.
         * @since 3.1
         * @return Returns PTT status of the profile.
         */
        virtual TProfilePttStatus PttStatus() const = 0;

    };

#endif      //  MPROFILEPTTSETTINGS_H

// End of File

