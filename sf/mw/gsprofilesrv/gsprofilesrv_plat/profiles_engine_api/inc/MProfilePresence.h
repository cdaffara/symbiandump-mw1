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
* Description:  Profile presence interface.
*
*/



#ifndef __MPROFILEPRESENCE_H
#define __MPROFILEPRESENCE_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Profile presence interface.
*  This interface offers methods to get status message and
*  availability attribute.
*
*  @lib N/A
*  @since 2.0
*/
class MProfilePresence
    {
    protected:  // Destructor

        virtual ~MProfilePresence() {};

    public: // New functions

        /**
        * Return status message of the profile.
        * @since 2.1
        * @return Status message of the profile.
        */
        virtual const TDesC& StatusMessage() const = 0;

        /**
        * Return availability attribute of the profile.
        * @since 2.1
        * @return Return availability attribute of the profile.
        */
        virtual TUint32 AvailabilityAttribute() const = 0;

    };

#endif      // __MPROFILEPRESENCE_H

// End of File
