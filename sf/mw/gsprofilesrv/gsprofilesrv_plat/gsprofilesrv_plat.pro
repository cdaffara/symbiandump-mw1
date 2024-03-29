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

SYMBIAN_PLATFORMS = WINSCW ARMV5

CONFIG += ordered

SUBDIRS += controlpanel_api ftuwizardmodel_api

symbian: {
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"./profiles_engine_api/group/bld.inf\""
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"./settings_uis_cenrep_collection_api/group/bld.inf\""
}

