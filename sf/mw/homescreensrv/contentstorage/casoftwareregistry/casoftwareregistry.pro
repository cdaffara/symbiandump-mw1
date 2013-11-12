#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
CONFIG += hb
DEFINES += CACLIENT_LIB

include(../../common.pri)
include(casoftwareregistry.pri)

DEPENDPATH += ./inc \
              ./src
INCLUDEPATH += ./inc \
            ../inc \
            ../cautils/inc
            
symbian: {

TARGET.UID2 = 0x1000008D
TARGET.UID3 = 0x2002DCF5

TARGET.CAPABILITY = CAP_GENERAL_DLL
TARGET.VENDORID = VID_DEFAULT

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

LIBS += -leuser \
        -lbafl \
        -lestor \
        -lscrclient \
        -lsifnotification \
        -lxqutils \
        -lPlatformEnv \
        -lefsrv \
        -lcautils
        
include(casoftwareregistry_s60.pri)
}

!symbian: {
DEPENDPATH += ./stub/inc \
              ./stub/src
INCLUDEPATH += ./stub/inc

include(casoftwareregistry_stub.pri)
}

symbian:MMP_RULES += SMPSAFE
