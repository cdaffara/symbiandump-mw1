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
# Description: 
#

TEMPLATE = subdirs

SYMBIAN_PLATFORMS = WINSCW ARMV5

CONFIG += ordered

include ( controlpanel_api.pri )
include ( logger_api.pri )

symbian: {  
    BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" 
      
    #exports controlpanel header files
    headers.sources = $$CONTROLPANEL_PLAT_HEADERS
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "./$$header $$MW_LAYER_PLATFORM_EXPORT_PATH($$basename(header))"
    
    #exports logger header files
    headers.sources = $$CONTROLPANEL_LOG_PLAT_HEADERS
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "./$$header $$MW_LAYER_PLATFORM_EXPORT_PATH(cplogger/$$basename(header))"
}

