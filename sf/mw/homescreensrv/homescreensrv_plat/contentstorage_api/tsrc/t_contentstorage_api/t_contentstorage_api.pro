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

TEMPLATE = app
CONFIG += hb mobility
MOBILITY = serviceframework
HB = hbcore
include(../../../../common.pri)
include(../../../../contentstorage/tsrc/testutils/testutils.pri)
CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}

CONFIG += debug_and_release \
    console
QT += testlib xml

DEFINES += CONTENTARSENAL_UNITTEST
DEFINES += CONTENT_ARSENAL_STORAGE_UT

coverage:DEFINES += COVERAGE_MEASUREMENT
DEPENDPATH += . \
    ./inc \
    ./src
INCLUDEPATH += . \
    ./inc \
    ../../../../contentstorage/caclient/inc \
    ../../../../contentstorage_api \
    ../../../../contentstorage/srvinc \
    ../../../../contentstorage/inc \
    ../../../../contentstorage/cautils/inc \
    ../../../../contentstorage/castorage/inc \
    ../../../../contentstorage/inc

LIBS += -lcaclient
   
symbian: {
    CONFIG += symbian_test
    ### get rid of localization warnings
    CONFIG -= hb
    load(hb.prf)
    CONFIG -= symbian_i18n
    ###

    TARGET.UID2 = 0x100039CE
    TARGET.UID3 = 0x2002DCFA
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCSTACKSIZE = 0x14000 // 80kB
    TARGET.EPOCHEAPSIZE = 0x20000 0x1000000 // 128kB - 16MB

    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

    LIBS += \
        -lapgrfx \
        -lefsrv \
        -lws32 \
        -lcautils \
        -lsqldb \
        -lbafl  \
        -lflogger \
        -lsysutil
        
    export.sources = ./data/armv5/*.sisx
    export.path = /testing/data/t_caclient/installs
    
    exportDB.sources = ./data/castorage.db
    exportDB.path = /testing/data/t_caclient/
    
    exportQM.sources = ./data/emailTest.qm
    exportQM.path = /resource/qt/translations/
    
    
    DEPLOYMENT += export \
                exportDB \ 
                exportQM
    
}

include(t_contentstorage_api.pri)
!win32:include(t_contentstorage_api_installs_win32.pri)
!symbian:unix:include(t_contentstorage_api_installs_unix.pri)


symbian:MMP_RULES += SMPSAFE
