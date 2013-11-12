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

include(./../../../common.pri)

CONFIG += symbian_i18n hb

TRANSLATIONS=contentstorage.ts

symbian: { 
    DEPENDPATH += ./inc \
        ./src
    INCLUDEPATH += ./inc \
        ../casrvmgr/inc \
        ../../cautils/inc \
        ../../srvinc \
        ../../castorage/inc \
        ../../inc \
        $$MW_LAYER_SYSTEMINCLUDE
    TEMPLATE = lib
    TARGET.UID2 = 0x10009D8D
    TARGET.UID3 = 0x20028708
    TARGET.CAPABILITY = ALL \
        -TCB
    TARGET.VENDORID = VID_DEFAULT
    TARGET.EPOCALLOWDLLDATA = 1
    rssResource = "SOURCEPATH ./data" \
        "START RESOURCE calocalizerscanner.rss" \
        " LANG sc" \
        " TARGETPATH resource/plugins" \
        " TARGET calocalizerscanner.rsc" \
        "END"
    MMP_RULES += rssResource
    HEADERS += ./inc/*.h
    SOURCES += ./src/*.cpp
    LIBS += -leuser \
        -lecom \
        -lcautils \
        -lcamenu \
        -lefsrv \
        -lcautils
}

exportResources(./*.qm, resource/qt/translations)





symbian:MMP_RULES += SMPSAFE
