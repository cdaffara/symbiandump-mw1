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
TARGET = testplugin

CONFIG += hb plugin
CONFIG += symbian_test

QMAKE_EXTRA_TARGETS += test autotest

LIBS += -lcpframework

include ( testplugin.pri )

symbian: { 
    TARGET.UID3 = 0X2002D6E9
}

MOC_DIR = moc
OBJECT_DIR = obj

symbian: {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += $$MW_LAYER_PLATFORM_EXPORT_PATH(cplogger)
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += $$MOC_DIR
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    
    deploy.path = C:
    headers.sources = data/testplugin.cpcfg
    headers.path = /resource/qt/plugins/controlpanel/config
    DEPLOYMENT += headers

    # This is for new exporting system coming in garden
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "./$$header $$deploy.path$$headers.path/$$basename(header)"
}

symbian: plugin { # copy qtstub and manifest
 
    PLUGIN_STUB_PATH = /resource/qt/plugins/controlpanel
  
    deploy.path = C:
    pluginstub.sources = $${TARGET}.dll
    pluginstub.path = $$PLUGIN_STUB_PATH
    DEPLOYMENT += pluginstub 

    qtplugins.path = $$PLUGIN_STUB_PATH
    qtplugins.sources += qmakepluginstubs/$${TARGET}.qtplugin
     
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin  $$deploy.path$$qtplugins.path/$$basename(qtplugin)" 
}





symbian:MMP_RULES += SMPSAFE
