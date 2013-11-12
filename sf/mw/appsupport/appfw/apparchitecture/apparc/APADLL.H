// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// apadll.h
//

#ifndef __APADLL_H__
#define __APADLL_H__

#include <e32std.h>
#include <e32base.h>
#include <apadef.h>
#include <apparc.h>

class CApaApplication;

NONSHARABLE_CLASS(RApaApplication)
	{
public:
	RApaApplication();
	void Close();
	void CreateApplicationL(TApaApplicationFactory aApplicationFactory); // Uses the factory to create the application
	
	TFileName AppFileName() const;
	TUid AppFileUid() const;
	CApaApplication* Application();
	inline const CApaApplication* Application() const;
	void ScheduleForAsyncDeletion(TBool aDoDelete = ETrue);
	TBool IsScheduleForAsyncDeletion() const;
private:
	~RApaApplication();
private:
	CApaApplication* iApplication;
	RBuf iAppName;
	TUid iFileUid;
	TBool iScheduledForDeletion;
	};


// inlines

const CApaApplication* RApaApplication::Application() const
	{ return const_cast<RApaApplication*>(this)->Application(); }

#endif	// __APADLL_H__
