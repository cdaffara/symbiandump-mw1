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
//

#include <tz.h>
#include "tzrules.h"
#include <tzdefines.h>

//
/**
Serialised TTzActualisedRule externaliser.
@param aStream A stream that will contain the serialised TTzActualisedRule.
*/
EXPORT_C void TTzActualisedRule::ExternalizeL(RWriteStream& aStream) const
	{

	TInt64 time = iTimeOfChange.Int64();
	aStream << time;

	aStream << iStdOffset;
	aStream << iDstOffset;
	aStream.WriteInt32L(iTimeReference);
	}

/**
Serialised TTzActualisedRule data internaliser.
@param aStream A stream containing the serialised TTzActualisedRule.
*/
EXPORT_C void TTzActualisedRule::InternalizeL(RReadStream& aStream)
	{
	TInt64 time;

	aStream >> time;
	iTimeOfChange = TTime(time);

	aStream >> iStdOffset;
	aStream >> iDstOffset;
	iTimeReference = static_cast<TTzTimeReference>(aStream.ReadInt32L()); 
	}

// comparator used by the RArray<TTzActualisedRule>
TInt TTzActualisedRule::Order(const TTzActualisedRule& aLeft, const TTzActualisedRule& aRight)
	{

	if (aLeft.iTimeOfChange < aRight.iTimeOfChange)
		{
		return -1;
		}
	if (aLeft.iTimeOfChange == aRight.iTimeOfChange)
		{
		return 0;
		}
	return 1;
	}

