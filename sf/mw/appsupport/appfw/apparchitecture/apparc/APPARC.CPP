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
// apparc.cpp
//

#include <apparc.h> // stuff everyone will want ie most things
#include "APADLL.H" // RApaApplication
#include "APASTD.H" // Panics etc.

  
//
// RApaApplication
//

RApaApplication::RApaApplication() : iApplication(NULL), iScheduledForDeletion(EFalse)
	{}

void RApaApplication::Close()
	{
	delete this;
	}

RApaApplication::~RApaApplication()
	{
	iAppName.Close();
	delete iApplication;
	}

void RApaApplication::CreateApplicationL(TApaApplicationFactory aApplicationFactory)
	{
	__ASSERT_ALWAYS(!iApplication, Panic(EPanicApplicationAlreadyExists));
	iApplication = aApplicationFactory.CreateApplicationL();
	User::LeaveIfNull(iApplication);
	
	iFileUid = aApplicationFactory.AppFileUid();
	iAppName.Assign(aApplicationFactory.AppFileNameL());
	
	iApplication->SetAppFileNameRef(iAppName);
	}

TFileName RApaApplication::AppFileName() const
	{
	return iAppName;
	}

TUid RApaApplication::AppFileUid()const
	{
	return iFileUid;
	}

CApaApplication* RApaApplication::Application()
	{
	return iApplication;
	}

void RApaApplication::ScheduleForAsyncDeletion(TBool aDoDelete)
	{
	iScheduledForDeletion = aDoDelete;
	}
	
TBool RApaApplication::IsScheduleForAsyncDeletion() const
	{
	return iScheduledForDeletion;
	}


