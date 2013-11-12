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
TARGET = HbDeviceInputDialogPlugin
CONFIG += hb plugin

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE




HEADERS += .\inc\hbdeviceinputdialogplugin_p.h \
		   .\inc\hbdeviceinputdialogpluginerrors_p.h \
           .\inc\hbdeviceinputdialogwidget_p.h

SOURCES += .\src\hbdeviceinputdialogplugin.cpp \
           .\src\hbdeviceinputdialogwidget.cpp

MOC_DIR = ./tmp

TRANSLATIONS	+=	filemanager.ts
symbian: {
    TARGET.UID3 = 0x2002F376
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1

    pluginstub.sources = HbDeviceInputDialogPlugin.dll
    pluginstub.path = /resource/plugins/devicedialogs
    DEPLOYMENT += pluginstub
}

symbian:
{ 
BLD_INF_RULES.prj_exports += \
  "$${LITERAL_HASH}include <platform_paths.hrh>" \
  "qmakepluginstubs/HbDeviceInputDialogPlugin.qtplugin   		/epoc32/data/z/pluginstub/HbDeviceInputDialogPlugin.qtplugin"\
  ".\rom\hbdeviceinputdialogplugin.iby             				CORE_MW_LAYER_IBY_EXPORT_PATH(hbdeviceinputdialogplugin.iby)" \
  ".\rom\hbdeviceinputdialogplugin_resources.iby 						LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(hbdeviceinputdialogplugin_resources.iby)"
}



