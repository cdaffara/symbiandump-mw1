#
#############################################################################
##
## Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
## All rights reserved.
## This component and the accompanying materials are made available
## under the terms of "Eclipse Public License v1.0"
## which accompanies this distribution, and is available
## at the URL "http://www.eclipse.org/legal/epl-v10.html".
##
## Initial Contributors:
## Nokia Corporation - initial contribution.
##
## Contributors:
##
## Description:
##
#############################################################################
#

TEMPLATE = lib
TARGET = DevicePowerMenuPlugin
CONFIG += hb plugin

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE	

# Input
HEADERS += 	.\inc\hbdevicepowermenuplugin_p.h 	\
			.\inc\hbdevicepowermenupluginerrors_p.h 	\
			.\inc\hbdevicepowermenuwidegt_p.h	

			
SOURCES += 	.\src\hbdevicepowermenuplugin.cpp \
			.\src\hbdevicepowermenuwidegt.cpp
			
RESOURCES += hbdevicepowermenu.qrc

MOC_DIR = ./tmp

TRANSLATIONS	+=	power_menu.ts

symbian:
{
TARGET.UID3 = 0x2002F375
TARGET.CAPABILITY = ALL -TCB
TARGET.EPOCALLOWDLLDATA = 1

pluginstub.sources = DevicePowerMenuPlugin.dll
pluginstub.path = /resource/plugins/devicedialogs
DEPLOYMENT += pluginstub
}

symbian:
{ 
BLD_INF_RULES.prj_exports += \
  "$${LITERAL_HASH}include <platform_paths.hrh>" \
  "qmakepluginstubs/DevicePowerMenuPlugin.qtplugin   		/epoc32/data/z/pluginstub/DevicePowerMenuPlugin.qtplugin" \
  ".\rom\devicepowermenuplugin.iby             				CORE_MW_LAYER_IBY_EXPORT_PATH(devicepowermenuplugin.iby)" \
  ".\rom\devicepowermenu_resources.iby 						LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(devicepowermenu_resources.iby)"
}



