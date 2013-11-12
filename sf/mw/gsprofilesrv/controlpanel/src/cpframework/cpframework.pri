
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
# Description: cpframework source files
#

HEADERS += ../../../gsprofilesrv_plat/controlpanel_api/inc/cpglobal.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpbasepath.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpplugininterface.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpsettingformitemdata.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cppluginlauncher.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpbasesettingview.h  \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpitemdatahelper.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpsettingformentryitemdata.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cpsettingformentryitemdataimpl.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cppluginloader.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cppluginutility.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cplauncherinterface.h \
           src/cpviewlauncher.h \
           src/cpbasesettingview_p.h \
           src/cpwatchdog.h \
           src/cputility.h \
           src/cpdataformbuttonentryviewitem.h \
           src/cpdataformlistentryviewitem.h \
           src/cpsettingformentryitemdata_p.h

SOURCES += src/cppluginlauncher.cpp \
		       src/cpviewlauncher.cpp \
		       src/cppluginloader.cpp \
		       src/cpsettingformentryitemdata.cpp \ 
		       src/cpdataformbuttonentryviewitem.cpp \
		       src/cpdataformlistentryviewitem.cpp \
		       src/cpsettingformitemdata.cpp \
		       src/cpbasesettingview.cpp \
		       src/cpbasesettingview_p.cpp \
		       src/cpwatchdog.cpp \
		       src/cpitemdatahelper.cpp \
		       src/cputility.cpp \
		       src/cppluginutility.cpp \
		       src/cpsettingformentryitemdata_p.cpp
