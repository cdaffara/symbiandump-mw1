/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UTCOFFSETCHANGEEVENTNOTIFIER_H__
#define __UTCOFFSETCHANGEEVENTNOTIFIER_H__

#include <e32base.h>
#include <e32property.h>

class CUtcOffsetChangeEventNotifier : public CActive
	{
public:
	static CUtcOffsetChangeEventNotifier* NewL();
	~CUtcOffsetChangeEventNotifier();
	
	void WaitForNotification();

private:
	CUtcOffsetChangeEventNotifier();
	void ConstructL();
	
	// from CActive
	void RunL();
	void DoCancel();
	
private:
	RProperty iProperty;
	CActiveSchedulerWait* iActiveSchedulerWait;
	};
 
#endif // __UTCOFFSETCHANGEEVENTNOTIFICATION_H__
