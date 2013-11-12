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

#include "ASSrvEnvironmentChangeManager.h"

// System includes

// User includes
#include "ASSrvStaticUtils.h"
#include "ASSrvServerWideData.h"
#include "ASSrvSessionCollection.h"
#include "ASSrvEnvironmentChangeObserver.h"

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> CASSrvEnvironmentChangeManager (source)
//

//*************************************************************************************
CASSrvEnvironmentChangeManager::CASSrvEnvironmentChangeManager(CASSrvServerWideData& aServerWideData)
:	iServerWideData(aServerWideData)
	{
	}


//*************************************************************************************
CASSrvEnvironmentChangeManager::~CASSrvEnvironmentChangeManager()
	{
	iObservers.Close();
	delete iChangeNotifier;
	}


//*************************************************************************************
void CASSrvEnvironmentChangeManager::ConstructL()
	{
	CachedWorkDays() = TLocale().WorkDays();

#ifdef _DEBUG
	iEnvChgHandling = ETrue;
#endif	
	
	
	// The priority of the change notifier should also be above that of the CASSrvAlarmTimer
	// When the system time is moved forward, if the UI broadcasts multiple events, like locale change
	// then at the end of handling of the first locale event, the alarm's due time is calculated as the old time
	// and the timer is called with it. In the meantime if the system time is also changed then the timer
	// expires with KErrUnderflow which causes the old alarm to go off.
	iChangeNotifier = CEnvironmentChangeNotifier::NewL(CActive::EPriorityHigh+1, TCallBack(EnvironmentChangeCallBack, this));
	iChangeNotifier->Start();
	}


//*************************************************************************************
CASSrvEnvironmentChangeManager* CASSrvEnvironmentChangeManager::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvEnvironmentChangeManager* self = new(ELeave) CASSrvEnvironmentChangeManager(aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//


//*************************************************************************************
/**
 * Request environment change notifications
 */
void CASSrvEnvironmentChangeManager::RequestEnvironmentChangesL(MASSrvEnvironmentChangeObserver& aObserver)
	{
	User::LeaveIfError(iObservers.InsertInAddressOrder(&aObserver));
	}


//*************************************************************************************
/**
 * Cancel a previous change notification request
 */
void CASSrvEnvironmentChangeManager::RequestEnvironmentChangesCancel(MASSrvEnvironmentChangeObserver& aObserver)
	{
	TInt index = KErrNotFound;
	const TInt error = iObservers.FindInAddressOrder(&aObserver, index);
	if	(error != KErrNotFound)
		iObservers.Remove(index);
	}


//
//
//


//*************************************************************************************
/**
 * @internalComponent
 */
TBool CASSrvEnvironmentChangeManager::EnvironmentChangeCallBack(TAny* aSelf)
	{
	CASSrvEnvironmentChangeManager& self = *reinterpret_cast<CASSrvEnvironmentChangeManager*>(aSelf);
#ifdef _DEBUG	
	if(!self.iEnvChgHandling)
		{
		 return ETrue;
		}
#endif	
	self.HandleEnvironmentChange();
	return ETrue;
	}


//
//
//


//*************************************************************************************
/**
 * Handle an environment change event
 */
void CASSrvEnvironmentChangeManager::HandleEnvironmentChange()
	{
	 if	(!iHaveAbsorbedFirstChange)
		{
		iHaveAbsorbedFirstChange = ETrue;
		return;
		}

	const TInt changes = iChangeNotifier->Change();
	//
	TBool workDaysChanged = EFalse;
	if	(changes & EChangesLocale)
		{
		const TUint workdays = TLocale().WorkDays();
		if	(workdays != CachedWorkDays())
			{
			CachedWorkDays() = workdays;
			workDaysChanged = ETrue;
			}
		}

	// Cascade changes to observers
	const TInt count = iObservers.Count();
	for(TInt i=0; i<count; i++)
		{
		iObservers[i]->MEnvChangeHandleEvent(changes, CachedWorkDays(), workDaysChanged);
		}
			
	}










