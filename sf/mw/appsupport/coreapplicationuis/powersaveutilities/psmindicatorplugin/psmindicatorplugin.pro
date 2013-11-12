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
# Description: 
#
TEMPLATE = lib
TARGET = psmindicatorplugin
CONFIG += plugin
CONFIG += hb
INCLUDEPATH += .
DEPENDPATH += .
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

# Input
HEADERS += inc/psmindicator.h
SOURCES += src/psmindicator.cpp
RESOURCES = 
symbian { 
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    SYMBIAN_PLATFORMS = WINSCW ARMV5
    TARGET.UID3 = 0x2000F85F
    pluginstub.sources = psmindicatorplugin.dll
    pluginstub.path = /resource/plugins/indicators
    DEPLOYMENT += pluginstub
    LIBS += -lcpframework 

}
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "rom/psmindicatorplugin.iby CORE_MW_LAYER_IBY_EXPORT_PATH(psmindicatorplugin.iby)" 
