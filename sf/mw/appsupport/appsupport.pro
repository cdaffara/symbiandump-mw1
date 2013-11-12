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
# Description: Project file for building phonesrv components
#
#

TEMPLATE = subdirs
CONFIG += ordered

symbian: {
:BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"group/bld.inf\""
SUBDIRS += coreapplicationuis\devicepowermenuplugin
SUBDIRS += systemsettings\accindicatorplugin
SUBDIRS += systemsettings\accindicatorplugin\accindicatorsettings
SUBDIRS += coreapplicationuis\powersaveutilities
SUBDIRS += coreapplicationuis\Rfs\Plugins\cprfsplugin
SUBDIRS += coreapplicationuis\hbdeviceinputdialogplugin
}
