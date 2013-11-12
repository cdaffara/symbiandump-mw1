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

class CTestServerTest
{
public: 
	inline CTestServerTest():iErr(KErrNone){};
	static CTestServerTest* NewL();
	static CTestServerTest* NewLC();
	void ConstructL();
	~CTestServerTest();
	// Test Methods
	void TestSetHomeTimeL();
	void TestFileSizeL();
	void TestDeleteFileL(const TDesC& aFileName);
	void TestKillProcess();
	void TestTimeZoneL();
	void TestChangeLocaleNameL();
	void CheckForFileL(const TDesC& aFileName, TBool aFilePresent);
	void CopyFileL();
	void CreateTestFileL(const TDesC& aFileName);
	void TestNotifyChangeL();
	void DeleteTestFileL(const TDesC& aFileName);

	
private: 
	void PrintTTimeL (const TDesC& aMessage, const TTime& aTime) const;
	void TestErrorL() const;
	
private:
	RPIMTestServer iServ;	
	TInt iErr;
	RFs iFs;
};
