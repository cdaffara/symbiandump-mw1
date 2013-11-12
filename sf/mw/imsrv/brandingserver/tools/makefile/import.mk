#
# Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of the License "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  make file
#


# ----------------------------------------------------------------------------
# Application ID
# This should be the same string as is used as the application ID
# of this application in the brand data.
# This is used for cleaning the brand data when calling abld clean.
#
# !!IMPORTANT!!
#
# Note that all the brands for this application will be wiped!
# If this is not wanted, change the rule in CLEAN to do_nothing, but
# then you will get errors when recompiling, since the same brand data
# cannot be entered twice.
# ----------------------------------------------------------------------------

APPID=app_id


# ----------------------------------------------------------------------------
# Brand ID
# Name of the created brand
# ----------------------------------------------------------------------------
BRAND=default

# ----------------------------------------------------------------------------
# Brand version
# ----------------------------------------------------------------------------
VERSION=1

# ----------------------------------------------------------------------------
# Source directory of the brand XML:s.
# Just put every XML you want to import into this directory.
# Note that this directory is for emulator, so c-drive is pointing to
# \epoc32\winscw\c.
# ----------------------------------------------------------------------------
SOURCEDIR=c:\xml


# ----------------------------------------------------------------------------
# Directory to clean when abld clean is called
# ----------------------------------------------------------------------------
CLEANED_DIR=\epoc32\winscw\c\private\102828dd\data\$(APPID)

# ----------------------------------------------------------------------------
# Language definition
# This should stay unchanged. Build script will change this when needed
# ----------------------------------------------------------------------------
LANGUAGE=sc

# ----------------------------------------------------------------------------
# empty rules 
# ----------------------------------------------------------------------------
do_nothing :
	@rem do_nothing

# ----------------------------------------------------------------------------
# Build rules 
# ----------------------------------------------------------------------------

MAKMAKE : do_nothing

# ----------------------------------------------------------------------------
# Resources
# Runs the parsers and importer. Note that this will cause an error when
# compiling more than once with the same brand data without calling abld
# clean in between
#
# Note! Edit these to suit our application
# ----------------------------------------------------------------------------
RESOURCE : 
	perl -- \s60\brandingserver\tools\xml\loc2xml.pl \S60\someapp\Group\someapp.loc \epoc32\winscw\c\xml\someapp_loc_$(LANGUAGE).xml $(BRAND) $(APPID) $(LANGUAGE) $(VERSION)
	perl -- \s60\brandingserver\tools\xml\mbg2xml.pl \epoc32\include\someapp.mbg \epoc32\winscw\c\xml\someapp_gfx.xml $(BRAND) $(APPID) SC $(VERSION)
	\epoc32\release\winscw\udeb\bsimport.exe -dtextshell -- -D $(SOURCEDIR)

# ----------------------------------------------------------------------------
# Build
# ----------------------------------------------------------------------------
BLD : do_nothing

# ----------------------------------------------------------------------------
# Cleans all brand data for the given application.
# ----------------------------------------------------------------------------
CLEAN : 
	if exist $(CLEANED_DIR) rd /S /Q $(CLEANED_DIR)
	

LIB : do_nothing

CLEANLIB : do_nothing

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	do_nothing
	
FINAL : do_nothing
