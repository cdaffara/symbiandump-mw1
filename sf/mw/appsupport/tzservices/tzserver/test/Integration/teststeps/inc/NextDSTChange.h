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



#ifndef __NEXT_DST_CHANGE_H__
#define __NEXT_DST_CHANGE_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

_LIT(KNextDSTChange, "NextDSTChange");
		
// Class CTestNextDSTChange
class CNextDSTChange : public CTestStep
	{
public:
	static CNextDSTChange* NewL();
	~CNextDSTChange();
	
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepL();
	TVerdict doTestStepPostambleL();
	
private:
	CNextDSTChange();
	void ConstructL();
	
private:
	RTz iTz;
	};

#endif // __NEXT_DST_CHANGE_H__
