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
SUBDIRS = unit/pluginfortest/firstpluginfortest \
          unit/pluginfortest/secondpluginfortest \
          unit/pluginfortest/thirdpluginfortest \
          unit/testplugin \
	        unit/ut_cppluginloader \ 
          unit/ut_cpbasesettingview \
          unit/ut_cpdataformbuttonentryviewitem \
          unit/ut_cpdataformlistentryviewitem \
          unit/ut_cpitemdatahelper \
          unit/ut_cppluginlauncher \
          unit/ut_cppluginutility \
          unit/ut_cpsettingformentryitemdata \
          unit/ut_cpsettingformitemdata \
          unit/ut_cputility \
          unit/ut_cpviewlauncher \
	        unit/ut_cpapi 
	
CONFIG += ordered

test.depends = first
test.CONFIG += recursive
autotest.depends = first
autotest.CONFIG += recursive
QMAKE_EXTRA_TARGETS += test autotest
