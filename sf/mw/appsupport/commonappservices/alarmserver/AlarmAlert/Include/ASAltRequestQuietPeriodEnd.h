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

#ifndef __ASALTREQUESTQUIETPERIODEND_H__
#define __ASALTREQUESTQUIETPERIODEND_H__

#include <e32base.h>

class RASAltClientSession;
class MASAltAlarmAlertObserver;

class CASAltRequestQuietPeriodEnd : public CActive
	{
public:								
	static CASAltRequestQuietPeriodEnd* NewL(RASAltClientSession& aSession,MASAltAlarmAlertObserver& aObserver);
	~CASAltRequestQuietPeriodEnd ();

	void RequestQuietPeriodEnd();

private:									
	CASAltRequestQuietPeriodEnd(RASAltClientSession& aSession,MASAltAlarmAlertObserver& aObserver);
	void ConstructL();

private:									
	void RunL();
	void DoCancel();

private:									
	inline RASAltClientSession&	Session() {return iSession;}

private:									
	RASAltClientSession& iSession;
	MASAltAlarmAlertObserver& iObserver;
	TPckgBuf<TTime> iTimeBuf;
	};

#endif
