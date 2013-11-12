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
* Description:  Internal Publish&Subscribe definitions of the
*                Core Application UIs  subsystem
*
*/


#ifndef COREAPPLICATIONUISINTERNALPSKEYS_H
#define COREAPPLICATIONUISINTERNALPSKEYS_H

#include <coreapplicationuisdomainpskeys.h>

/*****************************************
*   Location Global Privacy Value
******************************************/
// Use TUid KUidSystemCategory = { 0x101f75b6 } 
const TInt KPSUidLocationGlobalPrivacyValue = 0x100052FA;
const TUid KPSUidLocationGlobalPrivacy={KPSUidLocationGlobalPrivacyValue};
enum EPSLocationGlobalPrivacy
    {
	EPSLocationGlobalPrivacyUninitialized = 0,
    EPSLocPrivAcceptAll,  
    EPSLocPrivRejectAll,
    EPSLocPrivAlwaysAsk,
    EPSLocPrivIndividualPrivacy,
    EPSLocPrivAcceptAllTimed,
    EPSLocPrivRejectAllTimed
    };

#endif      // COREAPPLICATIONUISINTERNALPSKEYS_H

// End of File

