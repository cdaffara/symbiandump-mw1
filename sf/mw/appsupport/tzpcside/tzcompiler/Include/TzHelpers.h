// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// DST TZ Database Compiler 
// 
//

#ifndef __TZHELPERS_H_
#define __TZHELPERS_H_

#include <string>
#include "TzEntities.h"

//============================================================================
// CTzCpHelpers Class Definition
// Collection of useful static functions
//============================================================================
class CTzCpHelpers
	{
public:
	static int GetMonth(std::string monthString);
	static int GetDay(std::string dayString);
	static TInt16 GetTimeOfDayInMinutes(std::string aTimeString);
	static int GetTimeReference(std::string aTimeString);
	static int GetFileSizeInBytes(std::string aFileName);
	static void PrintStep(std::string aStep);
	static void PrintStepResult(int aResult);
	};
#endif
//============================================================================
// End of file
//============================================================================
