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
* Description:  Set tone settings interface.
*
*/



#ifndef __MPROFILESETTONES_H
#define __MPROFILESETTONES_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
struct TProfileToneSettings;

// CLASS DECLARATION

/**
*  Set tone settings interface.
*  This interface offers methods to set line 1 and 2 ringing tone,
*  message alert tone and tone settings.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileSetTones
    {
    protected:  // Destructor

        virtual ~MProfileSetTones() {};

    public: // New functions

        /**
        * Set line 1 ringing tone.
        * @since 2.0
        * @param aRingingTone File name of the ringing tone.
        */
        virtual void SetRingingTone1L( const TDesC& aRingingTone ) = 0;

        /**
        * Set line 2 ringing tone.
        * @since 2.0
        * @param aRingingTone File name of the ringing tone.
        */
        virtual void SetRingingTone2L( const TDesC& aRingingTone ) = 0;

        /**
        * Set message alert tone.
        * @since 2.0
        * @param aMessageAlertTone File name of the message alert tone.
        */
        virtual void SetMessageAlertToneL(
            const TDesC& aMessageAlertTone ) = 0;

        /**
        * Set tone settings of the profile.
        * @since 2.0
        * @return Tone settings structure.
        */
        virtual TProfileToneSettings& SetToneSettings() = 0;

    };

#endif      // __MPROFILESETTONES_H

// End of File
