// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PRNRES_H__
#define __PRNRES_H__

#if !defined(__TXTMRTSR_H__)
#include <txtmrtsr.h>
#endif

class TStoreResolver : public MRichTextStoreResolver
	{
public:
	TStoreResolver(const CStreamStore& aStore);
	// from MRichTextStoreResolver
	const CStreamStore& StreamStoreL(TInt aPos)const;
private:
	const CStreamStore& iStore;
	};

#endif
