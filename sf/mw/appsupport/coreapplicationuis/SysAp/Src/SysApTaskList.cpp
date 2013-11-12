/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "SysApTaskList.h"
#include <apgwgnam.h>

CSysApTaskList* CSysApTaskList::NewL(RWsSession& aWsSession)
	{
	CSysApTaskList* self = NewLC(aWsSession);
	CleanupStack::Pop(self);
	return self;
	}

 CSysApTaskList* CSysApTaskList::NewLC(RWsSession& aWsSession)
	{
	CSysApTaskList* self = new(ELeave) CSysApTaskList(aWsSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSysApTaskList::CSysApTaskList(RWsSession& aWsSession)
: iWs(aWsSession)
	{
	}

void CSysApTaskList::ConstructL()
	{
	UpdateListL();
	}

 CSysApTaskList::~CSysApTaskList()
	{
	iWgs.Close();
	}

 void CSysApTaskList::UpdateListL()
	{
	User::LeaveIfError(iWs.WindowGroupList(0, &iWgs));
	}

 const RArray<RWsSession::TWindowGroupChainInfo>& CSysApTaskList::WgArray() const
	{
	return iWgs;
	}

 TApaTask CSysApTaskList::FindRootApp(TUid aAppUid) const
	{
	TApaTask task(iWs);
	task.SetWgId(0);		// initialise task to non-existant task
	// wgId = 0 tells FindAppByUid to start looking for apps
	TInt wgId=0;
	FOREVER
		{
	 	CApaWindowGroupName::FindByAppUid(aAppUid, iWs, wgId);
	 	// KErrNotFound means that no more apps can be found
	 	if (wgId == KErrNotFound)
	 		break;
	 	if (IsRootWindowGroup(wgId))
	 		{
	 		// Found a root wg with the right app UID, return it.
	 		task.SetWgId(wgId);
	 		break;
	 		}
		}
	return task;
	}

 TBool CSysApTaskList::IsRootWindowGroup(TInt aWgId) const
	{
	TInt count = iWgs.Count();
	for (TInt ii=0; ii<count; ii++)
		{
        const RWsSession::TWindowGroupChainInfo& info = iWgs[ii];
        // find the window group id and check that it has no parent
        if (info.iId == aWgId)
        	return (info.iParentId <= 0);
		}
	return EFalse;
	}

 TInt CSysApTaskList::FindParentWgId(TInt aWgId) const
	{
	TInt count = iWgs.Count();
	for (TInt ii=0; ii<count; ii++)
		{
        const RWsSession::TWindowGroupChainInfo& info = iWgs[ii];
        if (info.iId == aWgId && info.iParentId > 0 && info.iParentId != info.iId)
        	return info.iParentId;
		}
	return 0;
	}

 TInt CSysApTaskList::FindChildWgId(TInt aWgId) const
	{
	TInt count = iWgs.Count();
	for (TInt ii=0; ii<count; ii++)
		{
        const RWsSession::TWindowGroupChainInfo& info = iWgs[ii];
        if (info.iParentId == aWgId && info.iId > 0 && info.iParentId != info.iId)
        	return info.iId;
		}
	return 0;
	}





