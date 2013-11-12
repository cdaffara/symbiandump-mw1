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

// System includes
#include <e32base.h>
#include <e32test.h>
#include <f32file.h>

// Literal constants
_LIT(KTestTitle, "TBitwiseOps");

// Globals
RTest TheTest(KTestTitle);

void DoSetAndClearL()
	{
	TInt iTestFlags   = 0;
	TInt iFlagIsSet   = 0;
	TInt iFlagIsClear = 0;

	//set "color" flags
	iTestFlags |= 0x00000001; //set blue color (0)
	iTestFlags |= 0x00000002; //set red color (1)
	iTestFlags |= 0x00000004; //set green color (2)
	iTestFlags |= 0x00000008; //set yellow color (3)  
	iTestFlags |= 0x00000010; //set purple color (4)
	iTestFlags |= 0x00000020; //set black color (5)
	iTestFlags |= 0x00000040; //set white color (6)
	iTestFlags |= 0x00000080; //set grey color (7)


	//Is the "color" flag set?
	//If yes, iFlagIsSet = the actual data value of the color
	//If not, iFlagIsSet = 0
	iFlagIsSet = iTestFlags & 0x00000001; //blue color (0)
	if (iFlagIsSet != 1) User::Leave(KErrGeneral);
	TheTest(iFlagIsSet == 1, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000002; //red color (1)
	TheTest(iFlagIsSet == 2, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000004; //green color (2)
	TheTest(iFlagIsSet == 4, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000008; //yellow color (3)
	TheTest(iFlagIsSet == 8, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000010; //purple color (4)
	TheTest(iFlagIsSet == 16, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000020; //black color (5)
	TheTest(iFlagIsSet == 32, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000040; //white color (6)
	TheTest(iFlagIsSet == 64, __LINE__);

	iFlagIsSet = iTestFlags & 0x00000080; //grey color (7)
	TheTest(iFlagIsSet == 128, __LINE__);


	//clear "color" flags
	iTestFlags &= ~0x00000001; //clear blue color (0)
	iTestFlags &= ~0x00000002; //clear red color (1)
	iTestFlags &= ~0x00000004; //clear green color (2)
	iTestFlags &= ~0x00000008; //clear yellow color (3)  
	iTestFlags &= ~0x00000010; //clear purple color (4)
	iTestFlags &= ~0x00000020; //clear black color (5)
	iTestFlags &= ~0x00000040; //clear white color (6)
	iTestFlags &= ~0x00000080; //clear grey color (7)

	//Is the "color" flag clear?
	//If yes, iFlagIsClear = 1
	//If not, iFlagIsClear = 0
	iFlagIsClear = !(iTestFlags & 0x00000001); //blue color (0)
	TheTest(iFlagIsClear == 1, __LINE__);

	iFlagIsClear = !(iTestFlags & 0x00000002); //red color (1)
	TheTest(iFlagIsClear == 1, __LINE__);
  
	iFlagIsClear = !(iTestFlags & 0x00000004); //green color (2)
	TheTest(iFlagIsClear == 1, __LINE__);

	iFlagIsClear = !(iTestFlags & 0x00000008); //yellow color (3)
	TheTest(iFlagIsClear == 1, __LINE__);

	iFlagIsClear = !(iTestFlags & 0x00000010); //purple color (4)
	TheTest(iFlagIsClear == 1, __LINE__);

	iFlagIsClear = !(iTestFlags & 0x00000020); //black color (5)
	TheTest(iFlagIsClear == 1, __LINE__);

	iFlagIsClear = !(iTestFlags & 0x00000040); //white color (6)
	TheTest(iFlagIsClear == 1, __LINE__);

	iFlagIsClear = !(iTestFlags & 0x00000080); //grey color (7)
	TheTest(iFlagIsClear == 1, __LINE__);
	}

void DoToggle()
	{
	TInt iTestFlags = 0;

	iTestFlags ^= 0x00000001; // switch on blue color (1)
	iTestFlags ^= 0x00000001; // switch off blue color
	iTestFlags ^= 0x00000001; // switch on blue color
	TheTest(iTestFlags == 1, __LINE__);

	iTestFlags ^= 0x00000002; // switch on and off red color (2)
	iTestFlags ^= 0x00000002;
	iTestFlags ^= 0x00000002;
	TheTest(iTestFlags == 3, __LINE__);

	iTestFlags ^= 0x00000004; // switch on and off green color (3)
	iTestFlags ^= 0x00000004;
	iTestFlags ^= 0x00000004;
	TheTest(iTestFlags == 7, __LINE__);

	iTestFlags ^= 0x00000008; // switch on and off yellow color (4)
	iTestFlags ^= 0x00000008;
	iTestFlags ^= 0x00000008;
	TheTest(iTestFlags == 15, __LINE__);

	iTestFlags ^= 0x00000010; // switch on and off purple color (5)
	iTestFlags ^= 0x00000010;
	iTestFlags ^= 0x00000010;
	TheTest(iTestFlags == 31, __LINE__);

	iTestFlags ^= 0x00000020; // switch on and off black color (6)
	iTestFlags ^= 0x00000020;
	iTestFlags ^= 0x00000020;
	TheTest(iTestFlags == 63, __LINE__);

	iTestFlags ^= 0x00000040; // switch on and off white color (7)
	iTestFlags ^= 0x00000040;
	iTestFlags ^= 0x00000040;
	TheTest(iTestFlags == 127, __LINE__);
	}

void DoOperators()
	{
	TInt iTestFlags1 = 0;
	TInt iTestFlags2 = 0;
  
	//test equality sign
	iTestFlags1 |= 0x00000040;
	iTestFlags2 |= 0x00000040;
	TheTest(iTestFlags1 == iTestFlags2, __LINE__);

	//test assignment sign
	iTestFlags1 ^= 0x00000040;
	iTestFlags2 = iTestFlags1;
	TheTest(iTestFlags1 == iTestFlags2, __LINE__);
	}

void RunTestsL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	DoSetAndClearL();
	DoToggle();
	DoOperators();
	
	CleanupStack::PopAndDestroy(scheduler);
	}

/**
@SYMTestCaseID PIM-TBITWISEOPS-0001
*/
TInt E32Main()
	{
	__UHEAP_MARK;
	//
	TheTest.Title();
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (!cleanup)
	return KErrNoMemory;
	//
	TheTest.Start(_L("@SYMTestCaseID PIM-TBITWISEOPS-0001 TBITWISEOPS"));
	TheTest.Title();
	
	TRAPD(err, RunTestsL());
	TheTest(err == KErrNone, __LINE__);

	delete cleanup;
	TheTest.End();
	TheTest.Close();

	__UHEAP_MARKEND;
	return KErrNone;
	}
