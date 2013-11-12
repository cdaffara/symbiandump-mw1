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
# Description:  This file provides the information required for building
#                General Settings.
#
#

symbian {
    BLD_INF_RULES.prj_exports += \
    "$${LITERAL_HASH}include<platform_paths.hrh>"\
    "conf/s60settingsuis.confml           MW_LAYER_CONFML(s60settingsuis.confml)" \
    "conf/s60settingsuis_101F877A.crml 	  MW_LAYER_CRML(s60settingsuis_101f877a.crml)" \
    "conf/s60settingsuis_101F877B.crml 	  MW_LAYER_CRML(s60settingsuis_101f877b.crml)" \
    "conf/s60settingsuis_101F877C.crml 	  MW_LAYER_CRML(s60settingsuis_101f877c.crml)" \
    "conf/s60settingsuis_101F877E.crml 	  MW_LAYER_CRML(s60settingsuis_101f877e.crml)" \
    "conf/s60settingsuis_101F877F.crml 	  MW_LAYER_CRML(s60settingsuis_101f877f.crml)" \
    "conf/s60settingsuis_101F883B.crml 	  MW_LAYER_CRML(s60settingsuis_101f883b.crml)" \
    "conf/s60settingsuis_101F8751.crml 	  MW_LAYER_CRML(s60settingsuis_101f8751.crml)" \
    "conf/s60settingsuis_101F8780.crml 	  MW_LAYER_CRML(s60settingsuis_101f8780.crml)" \
    "conf/s60settingsuis_101F8781.crml 	  MW_LAYER_CRML(s60settingsuis_101f8781.crml)" \
    "conf/s60settingsuis_101F8782.crml 	  MW_LAYER_CRML(s60settingsuis_101f8782.crml)" \
    "conf/s60settingsuis_101F8831.crml 	  MW_LAYER_CRML(s60settingsuis_101f8831.crml)" \
    "conf/s60settingsuis_1000A82B.crml 	  MW_LAYER_CRML(s60settingsuis_1000a82b.crml)" \
    "conf/s60settingsuis_1020720A.crml 	  MW_LAYER_CRML(s60settingsuis_1020720a.crml)" \
    "conf/s60settingsuis_1028239D.crml 	  MW_LAYER_CRML(s60settingsuis_1028239d.crml)" \
    "conf/s60settingsuis_2002120B.crml 	  MW_LAYER_CRML(s60settingsuis_2002120b.crml)"
}

#  End of File
