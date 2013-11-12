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

HEADERS   += src/ut_cpdataformbuttonentryviewitem.h \
	src/mycpdataformbuttonentryviewitem.h \
	../../../src/cpframework/src/cpdataformbuttonentryviewitem.h \
	
SOURCES   += src/ut_cpdataformbuttonentryviewitem.cpp \
    src/mycpdataformbuttonentryviewitem.cpp \
    ../../../src/cpframework/src/cpdataformbuttonentryviewitem.cpp \

INCLUDEPATH += . \
	 src/\
	 ../../../src/inc \
	 ../../../controlpanel_plat/inc \
	 ../../../src/cpframework/src 
