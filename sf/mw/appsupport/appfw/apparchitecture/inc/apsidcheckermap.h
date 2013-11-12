// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// apsidchecker.h
//

#ifndef __APSIDCHECKERMAP_H__
#define __APSIDCHECKERMAP_H__

#include <e32base.h>
#include <e32hashtab.h>

class CAppSidChecker;

/** Sid checker map

This Maps application type uids to Instances of the CAppSidChecker interface,
loading them if neccessary.

@internalComponent */
NONSHARABLE_CLASS(RSidCheckerMap) : public RHashMap<TUint,CAppSidChecker*>
	{
public:	
	RSidCheckerMap( TCallBack aRescanCallBack );
	
	CAppSidChecker& FindSidCheckerL(TUid aAppType);
	void Close(); //lint !e1511 Member hides non-virtual member
private:
	TCallBack iRescanCallBack;
	};


#endif	// __APSIDCHECKERMAP_H__

