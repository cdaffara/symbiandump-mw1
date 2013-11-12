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
# Description:  Project file for FTU wizard model
#

TEMPLATE = lib

DEPENDPATH += . \
              inc
INCLUDEPATH += . \
               inc
               
TARGET = ftuwizardmodel

CONFIG += qt debug_and_release warn_on hb
INCLUDEPATH += ./inc 
DEFINES += FTUWIZARDMODEL_LIB
QMAKE_CXXFLAGS.CW += -w error

# Input
HEADERS += ./inc/*.h

windows {
    CONFIG(release, debug|release) {
        DESTDIR = ../../bin/release/
        OBJECTS_DIR = win32/release/obj
    }

    CONFIG(debug, debug|release) {
        DESTDIR = ../../bin/debug/
        OBJECTS_DIR = win32/debug/obj
    }
}

symbian {
    TARGET.UID3 = 0x20027009
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
#    INCLUDEPATH +=  $$MW_LAYER_SYSTEMINCLUDE
}

#deployment configuration, deploy as a dll.
symbian {
    deploy_ftuwizardmodel.sources = ftuwizardmodel.dll
    deploy_ftuwizardmodel.path = /sys/bin
    DEPLOYMENT += deploy_ftuwizardmodel
    BLD_INF_RULES.prj_exports += \
     "rom/ftuwizardmodel.iby  CORE_MW_LAYER_IBY_EXPORT_PATH(ftuwizardmodel.iby)"
}

#ONLY FOR DEVELOPMENT! REMOVE THIS BEFORE EACH HS_Domain RELEASE!
symbian:MMP_RULES += SMPSAFE
