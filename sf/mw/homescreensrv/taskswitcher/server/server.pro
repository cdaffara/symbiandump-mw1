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
TARGET = hsrunningappmonitor
CONFIG += hb mobility
MOBILITY = serviceframework

myrssrules = \
"hidden = KAppIsHidden;"
RSS_RULES += myrssrules
    
INCLUDEPATH +=  inc \
                ../inc \
                ../../inc \
                ../utils/inc \
                ../backstepping/inc \
                
SOURCES +=  src/tsenv.cpp \
            src/tsdataobservertask.cpp \
            src/tsdataprovidertask.cpp \
            src/tsrunningappserver.cpp \
            src/tsrunningappsession.cpp \
            src/main.cpp \
            src/tsdatalist.cpp \
            src/tsdatatask.cpp \
            src/tsscreenshotprovider.cpp \
            src/tsmodelitemkey.cpp \
            src/tsmodelitem.cpp \
            src/tsmodel.cpp \
            src/tsrunningappmodel.cpp \
            src/tsstorage.cpp \
            src/tsserializeddataprovider.cpp \
            src/tsservice.cpp \
            src/tsservicesprovider.cpp \
            src/tsservicesproviderconfig.cpp \
            src/tsserviceobserver.cpp \
            src/tsmodelitemkeymsg.cpp \
            src/tswindowgroupsmonitorimp.cpp \
            src/tsrunningappimp.cpp \
            src/tsrunningappstorageimp.cpp \
            src/tsresourcemanagerimp.cpp \
            ../utils/src/tsentrykey.cpp \
            ../utils/src/tsentrykeygenerator.cpp \
            ../utils/src/tsentry.cpp \
            ../utils/src/tsscreenshotmsg.cpp \
            ../utils/src/tsunregscreenshotmsg.cpp \
            ../utils/src/tsvisibilitymsg.cpp \
            ../utils/src/tswindowgroupsobserverbase.cpp \
            ../utils/src/tsthumbnailprovider.cpp \
            ../utils/src/tsidlist.cpp \
            
HEADERS +=  inc/tsenv.h \
            inc/tsdataobservertask.h \
            inc/tsdataprovider.h \
            inc/tsdataprovidertask.h \
            inc/tsrunningappserver.h \
            inc/tsrunningappsession.h \
            inc/tsdatalist.h \
            inc/tsdatastorage.h \
            inc/tsdatatask.h \
            inc/tsscreenshotprovider.h \            
            inc/tsmodelobserver.h \
            inc/tsmodelitemkey.h \
            inc/tsmodelitem.h \
            inc/tsmodel.h \
            inc/tsrunningappmodel.h \
            inc/tsstorage.h \
            inc/tsserializeddataprovider.h \
            inc/tsservice.h \
            inc/tsservicesprovider.h \
            inc/tsservicesproviderconfig.h \
            inc/tsserviceobserver.h \
            inc/tsmodelitemkeymsg.h \
            inc/tswindowgroupsmonitorimp.h \
            inc/tsrunningappimp.h \
            inc/tsrunningappstorageimp.h \
            inc/tsresourcemanagerimp.h \
            ../utils/inc/tsdataobserver.h \
            ../utils/inc/tsentry.h \
            ../utils/inc/tsentrykey.h \
            ../utils/inc/tsentrykeygenerator.h \
            ../utils/inc/tsutils.h \
            ../utils/inc/tsscreenshotmsg.h \
            ../utils/inc/tsunregscreenshotmsg.h \
            ../utils/inc/tsvisibilitymsg.h \
            ../utils/inc/tswindowgroupsobserverbase.h \
            ../utils/inc/tsthumbnailprovider.h \
            ../utils/inc/tsthumbnailobserver.h \
            ../utils/inc/tsidlist.h \

LIBS += -lxqutils \
        -ltsbackstepping \
        -laknicon \
        -lfbscli \          #for CFbsBitmap
        -lestor \           #for RDesReadStream
        -lxqutils \
        -lapgrfx \          #for CApaWindowGroupName
        -lws32 \            #for CWsGraphic
        -lapparc \          #for TApaAppInfo
        -lcaclient \        #for getApplicationIcon
        -lcone

symbian {
    TARGET.UID3 = 0x200267B0
    TARGET.CAPABILITY = All -TCB
}



symbian:MMP_RULES += SMPSAFE
