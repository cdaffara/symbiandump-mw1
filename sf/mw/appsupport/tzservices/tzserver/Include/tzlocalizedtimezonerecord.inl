// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TZLOCALIZEDTIMEZONERECORD_INL__
#define __TZLOCALIZEDTIMEZONERECORD_INL__

inline TUint16 CTzLocalizedTimeZoneRecord::Id() const
	{
	return iId; 
	}

inline const TDesC& CTzLocalizedTimeZoneRecord::StandardName() const
	{
	return *iStandardName;
	}

inline const TDesC& CTzLocalizedTimeZoneRecord::DaylightName() const
	{
	return *iDaylightName;
	}

inline const TDesC& CTzLocalizedTimeZoneRecord::ShortStandardName() const
	{
	return *iShortStandardName;
	}

inline const TDesC& CTzLocalizedTimeZoneRecord::ShortDaylightName() const
	{
	return *iShortDaylightName;
	}

inline TUint CTzLocalizedTimeZoneRecord::ResourceId() const
	{
	return iResourceId;
	}

#endif
