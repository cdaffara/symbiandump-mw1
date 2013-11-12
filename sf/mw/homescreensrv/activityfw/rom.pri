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

BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>"
BLD_INF_RULES.prj_exports += "rom/activitymanager_core.iby  CORE_MW_LAYER_IBY_EXPORT_PATH(activitymanager_core.iby)"
BLD_INF_RULES.prj_exports += "./sis/stubs/activitymanager_stub.sis  /epoc32/release/winscw/udeb/z/system/install/activitymanager_stub.sis"
BLD_INF_RULES.prj_exports += "./sis/stubs/activitymanager_stub.sis  /epoc32/data/z/system/install/activitymanager_stub.sis"