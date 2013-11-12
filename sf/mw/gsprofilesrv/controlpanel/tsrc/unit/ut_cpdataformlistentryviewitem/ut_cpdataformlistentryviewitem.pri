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

HEADERS   += src/ut_cpdataformlistentryviewitem.h \
	src/mycpdataformlistentryviewitem.h \
	../../../src/cpframework/src/cpdataformlistentryviewitem.h \
	
SOURCES   += src/ut_cpdataformlistentryviewitem.cpp \
    src/mycpdataformlistentryviewitem.cpp \
    ../../../src/cpframework/src/cpdataformlistentryviewitem.cpp \

INCLUDEPATH += . \
	 src/\
	 ../../../src/inc \
	 ../../../controlpanel_plat/inc \
	 ../../../src/cpframework/src 
