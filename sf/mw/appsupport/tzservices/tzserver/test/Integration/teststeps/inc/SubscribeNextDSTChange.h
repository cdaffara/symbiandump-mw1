/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TEST_NEXT_DST_CHANGE_H__
#define __TEST_NEXT_DST_CHANGE_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

_LIT(KSubscribeNextDSTChange, "SubscribeNextDSTChange");
		
// Class CSubscribeNextDSTChange
class CSubscribeNextDSTChange : public CTestStep
	{
public:
	static CSubscribeNextDSTChange* NewL();
	~CSubscribeNextDSTChange();
	
	TVerdict doTestStepL();
	
private:
	CSubscribeNextDSTChange();
	void ConstructL();
	
private:
	RTz iTz;
	};

#endif // __TEST_NEXT_DST_CHANGE_H__
