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

#ifndef __MCOMPILER_H
#define __MCOMPILER_H
//============================================================================
// MCompiler class definition
//============================================================================
class MCompiler
	{
	public:
		virtual int Scan(const char* aFileName)		= 0;
		virtual void AssembleL()						= 0;
		virtual void Link()							= 0;
	};

#endif //__MCOMPILER


//============================================================================
// End of file
//============================================================================
