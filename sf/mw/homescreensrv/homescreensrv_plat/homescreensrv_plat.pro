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

TEMPLATE = subdirs
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>"

include(contentstorage_api/contentstorage_api.pri)
include(hswidgetmodel_api/hswidgetmodel_api.pri)
include(menu_sat_interface_api/menu_sat_interface_api.pri)
include(homescreen_information_api/homescreen_information_api.pri)
include(taskswitcher_api/taskswitcher_api.pri)
include(activity_framework_api/activity_framework_api.pri)

for(filename,PLATFORM_HEADERS){
    BLD_INF_RULES.prj_exports *= "$$filename MW_LAYER_PLATFORM_EXPORT_PATH($$basename(filename))"
  }
