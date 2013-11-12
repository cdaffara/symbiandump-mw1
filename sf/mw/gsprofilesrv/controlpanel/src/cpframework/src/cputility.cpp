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
* Description:  Utility class for cpframework.
*
*/
#include "cputility.h"
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <cpbasepath.h>
#include <cppluginloader.h>
#include <cpplugininterface.h>
#include <cplogger.h>


QStringList CpUtility::drives()
{
	static QStringList drives;

	if (drives.empty()) {
        CPFW_LOG("device drives:");
#ifdef WIN32
		drives.append("C:");
        CPFW_LOG("C:");
#else
		QFileInfoList fileInfoList = QDir::drives();
		foreach(const QFileInfo &fileInfo,fileInfoList) {
			QString str = fileInfo.filePath();
			if (str.length() > 2) {
				str = str.left(2);
			}
			drives.append(str);
            CPFW_LOG(str);
		}
#endif  
	}

	return drives;
}

static QStringList directoriesFromAllDrives(const QString &baseDir)
{
	QStringList dirs;

	QStringList drives = CpUtility::drives();
	foreach(const QString &drive,drives) {
		QString dir = drive + baseDir + QDir::separator();
		if (QFileInfo(dir).exists()) {
			dirs.append(dir);
            CPFW_LOG(dir);
		}
	}

	return dirs;
}

QStringList CpUtility::pluginDirectories()
{
	static QStringList dirs;
	if (dirs.empty()) {
        CPFW_LOG("ControlPanel plugin derectories:")
		dirs = directoriesFromAllDrives(CP_PLUGIN_PATH);
	}
	return dirs;
}

QStringList CpUtility::configFileDirectories()
{
	static QStringList dirs;
	if (dirs.empty()) {
        CPFW_LOG("ControlPanel configuration file derectories:");
		dirs = directoriesFromAllDrives(CP_PLUGIN_CONFIG_PATH);
	}
	return dirs;
}

