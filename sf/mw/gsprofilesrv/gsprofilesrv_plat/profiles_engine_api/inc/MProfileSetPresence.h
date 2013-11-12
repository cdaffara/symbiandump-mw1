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
* Description:  Set profile presence settings interface.
*
*/



#ifndef __MPROFILESETPRESENCE_H
#define __MPROFILESETPRESENCE_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Set profile presence settings interface.
*  This interface offers methods to set profile status message and
*  availability attribute.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileSetPresence
    {
    protected:  // Destructor

        virtual ~MProfileSetPresence() {};

    public: // New functions

        /**
        * Set status message of the profile.
        * @since 2.0
        * @param aStatusMessage status message of the profile.
        */
        virtual void SetStatusMessageL( const TDesC& aStatusMessage ) = 0;

        /**
        * Set availability attribute of the profile.
        * @since 2.0
        * @param aAvailabilityAttribute Availability attribute of the profile.
        */
        virtual void SetAvailabilityAttribute(
            const TUint32 aAvailabilityAttribute ) = 0;
    };

#endif      // __MPROFILESETPRESENCE_H

// End of File
