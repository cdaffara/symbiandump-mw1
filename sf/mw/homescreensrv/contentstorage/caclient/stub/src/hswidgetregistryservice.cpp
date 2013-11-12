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
* Description:  Manages installed widgets information
*
*/

#include <QDir>
#include <QPluginLoader>
#include <QDebug>

#include "hswidgetregistryservice.h"
#include "hswidgetregistryservice_p.h"
#include "hswidgetcomponentdescriptor.h"
#include "hswidgetcomponentparser.h"


/*!
    ?Qt_style_documentation
*/
HsWidgetRegistryServicePrivate::HsWidgetRegistryServicePrivate(
    const QString &installationPath, HsWidgetRegistryService *ptrToPublic,
    QObject *parent) : QObject(parent)
{
    Q_UNUSED(ptrToPublic);
    QStringList manifestPaths;

    this->mInstallationPath = installationPath;
    QDir currentDir = QDir::current();
    QString currentPath = currentDir.absolutePath();

    //Check widget installation dirs from root of different drives
    QFileInfoList drives = QDir::drives();

    // ?
    for (int i=0; i < drives.count(); i++) {
        QFileInfo drive = drives.at(i);
        QString driveLetter = drive.absolutePath();
        QString path = currentPath + "/" + mInstallationPath;
        QDir registryDir(path);

        if (registryDir.exists()) {
            // ?
            mManifestDirectories[path] = readManifestDirectories(path);
        }
    }
}

/*!
    ?Qt_style_documentation
*/
HsWidgetRegistryServicePrivate::~HsWidgetRegistryServicePrivate()
{

}

/*!
    ?Qt_style_documentation
*/
QList<HsWidgetComponentDescriptor> HsWidgetRegistryServicePrivate::widgets()
{
    QList<HsWidgetComponentDescriptor> widgets;
    QMapIterator<QString, QStringList> i(mManifestDirectories);

    while (i.hasNext()) {
        i.next();
        QStringList manifestFileList = i.value();
        QDir manifestDir(i.key());

        // ?
        for (int h=0; h < manifestFileList.count(); h++) {
            widgets << readManifestFile(manifestDir.absoluteFilePath(manifestFileList.at(h)));
        }
    }
    return widgets;
}

/*!
    ?Qt_style_documentation
*/
/*
IHsWidgetProvider *HsWidgetRegistryServicePrivate::loadProviderFromPlugin(
    const QString &pluginName)
{
    QPluginLoader loader(pluginName);
    QObject *plugin = loader.instance();
    IHsWidgetProvider *provider = qobject_cast<IHsWidgetProvider *>(plugin);

    if (provider) {
        // ?
        return provider;
    }

    // Don't leak memory if provider not IHsWidgetProvider
    if (plugin) {
        //qDebug("Widget provider load - !provider, deleting plugin.")
        delete plugin;
    }

    // qDebug("Widget provider load failed - Not found.")
    return 0;
}
*/

/*!
    ?Qt_style_documentation
*/
QStringList HsWidgetRegistryServicePrivate::readManifestDirectories(const QString &path)
{
    QStringList widgetDirPaths;
    QDir registryDir(path);
    QStringList widgetDirs = registryDir.entryList(QDir::AllDirs);

    // ?
    for (int i=0; i < widgetDirs.count(); ++i) {
        widgetDirPaths << registryDir.absoluteFilePath(widgetDirs.at(i));
    }
    return widgetDirPaths;
}

/*!
    ?Qt_style_documentation
*/
void HsWidgetRegistryServicePrivate::doWidgetRemove(const QString &path,
        const QStringList &originalList, const QStringList &currentList)
{
    Q_UNUSED(path);
    const int originalCount = originalList.count();

    // ?
    for (int i=0; i<originalCount; i++) {
        QString widgetDir = originalList.at(i);

        if (!currentList.contains(widgetDir)) {
            // ?
            QDir dir(widgetDir);
            int widgetUid = dir.dirName().toUInt(0, 16);
            mPublic->emitWidgetRemovedFromRegistry(widgetUid);
        }
    }
}

/*!
    ?Qt_style_documentation
*/
QList<HsWidgetComponentDescriptor> HsWidgetRegistryServicePrivate::readManifestFile(
    const QString &manifestFilePath)
{
    QList<HsWidgetComponentDescriptor> widgets;
    QStringList filters("*.manifest");
    QDir dir(manifestFilePath);
    QStringList manifestDir = dir.entryList(filters, QDir::Files);

    if (!manifestDir.isEmpty()) {
        // ?
        QString fileName = manifestDir.first();
        HsWidgetComponentParser componentParser(dir.absoluteFilePath(fileName));
        if ( !componentParser.error() ) {
            
            HsWidgetComponentDescriptor widgetDescriptor = componentParser.widgetComponentDescriptor();            
            widgetDescriptor.library = manifestFilePath + "/" + widgetDescriptor.uri + ".dll";
            if (widgetDescriptor.iconUri.length() > 0 && widgetDescriptor.iconUri.contains('.')) {
                widgetDescriptor.iconUri = manifestFilePath + "/" + widgetDescriptor.iconUri;
            }            
            if (widgetDescriptor.previewImage.length() > 0 && widgetDescriptor.previewImage.contains('.')) {
                widgetDescriptor.previewImage = manifestFilePath + "/" + widgetDescriptor.previewImage;
            }            

            int widgetUid = dir.dirName().toUInt(0, 16);
            widgetDescriptor.uid = widgetUid;
            widgets << widgetDescriptor;
            qDebug() << "HsWidgetRegistryServicePrivate::readManifestFile - " \
                     "widget added: " << fileName;
        }
    }
    return widgets;
}

