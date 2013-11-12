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
* Description:  Abstract interface for handling the active profile events.
*
*/



#ifndef __MPROFILECHANGEOBSERVER_H
#define __MPROFILECHANGEOBSERVER_H

// INCLUDES
#include <e32std.h>

// DATA TYPES
enum TProfileEvent
    {
    EProfileNewActiveProfile = 0,
    EProfileActiveProfileModified,
    EProfileUnknownEvent = 0xFFFFFFFF
    };

// CLASS DECLARATION

/**
*  Abstract interface for handling the active profile events.
*  User derives his class from this and implements HandleActiveProfileEventL().
*
*  @lib N/A
*  @since 2.0
*/
class MProfileChangeObserver
    {
    public: // New functions

        /**
        * This is a callback function which is called when a active profile event
        * completes. User must derive his class from MProfileChangeObserver
        * and implement this method if he is to use notify services.
        * @since 2.0
        * @param aProfileEvent Profile event
        * @param aProfileId Active profile id
        */
        virtual void HandleActiveProfileEventL(
            TProfileEvent aProfileEvent, TInt aProfileId ) = 0;


    };

#endif      // __MPROFILECHANGEOBSERVER_H

// End of File
