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

#ifndef __TIMEZONERULES_H__
#define __TIMEZONERULES_H__

#include <e32base.h>
#include <s32std.h>
#include <tzdefines.h>
#include "tzpersisted.h"

const TInt KActualisedRulesGranularity = 8;

// specifies the limits for rules caching.
// (year - KRuleCacheLowerLimit) , (year + KRuleCacheUpperLimit)
// the actual cache includes one more year at year - (KRuleCacheLowerLimit + 1)
// required to provide the offset used at the start of the search 
const TUint KRuleCacheLowerLimit = 2;
const TUint KRuleCacheUpperLimit = 3;

/**
Encapsulates a single Time Zone DST Rule

@internalComponent
@since 9.1
*/
class TTzActualisedRule
	{
public:	
	inline TTzActualisedRule(const TTime& aTimeOfChange, TInt  aStdOffset, TInt aDstOffset, TTzTimeReference aTimeReference);
	inline TTzActualisedRule();
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

	static TInt Order(const TTzActualisedRule& aLeft, const TTzActualisedRule& aRight);

public:
	/** time of DST change w.r.t to iTimeReference
	*/
	TTime iTimeOfChange;
	/** std to UTC offset
	*/
	TInt32  iStdOffset;

	/** wall-clock to standard time offset
	*/
	TInt32 iDstOffset;
				
	/** what time recogning iTimeOfChange is referenced to; see <code> TTzTimeReference </code> 
	*/
	TTzTimeReference iTimeReference;
	};

inline TTzActualisedRule::TTzActualisedRule(const TTime& aTimeOfChange, TInt  aStdOffset, TInt aDstOffset, TTzTimeReference aTimeReference) :
					iTimeOfChange(aTimeOfChange),
					iStdOffset(aStdOffset),
					iDstOffset(aDstOffset),
					iTimeReference(aTimeReference)
	{
	}

inline TTzActualisedRule::TTzActualisedRule() :
					iTimeOfChange(0),
					iStdOffset(0),
					iDstOffset(0),
					iTimeReference(ETzUtcTimeReference)
	{
	}

#endif


