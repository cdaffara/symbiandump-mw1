// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASSRVSYSTEMSTATE_H__
#define __ASSRVSYSTEMSTATE_H__

#include <ssm/ssmstateawaresession.h>
#include <e32property.h>

#include "assrvsystemstateobserver.h"

class CASSrvAlarmQueue;

/** A class to listen to changes in system state, such as shutdown events.
Observers can request notification of these events so that they can handle them.
*/
class CASSrvSystemState : public CActive
	{
public:
	static CASSrvSystemState* NewL();
	~CASSrvSystemState();
	
	void RequestNotificationL(MASSrvSystemStateObserver& aObserver);
	void RequestNotificationCancel(MASSrvSystemStateObserver& aObserver);
		
private:
	CASSrvSystemState();
	void ConstructL();
	
	TBool GetState(MASSrvSystemStateObserver::TState& aState);
	void AcknowledgeAndRequestStateNotification();
	
private: // from CActive
	void RunL();
	void DoCancel();
	
private:
	RSsmStateAwareSession iStateAwareSession;
	
	RPointerArray<MASSrvSystemStateObserver> iObservers;
	RProperty iWorkAroundProperty;
	RProperty iAcknowledgement;
	TBool iWorkAround;
	};

#endif
