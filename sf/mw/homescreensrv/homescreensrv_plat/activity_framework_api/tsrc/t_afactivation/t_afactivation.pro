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

INCLUDEPATH +=  ./inc \
                ../mocks/inc
                
SOURCES +=  ./src/t_afactivation.cpp 
HEADERS +=  ./inc/t_afactivation.h

SOURCES +=  ../../../activityfw/activityserviceplugin/src/aflauncher.cpp \
            ../../../activityfw/activityserviceplugin/src/afactivation.cpp \
            ../../../activityfw/activityserviceplugin/src/afactivation_p.cpp \
            ../../../activityfw/activityserviceplugin/src/afcommandlineparser.cpp \
            ../mocks/src/applicationlauncher_p.cpp \
            ../mocks/src/afstorageproxy.cpp \
            
HEADERS +=  ../../../activityfw/activityserviceplugin/inc/aflauncher.h \
            ../../afactivities_global.h \
            ../../afactivation.h \
            ../../../activityfw/activityserviceplugin/inc/afactivation_p.h \
            ../../../activityfw/activityserviceplugin/inc/afcommandlineparser.h \
            ../mocks/inc/applicationlauncher_p.h \
            ../mocks/inc/afstorageproxy.h \

include(../tsrc_common.pri)

symbian:!coverage:CONFIG += symbian_test


symbian:MMP_RULES += SMPSAFE
