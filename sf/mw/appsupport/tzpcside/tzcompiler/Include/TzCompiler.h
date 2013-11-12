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

#ifndef __TZCOMPILER_H
#define __TZCOMPILER_H
//============================================================================
// Forward Declarations
//============================================================================
class CTZDocument;
//============================================================================
// CTZCompiler class definition
//============================================================================
class CTzCompiler
	{
	public:
		CTzCompiler(CTZDocument& aTzDocument);
		void CompileL();
	private:
		CTZDocument& iTzDocument;
	};

#endif //__TZCOMPILER_H
//============================================================================
// End of file
//============================================================================
