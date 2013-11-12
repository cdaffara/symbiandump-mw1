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
#ifndef TSTASKMONITOR_H
#define TSTASKMONITOR_H

#include <QObject>
#include <QSharedPointer>
#include <QPair>

#include "tstaskmonitor_global.h"
#include "tstask.h"

class TsTaskMonitorPrivate;
class TsTaskChangeInfo;

typedef QPair<TsTaskChangeInfo, QSharedPointer <TsTask> > TsTaskChange;

class TSTASKMONITOR_EXPORT TsTaskMonitor : public QObject
{
    Q_OBJECT
    
public:
    TsTaskMonitor(QObject *parent = 0);
    ~TsTaskMonitor();
    QList<TsTaskChange> changeList(bool fullList = false); //TODO merge these functions with bool

signals:
    void taskListChanged();
    
private:
    TsTaskMonitorPrivate *d_ptr;   
    friend class TsTaskMonitorPrivate;
};

#endif //TSTASKMONITOR_H
