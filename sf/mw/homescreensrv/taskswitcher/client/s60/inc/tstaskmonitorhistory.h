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
#ifndef TSTASKMONITORHISTORY_H_
#define TSTASKMONITORHISTORY_H_

#include <QByteArray>
#include <QDateTime>

class TsTaskMonitorHistory
{
public:
    TsTaskMonitorHistory(const QByteArray &key, const QDateTime &updateTime);
    bool isEqual(const TsTaskMonitorHistory &item) const;
    bool isUpdated(const TsTaskMonitorHistory &item) const;
    int offset() const;
    void setOffset(int offset);
private:
    QByteArray mKey;
    QDateTime mUpdateTime;
    int mOffset;
};

#endif /* TSTASKMONITORHISTORY_H_ */
