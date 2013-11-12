/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The set of system state -related messages provided to 
*                Secondary Display software.
*
*/


#ifndef SECONDARY_DISPLAY_SYSTEM_STATE_API_H
#define SECONDARY_DISPLAY_SYSTEM_STATE_API_H

// INCLUDES
#include <e32base.h>

/*
* ==============================================================================
* This file contains the following sections:
*   - Command definitions
*   - Event definitions
*   - Parameter definitions
* ==============================================================================
*/

namespace SecondaryDisplay
{

// The category UID for the messages in this header file.
//
const TUid KCatSystemState = {0x102750B8};
  
/*
* ==============================================================================
* System state -related commands for Secondary Display.
* ==============================================================================
*/
enum TSecondaryDisplaySystemStateCommands
    {
    /**
    * A command for getting the system state.
    *
    * @output   TSystemStatePckg          The system state.
    */
    ECmdGetSystemState = 0x0001,
    
    /**
    * A command for getting the autolock status.
    *
    * @output   TAutolockStatusPckg       The autolock status.
    */
    ECmdGetAutolockStatus,
    
    /**
    * A command for getting the keyguard status.
    *
    * @output   TKeyGuardStatusPckg       The keyguard status.
    */
    ECmdGetKeyGuardStatus
    };

/*
* ==============================================================================
* System state -related events for Secondary Display.
* ==============================================================================
*/
enum TSecondaryDisplaySystemStateEvents
    {
    /**
    * A message notifying about a change in system state.
    *
    * @input    TSystemStatePckg          The new system state.
    */
    EMsgSystemStateChange,
    
    /**
    * A message notifying about a change in autolock status.
    *
    * @input    TAutolockStatusPckg       The autolock status.
    */
    EMsgAutolockEvent,
    
    /**
    * A message notifying about a change in keyguard status.
    *
    * @input   TKeyGuardStatusPckg       The keyguard status.
    */
    EMsgKeyGuardEvent
    };

    
/*
* ==============================================================================
* Parameter definitions for the messages in this file.
* ==============================================================================
*/

// Parameter definitions for EMsgSystemStateChange and EMsgSystemStateChange
//
enum TSystemState
    {
    ESystemStateNormal,
    ESystemStateCharging,
    ESystemStateAlarm,
    ESystemStateOffline,
    ESystemStateTest
    };
typedef TPckgBuf<TSystemState> TSystemStatePckg;

// Parameter definitions for ECmdGetAutolockStatus and EMsgAutolockEvent
//
enum TAutolockStatus
    {
    EAutolockOn,
    EAutolockOff
    };
typedef TPckgBuf<TAutolockStatus> TAutolockStatusPckg;

// Parameter definitions for ECmdGetKeyGuardStatus and EMsgKeyGuardEvent
//
enum TKeyGuardStatus
    {
    EKeyGuardOn,
    EKeyGuardOff
    };
typedef TPckgBuf<TKeyGuardStatus> TKeyGuardStatusPckg;

} // namespace SecondaryDisplay

#endif      // SECONDARY_DISPLAY_SYSTEM_STATE_API_H
            
// End of File
