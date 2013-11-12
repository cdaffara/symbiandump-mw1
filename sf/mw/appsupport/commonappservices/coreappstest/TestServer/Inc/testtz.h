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
//

#ifndef __TESTTZ_H__
#define __TESTTZ_H__

#include <e32base.h>
#include <s32std.h>
#include <tzdefines.h>

class CTestTzId : public CBase
	{
public:

	~CTestTzId();
	static CTestTzId* NewL(const TDesC8& aNameIdentity);
	static CTestTzId* NewL(RReadStream& aStream);
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	const TDesC8& TimeZoneNameID() const;

private:
	void ConstructL(const TDesC8& aZoneIdentity);
	CTestTzId();

private:
	HBufC8* iZoneId;
	TUint32 iReferenceId;
	};

class RTestTz : public RSessionBase
	{
public:
	CTestTzId* GetTimeZoneIdL() const;
	void SetTimeZoneL(CTestTzId& aZone) const;
	void Close();
	TInt Connect();
	~RTestTz();
	RTestTz();
	TInt ConvertToUniversalTime(TTime& aTime) const;
    // Methods for setting the system time.
    	TInt SetHomeTime(const TTime& aLocalTime) const;
    
	TVersion Version() const;

private:
	static TInt StartServer();
	TInt DoConnect();
	void doConvertL(TTime& aTime, 
					TTzTimeReference aTimerRef) const;
	};

#endif
