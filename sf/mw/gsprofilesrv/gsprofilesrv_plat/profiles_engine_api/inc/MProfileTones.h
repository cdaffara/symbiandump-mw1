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
* Description:  Profile tones interface.
*
*/



#ifndef __MPROFILETONES_H
#define __MPROFILETONES_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
struct TProfileToneSettings;

// CLASS DECLARATION

/**
*  Profile tones interface.
*  This interface offers methods to get line 1 and 2 ringing tone,
*  message alert tone and tone settings.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileTones
    {
    protected:  // Destructor

        virtual ~MProfileTones() {};

    public:

        /**
        * Returns line 1 ringing tone file name.
        * @since 2.0
        * @return Returns line 1 ringing tone file name.
        */
        virtual const TDesC& RingingTone1() const = 0;

        /**
        * Returns line 2 ringing tone file name.
        * @since 2.0
        * @return Returns line 2 ringing tone file name.
        */
        virtual const TDesC& RingingTone2() const = 0;

        /**
        * Returns message alert tone file name.
        * @since 2.0
        * @return Returns message alert tone file name.
        */
        virtual const TDesC& MessageAlertTone() const = 0;

        /**
        * Returns tone settings structure.
        * @since 2.0
        * @return Returns tone settings structure.
        */
        virtual const TProfileToneSettings& ToneSettings() const = 0;
    };

#endif      // __MPROFILETONES_H

// End of File
