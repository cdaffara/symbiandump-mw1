/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The set of messages provided to Secondary Display software by
*              : Profiles subsystem.
*
*/

#ifndef __SECONDARYDISPLAYPROFILESAPI_H
#define __SECONDARYDISPLAYPROFILESAPI_H

// INCLUDES
#include <e32base.h>

/*
* ==============================================================================
* This file contains the following sections:
*   - Dialog definitions
*   - Command definitions
*   - Event definitions
*   - Parameter definitions
* ==============================================================================
*/

// The category UID for the messages in this header file.
//
const TUid KCatProfiles = {0x2000B471}; 

/*
* ==============================================================================
* Dialogs shown by Profiles subsystem. These messages are handled using the
* Secondary Display support in Avkon.
* ==============================================================================
*/
enum TSecondaryDisplayProfilesDialogs
    {
    /**
    * No note. Error condition if this comes to CoverUI
    */
    ECmdNoNoteProfiles = 0,
    
    /**
    * A command for showing the "Timed profile expired" note on secondary display.
    */
    ECmdShowTimedProfileExpiryNote,
 
    };
#endif      // __SECONDARYDISPLAYPROFILESAPI_H
            
// End of File
