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


PLATFORM_HEADERS += \
                    inc/homescreendomainpskeys.h
symbian {    
    #qcrml export
    BLD_INF_RULES.prj_exports += \
         "inc/homescreeninformation.qcrml  c:/resource/qt/crml/homescreeninformation.qcrml "
}


for(filename,PLATFORM_HEADERS){
    BLD_INF_RULES.prj_exports *= "$$filename MW_LAYER_PLATFORM_EXPORT_PATH($$basename(filename))"
  }
