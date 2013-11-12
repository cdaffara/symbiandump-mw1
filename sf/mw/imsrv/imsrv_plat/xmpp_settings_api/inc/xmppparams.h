/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This header holds the various xmpp params/fields
*
*/

#ifndef __XMPPPARAMS_H
#define __XMPPPARAMS_H

#include <e32base.h>
namespace XmppParams
    {
      /** Presence status */
        enum TPresenceStatus
            {
            /** Unavailable */
            EUnavailable,
            /** Available */
            EAvailable,
            /** Chat */
            EChat,
            /** Away */
            EAway,
            /** Extended away */
            EXAway,
            /** Do not disturb */
            EDoNotDisturb,
            /** Invisible */
            EInvisible
            };

    // TInt
    _LIT( KXmppParamSnapId,                         "SNAP-ID");
    _LIT( KXmppParamServerPort,                     "SERVER-ADDRESS_PORT");
    _LIT( KXmppParamMsgToneVolume,                  "MSG-TONE_VOLUME");
    _LIT( KXmppParamSipProfileId,                   "VOIP-PROFILE-ID");
    _LIT( KXmppParamVoipProfileId,                  "SIP-PROFILE-ID");
    // TBool == TInt
    _LIT( KXmppParamPublishOnThePhoneState,         "PUBLISH-ON-THE-PHONE-STATUS");
    _LIT( KXmppParamAutoAcceptBuddyReq,             "ACCEPT-BUDDY-REQ-AUTO-ENABLED");
    // others == TInt
    _LIT( KXmppParamPresenceStatus,                 "PRESENCE-STATUS");
    _LIT( KXmppParamConSecurityOption,              "CONNECTION-SECURITY-OPTION");
    // TDesC16
    _LIT( KXmppParamMsgTonePathFile,                "MESSAGE-TONE-PATH-FILE");
    _LIT( KXmppParamXmppServerAddress,              "XMPP-SERVER-ADDRESS");
    _LIT( KXmppParamDomain,                         "XMPP-DOMAIN");
    _LIT( KXmppParamUsername,                       "XMPP-USERNAME");
    _LIT( KXmppParamPassword,                       "XMPP-PASSWORD");
    _LIT( KXmppParamPresenceStatusFreeTextOnline,         "PRESENCE-STATUS-FREE-TEXT-ONLINE");
    _LIT( KXmppParamPresenceStatusFreeTextAway,         "PRESENCE-STATUS-FREE-TEXT-AWAY");
    _LIT( KXmppParamPresenceStatusFreeTextBusy,         "PRESENCE-STATUS-FREE-TEXT-BUSY");    
    _LIT( KXmppParamPresenceStatusFreeTextOffline,         "PRESENCE-STATUS-FREE-TEXT-OFFLINE");
    _LIT( KXmppParamPresenceStatusFreeTextInvisible,    "PRESENCE-STATUS-FREE-TEXT-INVISIBLE"); 
    _LIT( KXmppParamLastLoggedInUserName,              "XMPP-LAST-LOGGEDIN-USERNAME");
    // gabble specific things


	_LIT( KXmppParamResource,                		"RESOURCE-FILE");
	_LIT( KXmppParamOldSSLRequired,                 "OLD_SSL_REQUIRED");
	_LIT( KXmppParamRegisterRequired,               "REGISTER-REQUIRED");
	_LIT( KXmppParamHttpsProxyServerPort,           "HTTPS-PROXY-SERVER-ADDRESS_PORT");
	_LIT( KXmppParamHttpsProxyServerAddress,        "HTTPS-PROXY-SERVER-ADDERES");  
	_LIT( KXmppParamAlais,                   		"ALIAS");
    _LIT( KXmppParamSnapName,                       "SNAP-NAME");
    _LIT( KXmppParamSettingsName,                   "SETTINGS-NAME");
	
	//Connection manager related    
    _LIT( KXmppParamConnMgrBus,                   	"CONNMGR_BUS");
    _LIT( KXmppParamConnMgrPath,                  	"CONNMGR_PATH");
    _LIT( KXmppParamProtocol,           	      	"PROTOCOL");

    }
#endif