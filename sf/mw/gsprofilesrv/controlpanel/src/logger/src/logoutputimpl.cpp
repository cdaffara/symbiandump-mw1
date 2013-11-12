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
#include "logoutputimpl.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QSettings>
#include "logger.h"
#include "loglogger.h"
#include "logoutputfactory.h"

#ifdef Q_WS_WIN
    #include <Windows.h>
#endif

#ifdef Q_OS_SYMBIAN
    #include <e32debug.h>
#endif

//DebugLogOutput
REGISTER_OUTPUT_LOG(DEBUGOUTPUT_NAME,DebugLogOutput)

DebugLogOutput::DebugLogOutput() 
{
}

DebugLogOutput::~DebugLogOutput()
{
}

void DebugLogOutput::output(const QString &log)
{
#if defined(Q_WS_WIN)
    QT_WA({
        OutputDebugStringW(reinterpret_cast<const WCHAR*>(log.utf16()));
    }, {
        OutputDebugStringA(log.toLocal8Bit().data());
    });
#elif defined(Q_OS_SYMBIAN)
    RDebug::Printf(log.toLocal8Bit().data());
#else
    fprintf(stderr, log.toLocal8Bit().data());
    fflush(stderr);
#endif
}

bool DebugLogOutput::init()
{
    return true;
}


bool DebugLogOutput::doLoad(QSettings &settings)
{
    Q_UNUSED(settings);
    return true;
}


//ConsoleLogOutput

REGISTER_OUTPUT_LOG(CONSOLEOUTPUT_NAME,ConsoleLogOutput)

ConsoleLogOutput::ConsoleLogOutput() 
{
}

ConsoleLogOutput::~ConsoleLogOutput()
{
}

void ConsoleLogOutput::output(const QString &log)
{
    printf(log.toLocal8Bit().data());
}

bool ConsoleLogOutput::init()
{
    return true;
}


bool ConsoleLogOutput::doLoad(QSettings &settings)
{
    Q_UNUSED(settings);
    return true;
}

//FileLogOutput

REGISTER_OUTPUT_LOG(FILEOUTPUT_NAME,FileLogOutput)

FileLogOutput::FileLogOutput(const QString &logFilePath /*= QString()*/) : 
    mLogFile(0),mTextStream(0),mLogFilePath(logFilePath),mTruncateFile(false)
{
}

FileLogOutput::~FileLogOutput()
{
    delete mTextStream;
    delete mLogFile;
}

void FileLogOutput::output(const QString &log)
{
    *mTextStream << log;
    mTextStream->flush();
}

bool FileLogOutput::init()
{
    if (mTextStream) {
        delete mTextStream;
        mTextStream = 0;
    }

    if (mLogFile) {
        delete mLogFile;
        mLogFile = 0;
    }

    QFileInfo fileInfo(mLogFilePath);
    QDir parentDir = fileInfo.dir();
    if (!parentDir.exists())
    {
        QString path = parentDir.absolutePath();
        parentDir.cdUp();
        parentDir.mkpath(path);
    }

    mLogFile = new QFile(mLogFilePath);
    QFile::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;

    if (mTruncateFile) {
        mode |= QIODevice::Truncate;
    }
    else {
        mode |= QIODevice::Append;
    }

    if (!mLogFile->open(mode))
    {
        LOGGER_LOG(QString("FileLogOutput::init << Open open file failed: ") + mLogFilePath);
        return false;
    }
    mTextStream = new QTextStream(mLogFile);
    return true;
}

bool FileLogOutput::doLoad(QSettings &settings)
{
    if (!parentLogger()) {
        return false;
    }

    settings.beginGroup(parentLogger()->name());
    mLogFilePath = settings.value(
        FILEOUTPUT_NAME + SETTINGS_SECTION_SEPERATOR + FILEOUTPUT_LOGFILE).toString();
    mTruncateFile = settings.value(
        FILEOUTPUT_NAME + SETTINGS_SECTION_SEPERATOR + FILEOUTPUT_TRUNCATE,QVariant(false)).toBool();
    settings.endGroup();

    if (mLogFilePath.isEmpty()) {
        return false;
    } 

    return true;
}

bool FileLogOutput::truncate() const
{
    return mTruncateFile;
}

void FileLogOutput::setTruncate(bool truncate)
{
    mTruncateFile = truncate;
}

QString FileLogOutput::logFile() const
{
    return mLogFilePath;
}

void FileLogOutput::setLogFile(const QString &logFile)
{
    mLogFilePath = logFile;
}

