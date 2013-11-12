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
# Description: controlpanel project - common qmake settings
#

HEADERS += $$PWD/src/*.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/logger.h  \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/loggerglobal.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/logoutput.h  \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/logoutputfactory.h \
           ../../../gsprofilesrv_plat/controlpanel_api/inc/cplogger.h  
           
SOURCES += $$PWD/src/*.cpp
