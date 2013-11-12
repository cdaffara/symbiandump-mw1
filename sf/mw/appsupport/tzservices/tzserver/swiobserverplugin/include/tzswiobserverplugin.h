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

#ifndef __TZSWIOBSERVERPLUGIN_H__
#define __TZSWIOBSERVERPLUGIN_H__

#include <f32file.h>
#include <ecom/implementationproxy.h>
#include <swi/swiobserverplugin.h>
#include <tz.h>

class Swi::CObservationFilter;

NONSHARABLE_CLASS(CTzSwiObserverPlugin) : public Swi::CSwiObserverPlugin
	{
public:
	static CTzSwiObserverPlugin* NewL();
	~CTzSwiObserverPlugin();
	
private:
	CTzSwiObserverPlugin();
	
private:	// from Swi::CSwiObserverPlugin
	void StartL(RFs& aFs);
 	void NextObservedHeaderL(const Swi::CObservationHeader& aHeader, TRequestStatus& aStatus);	
 	void NextObservedDataL(const Swi::CObservationData& aData, TInt aFilterIndex, TRequestStatus& aStatus);	
 	void FinalizeL(TRequestStatus& aStatus);
 	Swi::CObservationFilter* GetFilterL() const;
 	void AddFilterL(Swi::CObservationFilter& aFilterList, const TDesC& aFilter) const;

private:	// from CActive
	void RunL();
	TInt RunError(TInt aError);
	void DoCancel();
	
	RTz& GetTzClientL();

private:
	/** Handle to the file server session. */
	RFs* iFs;
	RTz iTz;
	RFileWriteStream* iFileWriteStream;
	TBool iTzConnected;
	};

#endif
