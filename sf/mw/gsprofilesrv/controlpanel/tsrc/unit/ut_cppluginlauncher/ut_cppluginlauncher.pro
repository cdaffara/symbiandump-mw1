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
TARGET = ut_cppluginlauncher 

include (../ut_common.pri)
include (ut_cppluginlauncher.pri)

symbian: { 
    TARGET.UID3 = 0xEEB2ED33
}

symbian*: {
	deploy.path = C:
	
    testplugindll.sources = testplugin.dll
    testplugindll.path = /sys/bin
	
    testpluginconfig.sources = ../testplugin/data/testplugin.cpcfg
    testpluginconfig.path = /resource/qt/plugins/controlpanel/config
	
	testpluginstub.sources = ../testplugin/qmakepluginstubs/testplugin.qtplugin
    testpluginstub.path = /resource/qt/plugins/controlpanel
	
    DEPLOYMENT += testplugindll testpluginconfig testpluginstub
}


symbian:MMP_RULES += SMPSAFE
