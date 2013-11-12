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
TARGET = ut_cpplugin
QT += testlib
CONFIG += hb qtestlib

unix {
    test.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpplugin.exe
    autotest.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpplugin.exe -xml -o c:/ut_cpplugin.xml
} else:win32 {
    test.CONFIG += recursive
    autotest.CONFIG += recursive
    build_pass {
        test.commands =/epoc32/RELEASE/WINSCW/udeb/ut_cpplugin.exe
        autotest.commands =/epoc32/RELEASE/WINSCW/udeb/ut_cpplugin.exe -xml -o c:/ut_cpplugin.xml
        }
}
QMAKE_EXTRA_TARGETS += test autotest



DEPENDPATH += .

INCLUDEPATH += . \
							 ../../../src/inc \
               ../../../src/cpframework/src \
               ../../../controlpanel_plat/inc

LIBS += -lcpframework

# Input
HEADERS += src/ut_cpplugin.h
HEADERS += ../../../src/cpframework/src/cppluginloader.h
HEADERS += ../../../src/cpframework/src/cppluginconfigreader.h
HEADERS += ../../../src/cpframework/src/cputility.h
SOURCES += src/ut_cpplugin.cpp
SOURCES += ../../../src/cpframework/src/cppluginloader.cpp
SOURCES += ../../../src/cpframework/src/cppluginconfigreader.cpp
SOURCES += ../../../src/cpframework/src/cppluginconfig.cpp
SOURCES += ../../../src/cpframework/src/cputility.cpp

symbian:MMP_RULES += SMPSAFE
