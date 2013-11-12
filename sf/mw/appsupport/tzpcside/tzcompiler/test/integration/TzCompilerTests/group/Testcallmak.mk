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
# Build TzCompilerTests.exe. This tool will be built only for WINSCW platform.
#
!if "$(PLATFORM)"=="WINSCW"
# If CFG is "UREL", tool will be created in Release mode.

# Otherwise, the tool will be created in Debug mode.
TOOLNAME=TzCompilerTests

!if "$(CFG)" == "UREL"
VC_CFG="$(TOOLNAME) - Win32 Release"
!else
VC_CFG="$(TOOLNAME) - Win32 Debug"
CFG=UDEB
!endif

TARGETDIR=.\Debug
SRCDIR=.


$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

TOOL=$(TOOLNAME).exe

$(TOOL) :
	echo NMAKE /S /NOLOGO /f $(SRCDIR)\$(TOOLNAME).MAK CFG=$(VC_CFG)
	NMAKE /S /NOLOGO /f $(SRCDIR)\$(TOOLNAME).MAK CFG=$(VC_CFG)

do_nothing:
	rem do nothing

#
# The targets invoked by bld...
#

MAKMAKE : do_nothing

FREEZE : do_nothing

LIB : do_nothing

BLD : $(TARGETDIR) $(TOOL)

SAVESPACE : BLD

CLEAN : 
	NMAKE /S /NOLOGO /f $(SRCDIR)\$(TOOLNAME).MAK CFG=$(VC_CFG) CLEAN
	-erase $(SRCDIR)\$(TOOL)

CLEANLIB : do_nothing

RELEASABLES : 
	@echo $(TOOL)


RESOURCE : do_nothing

FINAL : do_nothing

ROMFILE : do_nothing

!else

do_nothing:
	rem do nothing


MAKMAKE : do_nothing

FREEZE : do_nothing

LIB : do_nothing

BLD : do_nothing

SAVESPACE : do_nothing

CLEAN : do_nothing

CLEANLIB : do_nothing

RELEASABLES : do_nothing


RESOURCE : do_nothing

FINAL : do_nothing

ROMFILE : do_nothing

!endif