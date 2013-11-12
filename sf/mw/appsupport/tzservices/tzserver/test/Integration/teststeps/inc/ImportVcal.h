/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __IMPORTVCAL_H__
#define __IMPORTVCAL_H__

#include "DstIntStep.h"
#include <calprogresscallback.h>

const TInt KMaxVCardDateChars = 32;


// String for the server create test step code
_LIT(KImportVCal, "ImportVCal");

class CImportVCal : public CDstIntStep, public MCalProgressCallBack
	{
public:
	CImportVCal();
	~CImportVCal();

protected:
	// From CDstStep
	void PrepareTestEnvironmentL();

	// from MCalProgressCallBack
	void Progress(TInt aPercentageCompleted);
	void Completed(TInt aError);
	TBool NotifyProgress();
private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	TBuf8<KMaxLocationChars> iLocation;
	TInt iLocationId;
	TBuf<KMaxVCardDateChars> iDtStart;
	TBuf<KMaxVCardDateChars> iDtEnd;
	TTime iTimeStart;
	TTime iTimeEnd;
	};

#endif
