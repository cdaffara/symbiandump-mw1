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
* Description:  Set profile extra tones2 interface.
*
*/



#ifndef __MPROFILESETEXTRATONES2_H
#define __MPROFILESETEXTRATONES2_H

// INCLUDES
#include <e32std.h>
#include <MProfileSetExtraTones.h>

// CLASS DECLARATION

/**
*  Set profile extra tones2 interface.
*  This interface offers methods to set IM message alert tone file name.
*
*  @lib N/A
*  @since 10.1
*/
class MProfileSetExtraTones2 : public MProfileSetExtraTones
    {
    protected:  // Destructor

        virtual ~MProfileSetExtraTones2() {};

    public: // New functions
        
        /**
        * Set reminder tone.
        * @since 10.1
        * @param aReminderTone File name of the reminder tone.
        */
        virtual void SetReminderToneL( 
                const TDesC& aReminderTone ) = 0;
        
        /**
        * Set clock alarm tone.
        * @since 10.1
        * @param aClockAlarmTone File name of the clock alarm tone.
        */
        virtual void SetClockAlarmToneL( 
                const TDesC& aClockAlarmTone ) = 0;

    };


#endif      // __MPROFILESETEXTRATONES2_H

// End of File
