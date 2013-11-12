// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains declaration of CProgressCallBack class, which is needed to be 
// passed for asynchronous operations on the CalInterimAPI
// 
//

#ifndef __PROGRESS_CALL_BACK_H__
#define __PROGRESS_CALL_BACK_H__

// User Includes
#include "TestMultipleAlarmsServer.h"	

// System Includes
#include <calprogresscallback.h>

/**
Class which implements the MCalProgressCallBack mixin which has
to be passed when making asynchronous calls on CalInterimAPI
@internalTechnology
@test
*/
class CProgressCallBack : public CActive, public MCalProgressCallBack
	{
public:	
	CProgressCallBack(TRequestStatus& aStatus);
	
	// From MCalProgressCallBack
	void				Progress(TInt aPercentageCompleted);
	void				Completed(TInt aError);
	TBool				NotifyProgress();

	// From CActive
	void				RunL();
	void				DoCancel();
private:
	TRequestStatus& 	iRequestStatus;
	};

#endif		// __PROGRESS_CALL_BACK_H__


