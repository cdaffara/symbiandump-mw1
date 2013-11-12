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
* Description:   presence_avalability text
*
*/
#ifndef __AVABILITYTEXT_H
#define __AVABILITYTEXT_H

//availability text to be used with MPresenceBuddyInfo2::EBusy.
//changed status values to sync with icons ids available in the branddata
_LIT(KAwayState,"away");
_LIT(KOnPhoneState,"onmobile");
_LIT(KDndState,"dnd");

//availability text to be used with MPresenceBuddyInfo2::EUnknownAvailability.
_LIT(KInvisibleState,"offline");

//Extension key-value to be used with MPresenceBuddyInfo2.
//Extension Keys
_LIT(KExtensionKey,"ExtensionKey");

//Extension Values.
_LIT8(KPendingRequestExtensionValue,"pending");
_LIT8(KBlockedExtensionValue,"block");
_LIT8(KServiceExtensionValue,"ServiceOut");
_LIT8(KCallForwardExtensionValue,"CallForward");

/*
=========================Example For BUSY States =============================
MPresenceBuddyInfo2* buddyPresInfo = MPresenceBuddyInfo2::NewLC();

//This will set presence status as On phone
buddyPresInfo->SetAvailabilityL( MPresenceBuddyInfo2::EBusy,KOnPhoneState); 

//This will set presence status as Away
buddyPresInfo->SetAvailabilityL( MPresenceBuddyInfo2::EBusy,KAwayState); 

//This will set presence status as do not disture
buddyPresInfo->SetAvailabilityL( MPresenceBuddyInfo2::EBusy,KDndState); 

NOTE:: if TDesC& aAvailabilityText(second arg) is other then mentioned and 
	   TAvailabilityValues aAvailability is MPresenceBuddyInfo2::EBusy
	   then presence will be BUSY.
==============================================================================
*/

/*
=========================Example For INVISIBLE State===========================
MPresenceBuddyInfo2* buddyPresInfo = MPresenceBuddyInfo2::NewLC();

//This will set presence status as Invisible.
buddyPresInfo->SetAvailabilityL( MPresenceBuddyInfo2::EUnknownAvailability,KInvisibleState); 

NOTE:: if TDesC& aAvailabilityText(second arg) is other then mentioned and 
	   TAvailabilityValues aAvailability is MPresenceBuddyInfo2::EUnknownAvailability
	   then presence will be UNKNOWN.

==============================================================================
*/

/*
=========================Example For BLOCKED And PENDING State=================
MPresenceBuddyInfo2* buddyPresInfo = MPresenceBuddyInfo2::NewLC();

//This will set presence status as pending
buddyPresInfo->SetAnyFieldL( KExtensionKey, KPendingRequestExtensionValue ); 

//This will set presence status as blocked
buddyPresInfo->SetAnyFieldL( KExtensionKey, KBlockedExtensionValue );

NOTE:: if TDesC8& aValue(second arg) is other then mentioned then the(blocked)
and pending states will not be shown
provided the prnding and blocked state are not required at the smae time.
==============================================================================
*/

/*
=========================Example For ServiceOut And CallForward State=================
MPresenceBuddyInfo2* buddyPresInfo = MPresenceBuddyInfo2::NewLC();

//This will set presence status as CallForward
buddyPresInfo->SetAnyFieldL( KExtensionKey, KCallForwardExtensionValue ); 

//This will set presence status as ServiceOut
buddyPresInfo->SetAnyFieldL( KExtensionKey, KServiceOutExtensionValue );

NOTE:: if TDesC8& aValue(second arg) is other then mentioned then the(blocked)
and pending states will not be shown
provided the prnding and blocked state are not required at the smae time.
==============================================================================
*/
#endif      //__AVABILITYTEXT_H

//  End of File
