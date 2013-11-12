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

#if !defined(__con_internal_H__)
#define __con_internal_H__
#include <f32file.h>
#include <barsc.h>

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__APMSTD_H__)
#include <apmstd.h>
#endif

class CConverterBase;
/**
@internalComponent
@deprecated
*/
const TInt KUidConverterDllValue8=0x10000C61;

/**
@internalComponent
@deprecated
*/
const TUid KUidConverterDll8={KUidConverterDllValue8};

/**
@internalComponent
*/
const TInt KUidConverterInfoStreamValue=0x10000CD0;

/**
@internalComponent
*/
const TUid KUidConverterInfoStream={KUidConverterInfoStreamValue};

class ConverterListUtil
/** @internalComponent */
	{
public:
	static CConverterBase* UtilityConverterL(TUid aUid);
	static TInt CountUtilityConverters();
	};

#endif
