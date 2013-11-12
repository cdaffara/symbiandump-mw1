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

#include "tzswiobserver.h"

CTzSwiObserver* CTzSwiObserver::NewL()
	{
	CTzSwiObserver* self = new(ELeave) CTzSwiObserver();
	return self;
	}

CTzSwiObserver::~CTzSwiObserver()
	{
	iObservers.Close();
	}

CTzSwiObserver::CTzSwiObserver()
	{
	}

void CTzSwiObserver::AddObserverL(MTzDataObserver* aObserver)
	{
	User::LeaveIfError(iObservers.Append(aObserver));
	}

void CTzSwiObserver::RemoveObserver(MTzDataObserver* aObserver)
	{
	TInt j = 0;
	TInt jEnd = iObservers.Count();
	while (j < jEnd)
		{
		if (iObservers[j] == aObserver)
			{
			iObservers.Remove(j);
			break;
			}
		++j;
		}
	}

void CTzSwiObserver::BeginL()
	{
	iReadOnlyTzDbChanged = EFalse;
	iResourcesChanged = EFalse;
	}

void CTzSwiObserver::FileChangedL(RTz::TSWIObserverFilterIndex aType)
	{
	if (aType == RTz::EFilterTzPrivate)
		{
		iReadOnlyTzDbChanged = ETrue;
		}
	else if (aType == RTz::EFilterResourceTimezonelocalization)
		{
		iResourcesChanged = ETrue;
		}
	}

void CTzSwiObserver::EndL()
	{
	TInt j = 0;
	TInt end = iObservers.Count();
	while (j < end)
		{
		if (iReadOnlyTzDbChanged)
			{
			iObservers[j]->NotifyTZDataStatusChangeL(RTz::ETZDatabaseChanged);
			}
		if (iResourcesChanged)
			{
			iObservers[j]->NotifyTZDataStatusChangeL(RTz::ETZLocalizationDataChanged);
			}
		++j;
		}
	iReadOnlyTzDbChanged = EFalse;
	iResourcesChanged = EFalse;
	}
