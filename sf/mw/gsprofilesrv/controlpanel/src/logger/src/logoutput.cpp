/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  An interface to output log string.
*
*/
#include <logoutput.h>
#include <QString>

class LogOutputPrivate
{
public:
    LogOutputPrivate(Logger *parentLogger = 0) : mParentLogger(parentLogger)
    {
    }
    Logger *mParentLogger;
    QString mName;
};


//LogOutput
LogOutput::LogOutput(Logger *parentLogger /*= 0*/) 
: d_ptr(new LogOutputPrivate(parentLogger))
{
}

LogOutput::~LogOutput()
{
    delete d_ptr;
}

bool LogOutput::load(QSettings &settings)
{
    if (!doLoad(settings)) {
        return false;
    }
    return init();
}

Logger *LogOutput::parentLogger()
{
    return d_ptr->mParentLogger;
}

void LogOutput::setParentLogger(Logger *parentLogger)
{
    d_ptr->mParentLogger = parentLogger;
}

QString LogOutput::name() const
{
    return d_ptr->mName;
}

void LogOutput::setName(const QString &name)
{
    d_ptr->mName = name;
}

