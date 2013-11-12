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
* Description:  Common defines for this component.
*
*/




#ifndef _XMPPSETTINGSAPICOMMON_H
#define _XMPPSETTINGSAPICOMMON_H

#include <e32base.h>

_LIT( KColSettingId, "XMPP-SETTING-ID-COL");
const TUid  KXmppSettingsApiCenRep         ={0x1020E334};
// keys
const TUint32 KXmppKeyColumnTable       = 0x00000000;
const TUint32 KXmppKeyDataTable         = 0x01000000;
const TUint32 KCurrentSettingIdIndex    = 0xFFFFFFFF;

//masks
const TUint32 KXmppMaskColumns          = 0xFFFF0000;
const TUint32 KXmppMaskAllRows          = 0xFF00FFFF;

#endif // _XMPPSETTINGSAPICOMMON_H
