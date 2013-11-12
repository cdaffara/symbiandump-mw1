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
//

#ifndef __TFAILEDRECTEST_H__
#define __TFAILEDRECTEST_H__

#include <e32base.h>

class TFailedRecTest
	{
public:
	inline TFailedRecTest(TInt aTestNumber, const TDesC* aTestTitle, TInt aError);

public:
	TInt iTestNumber;
	TBuf<128> iTestTitle;
	TInt iError;
	};
	
inline
TFailedRecTest::TFailedRecTest(TInt aTestNumber, const TDesC* aTestTitle, TInt aError)
: iTestNumber(aTestNumber),  iError(aError)
	{
	iTestTitle.Copy(*aTestTitle);
	}

#endif // __TFAILEDRECTEST_H__
