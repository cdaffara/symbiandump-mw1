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
TARGET  = camificon

PLUGIN_SUBDIR = /resource/qt/plugins/iconengines

include (../../common.pri)

QTDIR_build:DESTDIR  = $$QT_BUILD_TREE/plugins/iconengines
target.path += $$[PLUGIN_SUBDIR]
INSTALLS += target

CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)

symbian: {
    TARGET.EPOCALLOWDLLDATA=1
    TARGET.CAPABILITY = All -Tcb
    TARGET = $${TARGET}$${QT_LIBINFIX}
    load(armcc_warnings)

    # load the environment specific feature definitions
    exists($${EPOCROOT}epoc32/tools/qt/mkspecs/features/environment.prf) {
        load($${EPOCROOT}epoc32/tools/qt/mkspecs/features/environment.prf)
    }
    LIBS +=  -laknicon
}

symbian: plugin { # copy qtstub and manifest

    pluginstub.sources = $${TARGET}.dll
    pluginstub.path = $$PLUGIN_SUBDIR

    DEPLOYMENT += pluginstub

    qtplugins.path = $$PLUGIN_SUBDIR
    qtplugins.sources += qmakepluginstubs/$${TARGET}.qtplugin    
    
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin z:$$qtplugins.path/$$basename(qtplugin)"
}

symbian:TARGET.UID3=0x2002DCF8

include(camificonengine.pri)

symbian:MMP_RULES += SMPSAFE
