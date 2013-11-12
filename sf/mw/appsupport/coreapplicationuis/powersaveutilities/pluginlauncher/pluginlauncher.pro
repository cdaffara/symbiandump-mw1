#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: cpframework source files
#

TEMPLATE = app
TARGET = CpPsmPluginLauncher
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += src/*.h
SOURCES += src/*.cpp

CONFIG += hb
CONFIG += no_icon

MOC_DIR = moc
OBJECT_DIR = obj
RCC_DIR = rcc

symbian: {
    TARGET.UID3 = 0x2002DD0C 
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    TARGET.CAPABILITY =  ReadDeviceData  WriteDeviceData
    TARGET.EPOCALLOWDLLDATA = 1
}

LIBS +=  -lcpframework
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "rom/pluginlauncher.iby CORE_MW_LAYER_IBY_EXPORT_PATH(pluginlauncher.iby)" 