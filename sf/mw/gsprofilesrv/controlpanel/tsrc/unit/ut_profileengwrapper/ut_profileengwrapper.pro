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
TARGET = ut_profileengwrapper
DEPENDPATH += . src
INCLUDEPATH += . ../../../src/cpplugins/profileenginewapper/src

QT += testlib
CONFIG += qtestlib
TARGET.CAPABILITY = ALL -TCB
symbian:LIBS += -lprofileenginewapper 

unix {
    test.commands = /epoc32/RELEASE/WINSCW/udeb/ut_profileengwrapper.exe
    autotest.commands = /epoc32/RELEASE/WINSCW/udeb/ut_profileengwrapper.exe -xml -o c:/ut_profileengwrapper.xml
} else:win32 {
    test.CONFIG += recursive
    autotest.CONFIG += recursive
    build_pass {
        test.commands =/epoc32/RELEASE/WINSCW/udeb/ut_profileengwrapper.exe
        autotest.commands =/epoc32/RELEASE/WINSCW/udeb/ut_profileengwrapper.exe -xml -o c:/ut_profileengwrapper.xml
        }
}
QMAKE_EXTRA_TARGETS += test autotest

# Input
HEADERS += src/ut_profileengwrapper.h
SOURCES += src/ut_profileengwrapper.cpp


symbian:MMP_RULES += SMPSAFE
