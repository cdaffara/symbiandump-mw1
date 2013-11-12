/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MediaKeys internal Publish & Subscribe keys.
*
*/


#ifndef __MEDIAKEYSINTERNALPSKEYS_H__
#define __MEDIAKEYSINTERNALPSKEYS_H__

const TUid KPSUidMediaKeysEventNotifier = {0x10207263};

// Media Keys Notification API

const TUint32 KMediaKeysVolumeKeyEvent = 0x00000001;
const TUint32 KMediaKeysCurrentVolume = 0x00000002;
const TUint32 KMediaKeysControlKeyEvent = 0x00000003;
const TUint32 KMediaKeysAccessoryVolumeEvent = 0x00000004;

#endif      // __MEDIAKEYSINTERNALPSKEYS_H__

