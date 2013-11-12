/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*                Startup subsystem.
*
*/


#ifndef SECONDARY_DISPLAY_STARTUP_API_H
#define SECONDARY_DISPLAY_STARTUP_API_H

// INCLUDES
#include <e32base.h>
#include <w32std.h>

/*
* ==============================================================================
* This file contains the following sections:
*   - Dialog definitions
*   - Command definitions
*   - Event definitions
*   - Parameter definitions
* ==============================================================================
*/

namespace SecondaryDisplay
{
	
// The category UID for the messages in this header file.
//
const TUid KCatStartup = {0x100058F4};

/*
* ==============================================================================
* Dialogs shown by Startup subsystem. These messages are handled using the
* Secondary Display support in Avkon.
* ==============================================================================
*/
enum TSecondaryDisplayStartupDialogs
    {
    /**
    * A command for showing a welcome note on secondary display.
    *
    * @input    TWelcomeNotePckg          The user defined string for the 
    *                                     welcome note.
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */
    ECmdShowWelcomeNote = 1,
    
    /**
    * A command for showing the Offline mode query on secondary display.
    *
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */    
    ECmdShowOfflineQuery,
    
    /**
    * A command for showing the time query on secondary display.
    *
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */
    ECmdShowTimeQuery,

    /**
    * A command for showing the date query on secondary display.
    *
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */
    ECmdShowDateQuery,
    
    /**
    * A command for showing the city query on secondary display.
    *
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */
    ECmdShowCityQuery,
    
    /**
    * A command for showing a security query on secondary display.
    *
    * @input    TSecurityQueryPckg        The security query to be shown.
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */
    ECmdShowSecurityQuery,
    
    /**
    * A command for showing the unlock MMC query on secondary display.
    *
    * @output   TDialogStatusPckg         The status of the dialog operation.
    */
    ECmdShowMMCQuery,
    
    /**
    * A command for showing an error note on secondary display.
    *
    * @input    TStartupErrorNotePckg     The error note to be shown.
    * @output   TDialogStatusPckg         The status of the dialog operation.

    */
    ECmdShowErrorNote
    };
    
/*
* ==============================================================================
* Startup-related commands for Secondary Display.
* ==============================================================================
*/
enum TSecondaryDisplayStartupCommands
    {
    /**
    * A command for synchronizing various startup operations on both displays. 
    * Series 60 software issues this command when starting some operation that 
    * needs to be synced.
    *
    * @input    TStartupSyncCmdPckg       The operation that needs to be synced.
    * @output   TStatusPckg               The status of the sync operation.
    */
    ECmdStartupSync = 101,
    
    /**
    * A command for skipping the welcome tone played with welcome animation.
    *
    * @input    TStartupPhaseSkipCmdPckg  The operation that needs to be skipped.

    * @output   TStatusPckg               The status of the skipping operation.
    */
    ECmdStartupPhaseSkip
    };

/*
* ==============================================================================
* Startup-related events for Secondary Display.
* ==============================================================================
*/
enum TSecondaryDisplayStartupEvents
    {
    /**
    * A message notifying about an event at startup.
    *
    * @input    TStartupEventPckg         The startup event.
    */
    EMsgStartupEvent = 201,

    /**
    * A message showing a welcome image.
    *
    * @input    TWelcomeImagePckg       The welcome image path.
    */
    EMsgWelcomeImageEvent
    };

    
/*
* ==============================================================================
* Parameter definitions for the messages in this file.
* ==============================================================================
*/

// General status parameter package.
//
typedef TPckgBuf<TInt> TStatusPckg;

// Dialog status parameter package.
//
typedef TPckgBuf<TKeyEvent> TDialogStatusPckg;

// Parameter definitions for ECmdShowWelcomeNote
// 
const TInt KWelcomeNoteSize = 50; // 50 characters
typedef TBuf<KWelcomeNoteSize> TWelcomeNote;
typedef TPckgBuf<TWelcomeNote> TWelcomeNotePckg;

// Parameter definitions for EMsgWelcomeImageEvent
// 
typedef TBuf<KMaxFileName> TWelcomeImage;
typedef TPckgBuf<TWelcomeImage> TWelcomeImagePckg;

// Parameter definitions for ECmdShowSecurityQuery
//
enum TSecurityQuery
    {
    EShowPIN1,
    EShowPIN2,
    EShowPUK1,
    EShowPUK2,
    EShowUPIN,
    EShowUPUK,
    EShowSecCode
    };
typedef TPckgBuf<TSecurityQuery> TSecurityQueryPckg;

// Parameter definitions for ECmdShowErrorNote
//
enum TErrorNote
    {
    EContactService,
    EInsertSim,
    ESimRejected,
    ESimUnaccepted,
    ESimRestrictonPending,
    ESimRestrictonOn,
    EApplicationError
    };
typedef TPckgBuf<TErrorNote> TErrorNotePckg;

// Parameter definitions for ECmdStartupSync
//
enum TStartupSyncCmd
    {
    EStartWelcomeAnimation,  // Used for starting welcome animation and sound 
                             // synchronously with Secondary Display SW.
    EStartOperatorAnimation, // Used for starting operator animation and sound 
                             // synchronously with Secondary Display SW.
    EStartShutdownAnimation, // Used for starting shutdown animation and sound 
                             // synchronously with Secondary Display SW.
    EStartStartupReady       // Used for showing S60 Idle and CoverUI Idle
                             // synchronously.
    };
typedef TPckgBuf<TStartupSyncCmd> TStartupSyncCmdPckg;

// Parameter definitions for ECmdStartupPhaseSkip
//
enum TStartupPhaseSkipCmd
    {
    ESkipStartupAnim, // Skip the startup animation.
    ESkipShutdownAnim // Skip the shutdown animation.
    };
typedef TPckgBuf<TStartupPhaseSkipCmd> TStartupPhaseSkipCmdPckg;

// Parameter definitions for EMsgStartupEvent
//
enum TStartupEvent
    {
    EStartupAnimSkipped, // The startup animation has been skipped.
    EShutdownAnimSkipped, // The shutdown animation has been skipped.
    EShowCountryQuery, // Show country query.
    EShowCityQuery // Show city query.
    };
typedef TPckgBuf<TStartupEvent> TStartupEventPckg;

} // namespace SecondaryDisplay

#endif      // SECONDARY_DISPLAY_STARTUP_API_H
            
// End of File
