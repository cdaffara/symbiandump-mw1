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
#include "tstaskmonitorhistory.h"

TsTaskMonitorHistory::TsTaskMonitorHistory(const QByteArray &key, 
                                           const QDateTime &updateTime) :
mKey(key), mUpdateTime(updateTime)
{
    
}

void TsTaskMonitorHistory::setOffset(int offset)
{
    mOffset = offset;
}

int TsTaskMonitorHistory::offset() const
{
    return mOffset;
}

bool TsTaskMonitorHistory::isEqual(const TsTaskMonitorHistory &item) const
{
    if (mKey == item.mKey) {
        return true;
    }
    return false;
}

bool TsTaskMonitorHistory::isUpdated(const TsTaskMonitorHistory &item) const
{
    Q_ASSERT(mKey == item.mKey);
    if (mUpdateTime != item.mUpdateTime) {
        return true;
    }
    return false;    
}
