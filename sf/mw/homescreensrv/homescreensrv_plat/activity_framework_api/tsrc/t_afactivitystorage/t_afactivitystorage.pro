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

SOURCES +=  ./src/t_afactivitystorage.cpp
HEADERS +=  ./inc/t_afactivitystorage.h

SOURCES +=  ../../../activityfw/activityserviceplugin/src/afactivitystorage.cpp \
            ../../../activityfw/activityserviceplugin/src/afactivitystorage_p.cpp \
            ../mocks/src/afstorageproxy.cpp \
            
HEADERS +=  ../../afactivities_global.h \
            ../../afactivitystorage.h \
            ../../../activityfw/activityserviceplugin/inc/afactivitystorage_p.h \
            ../mocks/inc/afstorageproxy.h \

include(../tsrc_common.pri)

symbian:!coverage: CONFIG += symbian_test


symbian:MMP_RULES += SMPSAFE