/*!
    ?Qt_style_documentation
*/
void HsWidgetRegistryServicePrivate::ensureWidgetRegistryPaths()
{
    /*
    mFileSystemWatcher.disconnect();
    QStringList pathList = mManifestDirectories.keys();

    // ?
    for(int i=0; i < pathList.count(); i++) {
        QDir registryDir(pathList.at(i));

        if(!registryDir.exists()) {
            // ?
            registryDir.mkpath(pathList.at(i));
        }
    }

    mFileSystemWatcher.removePaths(QStringList(mManifestDirectories.keys()));
    mFileSystemWatcher.addPaths(QStringList(mManifestDirectories.keys()));
    connect(&mFileSystemWatcher,SIGNAL(directoryChanged(const QString&)),SLOT(directoryChanged(const QString&)));
    */
}

/*!
    ?Qt_style_documentation
*/
void HsWidgetRegistryServicePrivate::directoryChanged(const QString &path)
{
    Q_UNUSED(path);
    /*
    int installationStatus = mInstallerObserver.value();

    if ((installationStatus & KSASwisOperationMask) == ESASwisNone) {
        // ?
        QStringList originalList = mManifestDirectories.value(path);
        QStringList currentList = readManifestDirectories(path);
        doWidgetAdd(path, originalList, currentList);
        doWidgetRemove(path, originalList, currentList);
        mManifestDirectories[path] = currentList;
        ensureWidgetRegistryPaths();
    } else {
        // ?
        mFileSystemWatcher.disconnect();
        connect(&mInstallerObserver,SIGNAL(valueChanged(int)),SLOT(installerStateChanged(int)));
        mInstallerObserver.subscribe();
        mLatestChangedDirectory = path;
    }
    */
}

/*!
    ?Qt_style_documentation
*/
void HsWidgetRegistryServicePrivate::installerStateChanged(int newValue)
{
    Q_UNUSED(newValue);
    /*
    if ((newValue & KSASwisOperationMask) == ESASwisNone) {
        // ?
        mInstallerObserver.unSubscribe();
        mInstallerObserver.disconnect();
        QStringList originalList = mManifestDirectories.value(mLatestChangedDirectory);
        QStringList currentList = readManifestDirectories(mLatestChangedDirectory);
        doWidgetAdd(mLatestChangedDirectory, originalList, currentList);
        doWidgetRemove(mLatestChangedDirectory, originalList, currentList);
        connect(&mFileSystemWatcher,SIGNAL(directoryChanged(const QString&)),SLOT(directoryChanged(const QString&)));
        mManifestDirectories[mLatestChangedDirectory] = currentList;
        ensureWidgetRegistryPaths();
    }
    */
}


/*!
    \class HsWidgetRegistryService
    \ingroup group_hsruntimeservices
    \brief Manages installed widgets information

    Manages information on available widgets and inform observers on
    registry modifications, i.e installing and uninstalling widgets.

*/

/*!
    Constructor.
    \a installationPath is the path where the widget is installed
    and the \a parent is the parent object.
*/
HsWidgetRegistryService::HsWidgetRegistryService(
    const QString &installationPath,
    QObject *parent)
    :QObject(parent),
     mPrivate(new HsWidgetRegistryServicePrivate(installationPath, this, this))
{

}

/*!
    Destructor.
*/
HsWidgetRegistryService::~HsWidgetRegistryService()
{

}

/*!
    Fetch available widgets information
    Return List of widget tokens.
*/
QList<HsWidgetComponentDescriptor> HsWidgetRegistryService::widgets()
{
    return mPrivate->widgets();
}

/*!
    \fn HsWidgetRegistryService::widgetAddedToRegistry(const QList<HsWidgetToken> &widgetTokenList);
    Emitted when new widgets are added to registry. \a widgetTokenList
    contains list of widget tokens.
*/

/*!
    \fn HsWidgetRegistryService::widgetRemovedFromRegistry(int uid)
    Emitted when a widget is removed from registry

*/


/*!
    \fn HsWidgetRegistryService::posterWidgetRemovedFromRegistry(int posterWidgetId)
    Emitted when a poster widget publisher is removed from cps
    \a posterWidgetId Poster widget id.
*/

/*!
    Emits the widgetAddedToRegistry() signal
    \a widgetsAdded Identifies the added widgets.
*/
void HsWidgetRegistryService::emitWidgetAddedToRegistry(const QList<HsWidgetComponentDescriptor> &widgetsAdded)
{
    emit widgetAddedToRegistry(widgetsAdded);
}


/*!
    Emits the widgetRemovedFromRegistry() signal
    \a uid HsWidget uid.
*/
void HsWidgetRegistryService::emitWidgetRemovedFromRegistry(int uid)
{
    emit widgetRemovedFromRegistry(uid);
}
