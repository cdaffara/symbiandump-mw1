# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description:

TEMPLATE = app
TARGET = ut_cpapi

DEPENDPATH += .
INCLUDEPATH += src/ \
               ../../../src/inc \
               ../../../controlpanel_plat/inc
               
include (../ut_common.pri)

# Input
HEADERS += src/ut_cpapi.h 
HEADERS += src/cptestview.h 
HEADERS += src/cptestpluginentryitem.h
SOURCES += src/ut_cpapi.cpp
SOURCES += src/cptestview.cpp 
SOURCES += src/cptestpluginentryitem.cpp

symbian*: { 
    TARGET.CAPABILITY = CAP_APPLICATION
    LIBS += -lfirstpluginfortest \
           -lsecondpluginfortest \
           -lthirdpluginfortest
    deploy.path = C:
    testdlls.sources += firstpluginfortest.dll secondpluginfortest.dll thirdpluginfortest.dll
    testdlls.path = /sys/bin
    
    testqtplugins.sources += ../pluginfortest/firstpluginfortest/qmakepluginstubs/firstpluginfortest.qtplugin \
                             ../pluginfortest/secondpluginfortest/qmakepluginstubs/secondpluginfortest.qtplugin \
                             ../pluginfortest/thirdpluginfortest/qmakepluginstubs/thirdpluginfortest.qtplugin 
    testqtplugins.path = /resource/qt/plugins/controlpanel                        
    
    DEPLOYMENT += testdlls testqtplugins
}



symbian:BLD_INF_RULES.prj_exports += "data/controlpanellog.conf C:/data/.config/ut_cpapi/controlpanellog.conf"

unix { 
    test.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe
    autotest.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe -xml -o c:/ut_cpapi.xml
}
else:win32 { 
    test.CONFIG += recursive
    autotest.CONFIG += recursive
    build_pass { 
        test.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe
        autotest.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe -xml -o c:/ut_cpapi.xml
    }
}

symbian:MMP_RULES += SMPSAFE
