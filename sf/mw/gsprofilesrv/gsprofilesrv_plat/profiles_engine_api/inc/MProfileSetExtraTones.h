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
* Description:  Set profile extra tones interface.
*
*/



#ifndef __MPROFILESETEXTRATONES_H
#define __MPROFILESETEXTRATONES_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Set profile extra tones interface.
*  This interface offers methods to set IM message alert tone file name.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileSetExtraTones
    {
    protected:  // Destructor

        virtual ~MProfileSetExtraTones() {};

    public: // New functions

        /**
        * Set IM message alert tone file name.
        * @since 2.0
        * @param aIMMessageAlertTone IM message alert tone.
        */
        virtual void SetIMMessageAlertToneL(
            const TDesC& aIMMessageAlertTone ) = 0;

        /**
        * Set Email alert tone file name.
        * @since 2.0
        * @param aEmailAlertTone Email alert tone.
        */
        virtual void SetEmailAlertToneL(
            const TDesC& aEmailAlertTone ) = 0;

        /**
        * Set Video call ringing tone file name.
        * @since 2.6
        * @param aRingingTone Video call ringing tone file name.
        */
        virtual void SetVideoCallRingingToneL(
            const TDesC& aRingingTone ) = 0;

    };


#endif      // __MPROFILESETEXTRATONES_H

// End of File
