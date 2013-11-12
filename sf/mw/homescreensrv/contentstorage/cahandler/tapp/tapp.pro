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
TARGET = catapphandlerplugin
CONFIG += plugin mobility
MOBILITY = serviceframework

PLUGIN_SUBDIR = /resource/qt/plugins/commandhandler


include (../../../common.pri)


DEPENDPATH += ./inc \
              ./src
			  
INCLUDEPATH += ./inc \
    ../../caclient/inc \
    ../../caclient/s60/inc \
    ../../cautils/inc


symbian: {
    TARGET.UID3 = 0x20028710

	
	LIBS += -lxqservice \
            -lxqutils \
            -lcautils \
            -lcaclient

}


include(tapp.pri)


symbian:MMP_RULES += SMPSAFE
