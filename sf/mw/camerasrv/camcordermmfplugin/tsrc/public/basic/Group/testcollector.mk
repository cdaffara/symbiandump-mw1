#
# Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:  GNU Makefile that updates the ATS3 drop for a test set.
#
TESTTYPE=STIF
DLLS=StifCamcTest.dll
DLLDIR=..\$(TESTTYPE)_temp


BUILD_DIR=\epoc32\RELEASE\$(PLATFORM)\$(CFG)

UPDATE_BINARIES := $(foreach DLL, $(DLLS), \
                   & xcopy /Q /Y /F "$(BUILD_DIR)\$(DLL)" "$(DLLDIR)")
UPDATE_BINARIES := $(wordlist 2, 1000, $(UPDATE_BINARIES))



# For these targets we shall not do anything.
MAKMAKE FREEZE LIB CLEANLIB RESOURCE RELEASABLES FINAL SAVESPACE:

# BLD target of this Makefile is executed after Symbian BLD target
# so the binaries are ready.
BLD: 
	echo Preparing Test collection...
	-mkdir "$(DLLDIR)" 2> NUL
	$(UPDATE_BINARIES)

# Clean the copied binaries from the ATS drop.
CLEAN:
	echo Cleaning Test collection...
	-rd /s /q "$(DLLDIR)" 2> NUL



