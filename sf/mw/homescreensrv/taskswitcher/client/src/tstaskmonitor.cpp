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

#include "tstaskmonitor.h"
#include "tstaskmonitor_p.h"

#include "tstask.h"

/*!
    @class TsTaskMonitor
    @ingroup publicApi
    @brief TsTaskMonitor gives access to list of running tasks.
    
    This class is one of the Qt interfaces for Task Monitor server. It can be
    used to retrieve list of running tasks and receive notifications about the
    changes of this list.
*/

/*!
    Constructor.
*/
TsTaskMonitor::TsTaskMonitor(QObject *parent) : QObject(parent), d_ptr(0)
{
    d_ptr = new TsTaskMonitorPrivate(this);
}

/*!
    Destructor.
*/
TsTaskMonitor::~TsTaskMonitor()
{
    delete d_ptr;
}

/*!
    \return Current list of tasks from Task Monitor server.

    \sa TsTask, taskListChanged()
*/
QList<TsTaskChange> TsTaskMonitor::changeList(bool fullList)
{
    return d_ptr->changeList(fullList);
}

/*!
    @fn TsTaskMonitor::taskListChanged()

    This signal is emitted when task list changes in any way:
    - the new task is started
    - running task is terminated
    - parameters of one task change (for example image representing the task is updated)

    No data is passed within this signal, user should retrieve task list
    using taskList().

    \sa taskList()
*/
