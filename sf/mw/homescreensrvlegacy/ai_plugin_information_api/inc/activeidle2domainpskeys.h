/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/


#ifndef ACTIVEIDLE2_DOMAIN_PS_KEYS_H
#define ACTIVEIDLE2_DOMAIN_PS_KEYS_H

/**
* PubSub Category AI information API
*/
const TUid KPSUidAiInformation = {0x101FD657};

/**
* PubSub keys
*/
const TUint KActiveIdleUid = 0x00000001; // Contains UID of StandbyUI (Active Idle) application

const TUint KActiveIdleState = 0x00000002; // Contains one value from following emuneration

enum EPSActiveIdleState
{
    // Indicates that Active Idle goes to background
    // Value is triggered by KAknFullOrPartialForegroundLost event
    //
    // Corresponds to value EPSTelephonyNotIdle writtern to former  KTelephonyIdleStatus
    EPSAiBackground = 0,

    // Indicates that Active Idle goes to foreground
    // Value is triggered by KAknFullOrPartialForegroundGained event
    //
    // Corresponds to value EPSTelephonyIdle writtern to former  KTelephonyIdleStatus
    EPSAiForeground,

    // Indicates that Active Idle goes to background and keyboard focus goes to Phone app for Tel number entry
    // The key is managed by windows server plug-in depends on the key pressed by end-user (See Chapter "Key event handling" from Active Idle SWAD)
    //
    // Corresponds to value EPSTelephonyIncomingKeyEvents written to former KTelephonyFocusInfo
    EPSAiNumberEntry
};

const TUint KActiveIdlePopupState = 0x00000003; // Contains one value from following emuneration

enum EPSActiveIdlePopupState
{
    // Indicates that Active Idle is displaying dialog or menu
    // The key is managed by windows server plug-in to determine whether to enable keylock handling
    EPSAiDisplayingMenuOrDialog,

    // Indicates that Active Idle is not displaying dialog or menu
    // The key is managed by windows server plug-in to determine whether to enable keylock handling
    EPSAiNotDisplayingMenuOrDialog
};


const TUint KActiveIdleActOnSendKey = 0x00000004; // Contains 0 if we are to ignore the send or > 0 if we need to react to it

const TUint KActiveIdleTouchToolbarWidth = 0x00000005;

const TUint KActiveIdleTouchToolbarHeight = 0x00000006;

const TUint KActiveIdleSimRegFailedReceived = 0x00000007; // Contains 1 if we have received sim reg failed message, 0 if not.

enum EPSActiveIdleSimRegReceiveStatus
{
    // Indicates that Active Idle is displaying dialog or menu
    // The key is managed by windows server plug-in to determine whether to enable keylock handling
    EPSSimRegFailedMessageNotReceived = 0,

    // Indicates that Active Idle is not displaying dialog or menu
    // The key is managed by windows server plug-in to determine whether to enable keylock handling
    EPSSimRegFailedMessageReceived
};

const TUint KActiveIdleThemeSupportsXsp = 0x00000008;   // Contains one value from following enumeration

enum EPSActiveIdleThemeSupportsXsp
{
    // Value indicates that xSP feature is not supported in currently active theme
    EPSAiXspNotSupported,

    // Value indicates that the current theme supports the xSP feature
    EPSAiXspIsSupported
};

const TUint KActiveIdleLaunch = 0x00000009; // Contains information if shortcut launching is ongoing or not

enum EPSActiveIdleIdleLaunch
{

	// Value indicates that no shortcut is in launching state and new launch tapping can be handled
	EPSAiLaunchNotActive = 0,
	
	// Value indicates that active idle shortcut is launching
	EPSAiLaunchIsActive	
};


// Indicates that Active Idle 2 should be restarted.
const TUint KActiveIdleRestartAI2 = 0x0000000B;

// Indicates that all the CPS Harvester plugins have been updated
const TUint KActiveIdleCpsPluginsUpdated = 0x0000000C;

enum EPSActiveIdleCpsPluginsUpdated
    {
    EPSAiPluginsNotUpdated = 0,
    EPSAiPluginsUpdated
    };

// Key to indicate wether the WS plug-in forwards all the numeric keypad 
// events to telephone or not.
const TUint KActiveIdleForwardNumericKeysToPhone = 0x0000000D;

enum EActiveIdleForwardNumericKeysToPhone
    {
    // Forward events to phone
    EPSAiForwardNumericKeysToPhone,
    
    // Don't forward events to phone
    EPSAiDontForwardNumericKeysToPhone
    };

// Indicates that in ExtHS mode the LSK should have a locked shortcut.
// Contains the full shortcut definition string.
const TUint KActiveIdleExtHS_LSKLocked = 0x00000501;

// Indicates that in ExtHS mode the RSK should have a locked shortcut.
// Contains the full shortcut definition string.
const TUint KActiveIdleExtHS_RSKLocked = 0x00000502;

// Indicates that in ExtHS mode the plugin configuration has changed.
const TUint KActiveIdleExtHS_PluginConfChange = 0x00000503;

/**
* PubSub Category AI plug-in registry API
*/
const TUid KPSUidActiveIdle2 =  {0x102750F0}; // ActiveIdle2 SID

/**
*
* First iterate Active plugin UID range to find all
* active plugin UID's. Use that UID as a key to find the plugins name
* from the name range.
*
*/

/**
* Active plugin count
*
* Possible integer values:
* 0x000000000 - 0xFFFFFFFF : Active plugin count
*/
const TUint KAIActivePluginCount = 0x00000000;

/**
* Active plugin UID range
*
* Possible integer values:
* 0x100000000 - 0xFFFFFFFF : Active plugins UID
*/
const TUint KAIActivePluginRangeStart = 0x00000001;
const TUint KAIActivePluginRangeEnd   = 0x0FFFFFFF;

/**
* Active plugin name range
*
* Possible string values:
* Plugin name
*/
const TUint KAIPluginNameRangeStart = 0x10000000;
const TUint KAIPluginNameRangeEnd   = 0xFFFFFFFF;

#endif // ACTIVEIDLE2_DOMAIN_PS_KEYS_H
