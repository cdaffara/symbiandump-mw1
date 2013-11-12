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

HEADERS += ./inc/*.h
SOURCES += ./src/*.cpp
symbian:
{
    HEADERS += ../../../../contentstorage/tsrc/testutils/inc/*.h
    SOURCES += ../../../../contentstorage/tsrc/testutils/src/*.cpp
    SOURCES += ../../../../contentstorage/srvsrc/castorageproxy.cpp
    SOURCES += ../../../../contentstorage/srvsrc/cabackupnotifier.cpp
    SOURCES += ../../../../contentstorage/castorage/src/*.cpp
    
    
    BLD_INF_RULES.prj_testexports += "data/winscw/sapiapp_S60_3_X_v_1_0_0.sisx c:/testing/data/t_caclient/installs/sapiapp_S60_3_X_v_1_0_0.sisx"
    BLD_INF_RULES.prj_exports += "data/winscw/sapiapp_S60_3_X_v_1_0_0.sisx c:/testing/data/t_caclient/installs/sapiapp_S60_3_X_v_1_0_0.sisx"
}