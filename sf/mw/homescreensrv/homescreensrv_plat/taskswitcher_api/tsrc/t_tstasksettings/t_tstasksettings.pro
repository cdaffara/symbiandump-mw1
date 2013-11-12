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

QT *= testlib
CONFIG += console   
      
INCLUDEPATH +=  . \
      
HEADERS += inc/t_tstasksettings.h \

SOURCES += src/t_tstasksettings.cpp \

DEFINES += TSTASKMONITOR_TEST

symbian {
    CONFIG += symbian_test
    coverage: CONFIG -= symbian_test
    TARGET.EPOCSTACKSIZE = 0x14000 // 80kB
    TARGET.EPOCHEAPSIZE = 0x20000 0x1000000 // 128kB - 16MB
    
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA=1   
    
    LIBS += -lcone \
            -lapgrfx \      # for RApaLsSession
            -lws32 \        # for RWsSession
            
}

INCLUDEPATH += ../../../../internal/tstestutils/inc
LIBS += -ltstestutils.lib

coverage {
    DEFINES += COVERAGE_MEASUREMENT
    DEFINES += QT_NO_DEBUG  # omit ASSERTS in coverage measurements
}

include(../../../../taskswitcher/client/client.pri)

symbian:MMP_RULES += SMPSAFE