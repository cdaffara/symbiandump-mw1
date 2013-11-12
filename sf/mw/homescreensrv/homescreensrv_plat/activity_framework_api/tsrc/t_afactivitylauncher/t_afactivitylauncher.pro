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

CONFIG += qtestlib console

INCLUDEPATH +=  ./inc \
                mocks/inc/ \
                ../../../../activityfw/afactivitylauncher/inc/ \
                ../../../../activityfw/inc/ \
                ../../../../activityfw/storage/common/inc/ \
                ../../../../activityfw/storage/client/inc/

HEADERS +=  ./inc/t_afactivitylauncher.h \
            ../../../../activityfw/storage/common/inc/afentry.h

SOURCES +=  mocks/src/afapplicationlauncher.cpp \
            ./src/t_afactivitylauncher.cpp \
            ../../../../activityfw/afactivitylauncher/src/afactivitylauncher.cpp \
            ../../../../activityfw/afactivitylauncher/src/afactivitylauncher_p.cpp \
            mocks/src/afstorageclient.cpp \
            ../../../../activityfw/storage/common/src/afentry.cpp
            
            

LIBS += -linetprotutil \
        -lestor \
        -lapgrfx \
        -lws32 \

symbian:!coverage: CONFIG += symbian_test


symbian:MMP_RULES += SMPSAFE
