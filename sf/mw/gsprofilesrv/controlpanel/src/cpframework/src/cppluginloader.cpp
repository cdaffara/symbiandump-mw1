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
#include <cppluginloader.h>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>
#include "cputility.h"
#include <cplogger.h>


/*!
    \class CpPluginLoader
    \brief The CpPluginLoader class loads a controlpanel plugin at run-time.
 */

#ifdef WIN32
    #define PLUGINFILE_SUFFIX "dll"
#else
    #define PLUGINFILE_SUFFIX "qtplugin"
#endif

/*
 *  Load the root component object of the plugin from @pluginFile
 *  if @pluginFile is an absoulte file path, load it directly, if is a 
 *  file name, load the root component from path /resource/qt/plugins/controlpanel
*/
static QObject* loadPluginInterface(const QString &pluginFile)
{
    CPPERF_LOG( QLatin1String("Loading plugin: ") + pluginFile );
    
    QFileInfo fileInfo(pluginFile);

    // scan the plugin file from path /resource/qt/plugins/controlpanel
    if (!fileInfo.isAbsolute()) {
        QString fileName = fileInfo.fileName();
        if (fileInfo.suffix().compare(PLUGINFILE_SUFFIX,Qt::CaseInsensitive)) {
            fileName = fileInfo.baseName() + '.' + PLUGINFILE_SUFFIX;
        }

		QStringList pluginDirs = CpUtility::pluginDirectories();
		foreach(const QString &pluginDir,pluginDirs) {
			fileInfo.setFile(pluginDir + fileName);
			// found a valid plugin file.
			if (fileInfo.exists() && QLibrary::isLibrary(fileInfo.absoluteFilePath())) {
			    CPPERF_LOG( QLatin1String("Valid plugin stub found: ") + fileInfo.absoluteFilePath() );
				break;
			}
		}
    }

	QPluginLoader loader(fileInfo.absoluteFilePath());
	QObject *plugin = loader.instance();
	if (!plugin) {
		loader.unload();
	}
	
	CPPERF_LOG( QLatin1String("Load plugin ") + (plugin ? QLatin1String("succeed.") : QLatin1String("failed.")) );
    
    return plugin;
}

/*!
    load a CpPluginInterface by a controlpanel plugin file.
    the plugin file can either absoulte file path or only file name.
    acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
 */
CpPluginInterface *CpPluginLoader::loadCpPluginInterface(const QString &pluginFile)
{
    return qobject_cast<CpPluginInterface*>(::loadPluginInterface(pluginFile));
}

/*!
    load a CpLauncherInterface by a controlpanel plugin file.
    the plugin file can either absoulte file path or only file name.
    acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
 */
CpLauncherInterface *CpPluginLoader::loadCpLauncherInterface(const QString &pluginFile)
{
    return qobject_cast<CpLauncherInterface*>(::loadPluginInterface(pluginFile));  
}


