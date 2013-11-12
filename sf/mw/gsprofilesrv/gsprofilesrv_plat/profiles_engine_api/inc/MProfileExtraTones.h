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
* Description:  Profile extra tones interface.
*
*/



#ifndef __MPROFILEEXTRATONES_H
#define __MPROFILEEXTRATONES_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Profile extra tones interface.
*  This interface offers method to get IM message alert tone file name.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileExtraTones
    {
    protected:  // Destructor

        virtual ~MProfileExtraTones() {};

    public:

        /**
        * Returns IM message alert tone file name.
        * @since 2.0
        * @return Returns IM message alert tone file name.
        */
        virtual const TDesC& IMMessageAlertTone() const = 0;

        /**
        * Returns Email alert tone file name.
        * @since 2.0
        * @return Returns Email alert tone file name.
        */
        virtual const TDesC& EmailAlertTone() const = 0;

        /**
        * Returns Video call ringing tone file name.
        * @since 2.6
        * @return Returns Video call ringing tone file name.
        */
        virtual const TDesC& VideoCallRingingTone() const = 0;

    };


#endif      // __MPROFILEEXTRATONES_H

// End of File
