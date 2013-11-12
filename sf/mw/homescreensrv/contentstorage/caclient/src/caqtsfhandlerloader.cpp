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

#include <QApplication>
#include <QDir>
#include <QMap>
#include <QScopedPointer>
#include <QString>
#include <qservice.h>
#include <qstringlist.h>
#include <qservicemanager.h>

#include "cahandler.h"
#include "caqtsfhandlerloader.h"
#include "caclient_defines.h"

QTM_USE_NAMESPACE

/*!
    \class CaQtSfHandlerLoader
    \ingroup
    \brief Loads handlers implementation

    The purpose of the class is to find Qt SF plugins implementing command handlers.
    Temporarily because of issues with Qt SF this is replaced by immediate object
    creation.
    \sa CaHandlerLoader
*/

/*!
    Constructor.
*/
CaQtSfHandlerLoader::CaQtSfHandlerLoader()
{
    registerPlugins();
}

/*!
    Load plugins for command handling.
*/
void CaQtSfHandlerLoader::registerPlugins() const
{
    const QString pluginPath("/resource/qt/plugins/commandhandler");
    const QFileInfoList drives = QDir::drives();
    
    foreach (QFileInfo drive, drives) {
        const QString driveLetter = drive.absolutePath();
        const QString pluginAbsolutePath = driveLetter + pluginPath;
        const QDir pluginDir(pluginAbsolutePath);
        if (QDir(pluginDir).exists()) {
            const QFileInfoList fileInfos = 
                pluginDir.entryInfoList(QStringList("*.xml"));
            
            QApplication::addLibraryPath(pluginAbsolutePath);
            
            QServiceManager serviceManager;
            foreach(QFileInfo fileInfo, fileInfos) {
                serviceManager.addService(fileInfo.absoluteFilePath());
            }
        }
    }
}

/*!
    Loads handler implementations appropriate for the requested entry type name and command.

    The caller takes ownership of the returned pointer.

    \param entryTypeName Entry type name.
    \param commandName Name of the command to be handled.
    \return A pointer to handler serving the entry type and command if found, NULL otherwise.
*/
CaHandler *CaQtSfHandlerLoader::loadHandler(const QString &entryTypeName,
        const QString &commandName)
{
    Q_UNUSED(commandName);

    QString typeName(entryTypeName);
    if (entryTypeName == WIDGET_ENTRY_TYPE_NAME
        || entryTypeName == PACKAGE_ENTRY_TYPE_NAME) {
        typeName = QString(APPLICATION_ENTRY_TYPE_NAME);
    }

    QServiceManager serviceManager;
    QServiceFilter serviceFilter("com.nokia.homescreen.ICommandHandler");
    serviceFilter.setCustomAttribute("entryTypeName", typeName);
    QList<QServiceInterfaceDescriptor> serviceInterfaceDescriptorList =
        serviceManager.findInterfaces(serviceFilter);
    CaHandler *interfaceHandler = NULL;
    if (!serviceInterfaceDescriptorList.isEmpty()) {
        QServiceInterfaceDescriptor serviceInterfaceDescriptor =
            serviceInterfaceDescriptorList[0];
         QObject *handler =
            serviceManager.loadInterface(serviceInterfaceDescriptor);
         interfaceHandler = qobject_cast<CaHandler *>(handler);
    }
    return interfaceHandler;
}


