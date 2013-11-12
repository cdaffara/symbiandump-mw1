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
* Description:  The private implementation of the HsWidgetRegistryService.
*
*/


#ifndef HSWIDGETREGISTRYSERVICE_P_H
#define HSWIDGETREGISTRYSERVICE_P_H

#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <QMap>
#include <QFileSystemWatcher>

#include "hswidgetcomponentdescriptor.h"

class HsWidgetRegistryService;

class HsWidgetRegistryServicePrivate : public QObject
{
    Q_OBJECT

public:

    HsWidgetRegistryServicePrivate(const QString &installationPath,
                                   HsWidgetRegistryService *ptrToPublic, QObject *parent=0);

    ~HsWidgetRegistryServicePrivate();

    QList<HsWidgetComponentDescriptor> widgets();

private:

    Q_DISABLE_COPY(HsWidgetRegistryServicePrivate)

//    IHsWidgetProvider *loadProviderFromPlugin(const QString &pluginName);

    QStringList readManifestDirectories(const QString &path);

    void doWidgetRemove(const QString &path, const QStringList &originalList,
                        const QStringList &currentList);

    QList<HsWidgetComponentDescriptor> readManifestFile(const QString &path);

    void ensureWidgetRegistryPaths();

public slots:

private slots:

    void directoryChanged(const QString &path);

    void installerStateChanged(int newValue);

private:

    QString mInstallationPath;

    QMap<QString,QStringList> mManifestDirectories;

    HsWidgetRegistryService *mPublic;
};

#endif //HSWIDGETREGISTRYSERVICE_P_H
