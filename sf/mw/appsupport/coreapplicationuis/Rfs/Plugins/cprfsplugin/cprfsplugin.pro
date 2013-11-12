#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0""
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
# 
# Contributors:
#
# Description:  Project file for the RFS Plugin. It creates the bld.inf and .mmp file on executing
# qmake command.
#  
#

TEMPLATE = lib
TARGET = cprfsplugin
DEPENDPATH += . inc src
INCLUDEPATH += .
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

CONFIG += hb\
           plugin
           
MOC_DIR =     moc
OBJECTS_DIR = obj
RCC_DIR =     rcc

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
                    ARMV5

LIBS += -lcpframework \
				-lRfs
# Input
HEADERS += inc/cprfsplugin.h \
           inc/cprfssettingsdataformcustomitem.h \
           inc/cprfssettingsform.h \
           inc/cprfsview.h
SOURCES += src/cprfsplugin.cpp \
           src/cprfssettingsdataformcustomitem.cpp \
           src/cprfssettingsform.cpp \
           src/cprfsview.cpp

symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += PLUGINUID3=0x10275117
    TARGET.UID3 = 0x10275117    
}
symbian { 
    deploy.path = C:
    qtplugins.path = /resource/qt/plugins/controlpanel
    qtplugins.sources += qmakepluginstubs/cprfsplugin.qtplugin    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
}
# Build.inf rules
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "rom/cprfssettingsplugin.iby CORE_MW_LAYER_IBY_EXPORT_PATH(cprfssettingsplugin.iby)" 

TARGET.CAPABILITY = All -TCB
plugin.sources = cprfsplugin.dll
plugin.path = \resource\qt\plugins\controlpanel
DEPLOYMENT += plugin	
