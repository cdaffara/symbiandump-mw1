#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: 
#

TEMPLATE = lib
TARGET = activitytsplugin

CONFIG += plugin 

CONFIG += mobility
MOBILITY = serviceframework

CONFIG += qtservice 
QTSERVICE.DESCRIPTOR = data/activitytsplugin.xml

INCLUDEPATH +=  inc \
                ../../../inc \
                ../utils/inc

HEADERS +=  inc/activitytsplugin.h \
            inc/activitytsmodel.h \
            inc/activitytsentry.h \
            
SOURCES +=  src/activitytsplugin.cpp \
            src/activitytsmodel.cpp \
            src/activitytsentry.cpp \
            

symbian {
    load(data_caging_paths)
    pluginDep.sources = activitytsplugin.dll
    pluginDep.path = $$QT_PLUGINS_BASE_DIR    
    DEPLOYMENT += pluginDep
    
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    
    LIBS += -lfbscli
    LIBS += -lxqsettingsmanager

    # service framework XML deployment/export
    xml.sources = ./data/activitytsplugin.xml
    xml.path = $$RESOURCE_FILES_DIR/taskswitcher
    
    DEPLOYMENT += xml

    #temporary workaround
    BLD_INF_RULES.prj_exports += "data/activitytsplugin.xml z:/resource/taskswitcher/activitytsplugin.xml"
    
    MMP_RULES += SMPSAFE
}
