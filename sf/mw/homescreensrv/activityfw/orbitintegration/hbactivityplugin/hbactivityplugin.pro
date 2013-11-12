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
TARGET = hbactivityplugin

CONFIG += plugin 

CONFIG += mobility
MOBILITY = serviceframework

HEADERS +=  hbactivityplugininterface_p.h \
            hbactivityplugin.h \

SOURCES +=  hbactivityplugin.cpp \

RESOURCES += hbactivityplugin.qrc

symbian {
    load(data_caging_paths)
    pluginDep.sources = hbactivityplugin.dll
    pluginDep.path = $$QT_PLUGINS_BASE_DIR    
    DEPLOYMENT += pluginDep
    TARGET.UID3 = 0x200267B1
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
}


symbian:MMP_RULES += SMPSAFE
