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
#include <logoutputfactory.h>
#include <QHash>
#include <QMetaObject>
#include <QMetaProperty>
#include "logoutput.h"
#include "loglogger.h"

typedef QHash<QString,LogOutputFactory::CREATE_ENTRY> CreateEntryContainer;

Q_GLOBAL_STATIC(CreateEntryContainer, theCreateEntryList)

static QString formalizeName(const QString &name)
{
    QString formalName = name;
    formalName.toLower();
    formalName = formalName.trimmed();
    return formalName;
}

LogOutput *LogOutputFactory::createLogOutput(const QString &name)
{
    return createLogOutput(name,QHash<QString,QVariant>());
}

LogOutput *LogOutputFactory::createLogOutput(const QString &name,
                                             const QHash<QString,QVariant> &properties /*= QHash<QString,QVariant>()*/)
{
    LogOutputFactory::CREATE_ENTRY entry = theCreateEntryList()->value(formalizeName(name));
    
    LogOutput *output = entry ? (*entry)() : 0;

    if (output && !properties.isEmpty()) {
        const QMetaObject *metaObj = output->metaObject();
        int count = metaObj->propertyCount();
        for (int i = 0; i < count; i++) {
            QMetaProperty metaProperty = metaObj->property(i);
            if (metaProperty.isValid() && metaProperty.isWritable()) {
                QVariant var = properties.value(metaProperty.name());
                if (var.isValid()) {
                    metaProperty.write(output,var);
                }
            }
        }
    }

    return output;
}

void LogOutputFactory::addCreateLogOutputEntry(const QString &name,CREATE_ENTRY entry)
{
    if (!entry) {
        return;
    }

    QString formalName = formalizeName(name);

    if (theCreateEntryList()->value(formalName)) {
        LOGGER_LOG(QString("LogOutputFactory::addCreateLogOutputEntry << ") 
            + QString("create entry with the name:") + formalName + QString("already exists."));
        return;
    }

    theCreateEntryList()->insert(formalName,entry);
}
