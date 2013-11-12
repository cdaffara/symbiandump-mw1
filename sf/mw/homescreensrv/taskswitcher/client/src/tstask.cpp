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
#include "tstask.h"

#include "tstaskcontent.h"
#include "tstasklauncher.h"

/*!
    @class TsTask
    @ingroup publicApi
    @brief TsTask object represents a single task.
    
    This class represents one running task. It can be queried for task content
    like screenshot or task name, or used to start or close task.
*/

/*!
    @internal
    Constructor. 
*/
TsTask::TsTask(const QSharedPointer<TsTaskContent> &content, TsTaskLauncher &launcher) : mContent(content), mLauncher(launcher)
{
}

/*!
    Destructor.
*/
TsTask::~TsTask()
{
}

/*!
    @return True if it's possible to close the task, false otherwise.
*/
bool TsTask::isClosable() const
{
    return mContent->mClosable;
}

/*!
    @return True if the task is running, false otherwise.
*/
bool TsTask::isActive() const
{
    return mContent->mActive;
}

/*!
    @return Screenshot of the task.
*/
QPixmap TsTask::screenshot() const
{
    return mContent->mScreenshot;
}

/*!
    @return Name of the task.
*/
QString TsTask::name() const
{
    return mContent->mName;
}

/*!
    Start or bring the task to foreground.
*/
void TsTask::open()
{
    mLauncher.openTask(mContent->mKey);
}

/*!
    Close the task.
*/
void TsTask::close()
{
    mLauncher.closeTask(mContent->mKey);
}
