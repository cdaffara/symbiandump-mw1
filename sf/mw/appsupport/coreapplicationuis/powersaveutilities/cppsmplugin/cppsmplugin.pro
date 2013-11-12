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
# Description:  Project file for the RFS Plugin. It creates the bld.inf and .mmp file on executing
# qmake command.
#  
#

TEMPLATE = lib
TARGET = cppsmplugin
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

LIBS += -lcpframework -lpsmclient -lcentralrepository -lcenrepnotifhandler
HEADERS += src/cppsmplugin.h \
           src/cppsmview.h 
    
SOURCES += src/cppsmplugin.cpp \
           src/cppsmview.cpp 
    
symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += PLUGINUID3=0x2000E51E
    TARGET.UID3 = 0x2000E51E
}

symbian { 
    deploy.path = C:
    qtplugins.path = /resource/qt/plugins/controlpanel
    qtplugins.sources += qmakepluginstubs/cppsmplugin.qtplugin    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
}
# common translation file for all plugins
TRANSLATIONS = powermanagement.ts

# Build.inf rules
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "rom/cppsmplugin.iby CORE_MW_LAYER_IBY_EXPORT_PATH(cppsmplugin.iby)" \
  ".\rom\cppsmplugin_resources.iby 						LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(cppsmplugin_resources.iby)"
TARGET.CAPABILITY = All -TCB
plugin.sources = cppsmplugin.dll
plugin.path = \resource\qt\plugins\controlpanel
DEPLOYMENT += plugin	
