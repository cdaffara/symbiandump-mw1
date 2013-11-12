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
* Description:  Profile extra tones2 interface.
*
*/



#ifndef __MPROFILEEXTRATONES2_H
#define __MPROFILEEXTRATONES2_H

// INCLUDES
#include <e32std.h>
#include <MProfileExtraTones.h>

// CLASS DECLARATION

/**
*  Profile extra tones interface.
*  This interface offers method to get IM message alert tone file name.
*
*  @lib N/A
*  @since 10.1
*/
class MProfileExtraTones2 : public MProfileExtraTones
    {
    protected:  // Destructor

        virtual ~MProfileExtraTones2() {};

    public:       
        /**
        * Returns reminder tone file name.
        * @since 10.1
        * @return Returns reminder tone file name.
        */
        virtual const TDesC& ReminderTone() const = 0;
        
        /**
        * Returns clock alarm tone file name.
        * @since 10.1
        * @return Returns clock alarm tone file name.
        */
        virtual const TDesC& ClockAlarmTone() const = 0;
    };


#endif      // __MPROFILEEXTRATONES2_H

// End of File
