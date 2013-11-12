/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef AFQUERIES_H
#define AFQUERIES_H


#include <e32base.h>

_LIT( KSelectRow, "SELECT * FROM Activities WHERE ApplicationId=%S AND ActivityName='%S'");
_LIT( KSelectRows, "SELECT * FROM Activities ORDER BY Timestamp DESC");
_LIT( KSelectAppRows, "SELECT * FROM Activities WHERE ApplicationId=%S");
_LIT( KDeleteRow, "DELETE FROM Activities WHERE ApplicationId=%S AND ActivityName='%S'");
_LIT( KDeleteRows, "DELETE FROM Activities WHERE ApplicationId=%S");
_LIT( KDeleteNonPersistentActivities, "DELETE FROM Activities WHERE Flags<%S");

_LIT(KApplicationColumnName, "ApplicationId");
_LIT(KActivityColumnName, "ActivityName");
_LIT(KCustomNameColumnName, "CustomName");
_LIT(KFlagsColumnName, "Flags");
_LIT(KTimestampColumnName, "Timestamp");
_LIT(KDataColumnName, "Data");

_LIT(KActivityTableName, "Activities");
_LIT(KActivityIndexName, "ActivitiesKey");
#endif  // AFQUERIES_H

