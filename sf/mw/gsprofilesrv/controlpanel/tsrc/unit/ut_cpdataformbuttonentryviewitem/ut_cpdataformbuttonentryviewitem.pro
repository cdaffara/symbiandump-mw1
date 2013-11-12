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

TEMPLATE = app
TARGET = ut_cpdataformbuttonentryviewitem 

include (../ut_common.pri)
include (ut_cpdataformbuttonentryviewitem.pri)

symbian: { 
    TARGET.UID3 = 0xE43C01BC
}


symbian:MMP_RULES += SMPSAFE
