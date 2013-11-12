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
* Description:  This class provide log functionality.
*
*/
#include <logger.h>
#include "logoutput.h"
#include "logoutputimpl.h"
#include <QtAlgorithms>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QSettings>
#include <QFileInfo>
#include "loglogger.h"
#include "logoutputfactory.h"

typedef QHash<QString,Logger*> LoggerContainer;

Q_GLOBAL_STATIC(LoggerContainer,theLoggerList)

class InitLogLogger 
{
public:
    InitLogLogger() {
        LogOutput *output = new DebugLogOutput();
        output->init();
        Logger::instance(LOGLOGGER_NAME)->addLogOutput(output);
    }
} dummy_instance;

class LoggerPrivate 
{
public:
    LoggerPrivate(const QString &name = QString())
        : mName(name), 
          mDateTimeFormat("hh:mm:ss:zzz"),
          mLogDateTime(true),
          mLogLoggerName(true)
    {
    }

    ~LoggerPrivate()
    {
        clearAllLogOutput();
    }
    
    void clearAllLogOutput() 
    {
        qDeleteAll(mLogOutputs.begin(),mLogOutputs.end());
        mLogOutputs.clear();
    }

    QString mName;
    QString mDateTimeFormat;
    QList<LogOutput*> mLogOutputs;
    bool mLogDateTime;
    bool mLogLoggerName;
};

Logger *Logger::instance(const QString &name)
{
    if (name.isEmpty()) {
        LOGGER_LOG("Logger::instance() >> name is empty.");
        return 0;
    }

    QString nameLower(name);
    nameLower.toLower();

    Logger *logger = theLoggerList()->value(nameLower);
    if (!logger) {
        logger = new Logger(nameLower);
        theLoggerList()->insert(nameLower,logger);
    }

    return logger;
}

void Logger::closeAll()
{
    qDeleteAll(theLoggerList()->begin(),theLoggerList()->end());
    theLoggerList()->clear();
}

void Logger::close(const QString &name)
{
    QString nameLower(name);
    nameLower.toLower();
    delete theLoggerList()->take(nameLower);
}

Logger::Logger(const QString &name /*= QString()*/,QObject *parent /*= 0*/) : QObject(parent),
    d_ptr(new LoggerPrivate(name))
{
    
}

Logger::~Logger()
{
    delete d_ptr;
}

void Logger::log(const QString &log)
{
    if (d_ptr->mLogOutputs.isEmpty()) {
        return;
    }

    QString logStr;

    if (d_ptr->mLogDateTime) {
        logStr += QDateTime::currentDateTime().toString(d_ptr->mDateTimeFormat) + ' ';
    }
    
    if (d_ptr->mLogLoggerName) {
        logStr += '[';
        logStr += name();
        logStr += "]:";
    }

    logStr += log;
    logStr += LINE_SEPERATOR;

    foreach(LogOutput *logOutput, d_ptr->mLogOutputs) {
        logOutput->output(logStr);
    }
}

void Logger::configure(const QString &configFile,QSettings::Format format /*= QSettings::NativeFormat*/)
{
    if (QFileInfo(configFile).exists()) {
        QSettings settings(configFile,format);
        configure(settings);
    }
}

void Logger::configure(QSettings &settings)
{
    clearAllLogOutput();

    settings.beginGroup(name());
    QString strOutput = settings.value(LOGGER_OUTPUT).toString();
    d_ptr->mLogDateTime = settings.value(LOGGER_LOGDATETIME,true).toBool();
    d_ptr->mLogLoggerName = settings.value(LOGGER_LOGLOGGERNAME,true).toBool();
    QString strDateTimeFormat = settings.value(LOGGER_DATETIMEFORMAT).toString();
    if (!strDateTimeFormat.isEmpty()) {
        d_ptr->mDateTimeFormat = strDateTimeFormat;
    }
    settings.endGroup();

    QStringList outputNameList = strOutput.split(' ',QString::SkipEmptyParts);
    foreach(const QString &outputName,outputNameList) {
        LogOutput *output = LogOutputFactory::createLogOutput(outputName);
        if (output) {
            output->setParentLogger(this);
            if (output->load(settings)) {
                addLogOutput(output);
            }
            else {
                delete output;
            }
        }
    }
}

bool Logger::addLogOutput(LogOutput *output)
{
    if (!output) {
        LOGGER_LOG("Logger::addLogOutput >> output is NULL.");
        return false;
    }

    if ( d_ptr->mLogOutputs.indexOf(output) >= 0) {
        LOGGER_LOG("Logger::addLogOutput >> output is already in list.");
        return false;
    }

    if (logOutput(output->name())) {
        LOGGER_LOG( QString("Logger::addLogOutput >> output with the name <") + output->name() + "> is already in list.");
    }

    output->setParentLogger(this);
    d_ptr->mLogOutputs.append(output);

    return true;
}

void Logger::removeLogOutput(LogOutput *output)
{
    d_ptr->mLogOutputs.removeAll(output);
}

LogOutput *Logger::logOutput(const QString &name)
{
    foreach(LogOutput *output,d_ptr->mLogOutputs) {
        if (output->name().compare(name,Qt::CaseInsensitive) == 0) {
            return output;
        }
    }

    return 0;
}

void Logger::clearAllLogOutput()
{
    d_ptr->clearAllLogOutput();
}

QString Logger::name() const
{
    return d_ptr->mName;
}

bool Logger::logDateTime() const
{
    return d_ptr->mLogDateTime;
}

void Logger::setLogDateTime(bool on)
{
    d_ptr->mLogDateTime = on;
}

bool Logger::logLoggerName() const
{
    return d_ptr->mLogLoggerName;
}

void Logger::setLogLoggerName(bool on)
{
    d_ptr->mLogLoggerName = on;
}

QString Logger::dateTimeFormat() const
{
    return d_ptr->mDateTimeFormat;
}

void Logger::setDateTimeFormat(const QString &format)
{
    d_ptr->mDateTimeFormat = format;
}
