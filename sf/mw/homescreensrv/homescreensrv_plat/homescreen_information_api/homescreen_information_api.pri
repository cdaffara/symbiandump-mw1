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

PLATFORM_HEADERS += \
                    homescreen_information_api/inc/homescreendomainpskeys.h
symbian {    
    #qcrml export
    BLD_INF_RULES.prj_exports += \
         "homescreen_information_api/inc/homescreeninformation.qcrml  c:/resource/qt/crml/homescreeninformation.qcrml "
}