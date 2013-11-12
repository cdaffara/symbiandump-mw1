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
#ifndef TSTASKMONITOR_P_H
#define TSTASKMONITOR_P_H

#include <QList>
#include <QSharedPointer>
#include <QDateTime>

#include "tstask.h"

class TsTaskMonitor;

class TsTaskMonitorPrivate
{
public:
    TsTaskMonitorPrivate(TsTaskMonitor *q);
    
public:
    QList< QSharedPointer<TsTask> > taskList();
    
private:
 };

#endif //TSTASKMONITOR_P_H
