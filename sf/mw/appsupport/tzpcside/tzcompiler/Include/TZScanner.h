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

#ifndef __TZSCANNER_H
#define __TZSCANNER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "MScanner.h"
//============================================================================
// Forward declarations
//============================================================================
class MDocument;
//============================================================================
// CTzCpScanner class definition
//============================================================================
class CTzCpScanner : public MScanner
	{
	public:
		CTzCpScanner();
		~CTzCpScanner();
		
		void SetDocument(MDocument* aDocument) { iDocument = aDocument; }
	private:
		//From MScanner
		int Scan(const char* aFileName);

		void ControlScanning();
		MDocument* iDocument;
		
		std::ifstream dbFile;
		
		enum EScanState
			{
			EWaitFirstChar,
			EWaitTokenEnd,
			EWaitTokenStart,
			EWaitLineFeed
			};
		EScanState scanState;

		int iLineNo;
		int iColumnNo;

		std::string iFileName;
		const char* iValidChars;
	};

#endif //__TZSCANNER_H
//============================================================================
// End of file
//============================================================================
