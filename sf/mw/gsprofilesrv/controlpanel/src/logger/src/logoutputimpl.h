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
* Description:  
*
*/
#ifndef LOGOUTPUTIMPL_H
#define LOGOUTPUTIMPL_H

#include <logoutput.h>
#include <QString>

// name: "debugoutput"
class DebugLogOutput : public LogOutput
{
    Q_OBJECT
public:
    DebugLogOutput();
    virtual ~DebugLogOutput();
    virtual void output(const QString &log);
    virtual bool init();
private:
    virtual bool doLoad(QSettings &settings);
};

// name: "consoleoutput"
class ConsoleLogOutput : public LogOutput
{
    Q_OBJECT
public:
    ConsoleLogOutput();
    virtual ~ConsoleLogOutput();
    virtual void output(const QString &log);
    virtual bool init();
private:
    virtual bool doLoad(QSettings &settings);
};

// name: "fileoutput"

/* config format:
    fileoutput/logfile = C:/data/logs/app.log
    fileoutput/truncate = 0
*/
class QFile;
class QTextStream;
class FileLogOutput : public LogOutput
{
    Q_OBJECT
    Q_PROPERTY(bool truncate READ truncate WRITE setTruncate)
    Q_PROPERTY(QString logFile READ logFile WRITE setLogFile)
public:
    explicit FileLogOutput(const QString &logFilePath = QString());
    virtual ~FileLogOutput();
    virtual void output(const QString &log);
    virtual bool init();

    bool truncate() const;
    void setTruncate(bool truncate);
    QString logFile() const;
    void setLogFile(const QString &logFile);
private:
    virtual bool doLoad(QSettings &settings);
private:
    QFile  *mLogFile;
    QTextStream *mTextStream;
    QString mLogFilePath;
    bool mTruncateFile;
};

#endif //LOGOUTPUTIMPL_H
