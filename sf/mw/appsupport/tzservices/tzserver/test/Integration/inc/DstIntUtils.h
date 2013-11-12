/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __DSTINTUTILS_H__
#define __DSTINTUTILS_H__

#include <calentryview.h>
#include <calsession.h>
#include <f32file.h>

class RTz;

class CDstIntUtils
	{
public:
	// Static general utility methods
	static TInt SetHomeTimeZoneL(const TDesC8& aLocation, const RTz &aServer);
	static TInt CheckHomeTimeZoneL(const RTz &aServer, const TDesC8& aLocation);
	static void AppendTimeParamStringL(TTime aTime, TDes& aParamString);
	static void GenerateVCalL(RFs& aFs, const TDesC& aFileName, const TDesC& aStartTime, const TDesC& aEndTime);
	static void OpenAgendaFileL(CCalSession& aModel, const TDesC& aFilename);
	static TCalLocalUid ImportVCalL(RFs& aFs, CCalSession& aModel, CCalEntryView& aEntryView, const TDesC& aFileName);
	static TInt CheckAgendaEntryL(CCalEntryView& aEntryView, const TCalLocalUid& aId, const TTime& aStart, const TTime& aEnd);
	};

#endif



