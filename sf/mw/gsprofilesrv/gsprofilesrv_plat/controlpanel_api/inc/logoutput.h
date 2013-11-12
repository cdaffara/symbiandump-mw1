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
#ifndef LOGOUTPUT_H
#define LOGOUTPUT_H

#include <loggerglobal.h>
#include <QObject>

class QString;
class QSettings;
class Logger;

class LogOutputPrivate;
class LOGGER_EXPORT LogOutput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
public:
    virtual ~LogOutput();
    virtual void output(const QString &log) = 0;
    virtual bool init() = 0;
    bool load(QSettings &settings);

    Logger *parentLogger();
    void setParentLogger(Logger *parentLogger);

    QString name() const;
    void setName(const QString &name);
private:
    virtual bool doLoad(QSettings &settings) = 0;
protected:
    LogOutput(Logger *parentLogger = 0);
private:
    LogOutputPrivate *d_ptr;
};

#endif //LOGOUTPUT_H
