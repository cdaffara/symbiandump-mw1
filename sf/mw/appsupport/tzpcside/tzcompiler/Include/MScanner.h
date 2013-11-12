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

#ifndef __MSCANNER_H
#define __MSCANNER_H
//============================================================================
// MScanner class definition
//============================================================================
class MScanner
	{
	public:
		virtual int Scan(const char* aFileName)	= 0;
	};

#endif //__MSCANNER_H
//============================================================================
// End of file
//============================================================================
