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

#ifndef __MDOCUMENT_H
#define __MDOCUMENT_H
//============================================================================
// MDocument class definition
//============================================================================
class MDocument
	{
public:
	virtual void CreateChildElement()							= 0;
	virtual void CreateRootChildElement()						= 0;
	virtual void AddAttribute(const char* aValue)				= 0;
	virtual void CloseElement()									= 0;
	virtual void HandleScanError(const char* aFileName,int aLine,int aCol,char aError)= 0;
	};

#endif //__MDOCUMENT_H
//============================================================================
// End of file
//============================================================================
