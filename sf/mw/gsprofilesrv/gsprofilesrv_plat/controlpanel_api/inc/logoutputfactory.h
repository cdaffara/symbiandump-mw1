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
* Description:  Factory class to create LogOutput.
*
*/
#ifndef LOGOUTPUTFACTORY_H
#define LOGOUTPUTFACTORY_H

class LogOutput;
class QVariant;
class QString;

#include <QHash>
#include <loggerglobal.h>

class LOGGER_EXPORT LogOutputFactory
{
public:
    typedef LogOutput* (*CREATE_ENTRY)();
    static LogOutput *createLogOutput(const QString &name);
    static LogOutput *createLogOutput(const QString &name,const QHash<QString,QVariant> &properties);
    static void addCreateLogOutputEntry(const QString &name,CREATE_ENTRY entry); 
};

class LogOutputCreateEntryRegister
{
public:
    LogOutputCreateEntryRegister(const QString &name,LogOutputFactory::CREATE_ENTRY entry) {
        LogOutputFactory::addCreateLogOutputEntry(name,entry);
    }
};

#define REGISTER_OUTPUT_LOG(name,Class) \
    static LogOutput *create##Class() { \
        LogOutput *output = new Class(); \
        output->setName(name); \
        return output; \
    } \
    LogOutputCreateEntryRegister the##Class##EntryRegister(name,create##Class);

#endif //LOGOUTPUTFACTORY_H
