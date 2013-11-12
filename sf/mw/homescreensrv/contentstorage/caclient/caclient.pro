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
# Description: caclient.pro
#

TEMPLATE = lib
CONFIG += hb mobility
HB = hbcore

MOBILITY = serviceframework

DEFINES += CACLIENT_LIB

include(../../common.pri)

DEPENDPATH += ./inc \
              ./src
INCLUDEPATH += ./inc 

include(caclient.pri)

symbian: {
DEPENDPATH += ./s60/inc \
              ./s60/src
INCLUDEPATH += ./s60/inc

TARGET.UID3 = 0x20022F81

INCLUDEPATH += \
             ../cautils/inc \
             ../extinc \
             ../inc

LIBS += -lavkon \
        -lcautils \
        -lestor \
        -lbafl \
        -laknicon \
        -lcharconv \
        -lfbscli \
        -lbitgdi \
        -lxqutils \
        -lsif \
        -lscrclient \
        -laknicon \
        -lapgrfx \
        -lapparc \
        -lSatClient

include(caclient_s60.pri)
}

!symbian: {
DEPENDPATH += ./stub/inc \
              ./stub/src
INCLUDEPATH += ./stub/inc

QT += sql

include(caclient_stub.pri)
include(installs_win32.pri)
}


symbian:MMP_RULES += SMPSAFE
