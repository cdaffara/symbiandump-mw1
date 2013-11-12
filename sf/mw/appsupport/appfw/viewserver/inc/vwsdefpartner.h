// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __VWSDEF_partner_H__
#define __VWSDEF_partner_H__

#ifndef __VWSDEF_H__
#include <vwsdef.h>
#endif


#include <e32std.h>

//class defined
class TVwsViewEvent;

//class referenced
class TVwsViewId;

/**
@internalTechnology
@released
*/
#define KNullViewId TVwsViewId(KNullUid,KNullUid)

/**
@internalTechnology
@released
*/
const TInt KErrViewWrongMode=1; // !!! Wrong error code - should be -ve & unique


/**
@publishedPartner
@released
*/
#define KUidCustomCrossCheckValue 0x80282748

/**
@publishedPartner
@released
*/
const TUid KUidCustomCrossCheck={KUidCustomCrossCheckValue};

class TVwsViewEvent
/**
The TVwsViewEvent class encapsulates the attributes of a view server event.
These are:
- The event type
- The view ids of the two views associated with the event
- The attributes of any message which can go with the event.

@publishedPartner
@released 
*/
	{
public:
	enum TVwsViewEventType
		{
		EVwsActivateView,
		EVwsDeactivateView,
		EVwsScreenDeviceChanged,
		EVwsDeactivationNotification,
		EVwsActivationNotification,
		EVwsDeactivateViewDifferentInstance
		};
public:
	inline TVwsViewEvent();
	inline TVwsViewEvent(TVwsViewEventType aEventType);
	inline TVwsViewEvent(TVwsViewEventType aEventType,const TVwsViewId& aViewOneId);
	inline TVwsViewEvent(TVwsViewEventType aEventType,const TVwsViewId& aViewOneId,const TVwsViewId& aViewTwoId);
	inline TVwsViewEvent(TVwsViewEventType aEventType,const TVwsViewId& aViewOneId,const TVwsViewId& aViewTwoId,TUid aCustomMessageId,TInt aCustomMessageLength);
public:
	/**
	The event type.
	*/
	TVwsViewEventType iEventType;
	/**
	The first view id associated with the event. This has a different meaning for each of the event types:
	- For ActivateView events, it is the view to activate
	- For DeactivateView events, it is the view to deactivate
	- For ScreenDeviceChange events, it is the id of the currently active view
	- For ActivationNotification events, it is the id of the activated view
	- For DeactivationNotification events, it is the id of the deactivated view
	*/
	TVwsViewId iViewOneId;
	/**
	The second view id associated with the event. This has a different meaning for each of the event types:
	- For ActivateView events, it is the view that will be deactivated
	- For DeactivateView events, it is the view that has been activated
	- For ScreenDeviceChange events, it is unused
	- For ActivationNotification events, it is the view that will be deactivated
	- For DeactivationNotification events, it is the id of the view that has been activated
	*/
	TVwsViewId iViewTwoId;
	/**
	The type of message being passed in the custom message buffer
	*/
	TUid iCustomMessageId;
	/**
	The length of the custom message being held by the server.
	To retrieve the message itself from the server, a buffer of this length must be allocated
	*/
	TInt iCustomMessageLength;
	};

inline TVwsViewEvent::TVwsViewEvent() {}

inline TVwsViewEvent::TVwsViewEvent(TVwsViewEventType aEventType)
	: iEventType(aEventType),iViewOneId(),iViewTwoId(),iCustomMessageId(KNullUid),iCustomMessageLength(0) {}

inline TVwsViewEvent::TVwsViewEvent(TVwsViewEventType aEventType,const TVwsViewId& aViewOneId)
	: iEventType(aEventType),iViewOneId(aViewOneId),iViewTwoId(),iCustomMessageId(KNullUid),iCustomMessageLength(0) {}

inline TVwsViewEvent::TVwsViewEvent(TVwsViewEventType aEventType,const TVwsViewId& aViewOneId,const TVwsViewId& aViewTwoId)
	: iEventType(aEventType),iViewOneId(aViewOneId),iViewTwoId(aViewTwoId),iCustomMessageId(KNullUid),iCustomMessageLength(0) {}

inline TVwsViewEvent::TVwsViewEvent(TVwsViewEventType aEventType,const TVwsViewId& aViewOneId,const TVwsViewId& aViewTwoId,TUid aCustomMessageId,TInt aCustomMessageLength)
	: iEventType(aEventType),iViewOneId(aViewOneId),iViewTwoId(aViewTwoId),iCustomMessageId(aCustomMessageId),iCustomMessageLength(aCustomMessageLength) {}


#endif
