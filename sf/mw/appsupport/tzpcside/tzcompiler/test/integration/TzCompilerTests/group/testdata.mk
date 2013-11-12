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
# Invokes TzCompilerTests.exe for WINSCW platform.

!if "$(PLATFORM)"=="WINSCW"
TARGET_DATA_DIRECTORY=$(EPOCROOT)epoc32\winscw\c\logs\TzCompiler

TARGET_FILES=\
	$(TARGET_DATA_DIRECTORY)\TZDB_Test1.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test2.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test3.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test4.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test5.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test6.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test7.txt\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test8.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test9.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test10.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test11.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TZDB_Test12.DBZ\
 	$(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test13.txt\

# Tool that is to be invoked
TOOL= .\TzCompilerTests.exe

$(TARGET_DATA_DIRECTORY) :
	@perl -S emkdir.pl $(TARGET_DATA_DIRECTORY)\

# Invalid Default Zone(Asia/London) specified. Console output would indicate the invalid zone.
# The database will be successfully created with a valid zone. 
$(TARGET_DATA_DIRECTORY)\TZDB_Test1.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test1
	echo ========================================================
	copy ..\testdata\Test1\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test1\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test1.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test1.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test1.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 

# DefaultZone not set in TzCompiler.ini. Console output should indicate "Not Set". 
# A defect is raised on this.
$(TARGET_DATA_DIRECTORY)\TZDB_Test2.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test2
	echo ========================================================
	copy ..\testdata\Test2\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test2\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test2.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test2.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test2.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 

# Invalid value given for EarliestDateOfInterest. Console output should set it to default value(1980) or
# flag an error. A defect is raised on this.
$(TARGET_DATA_DIRECTORY)\TZDB_Test3.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test3
	echo ========================================================
	copy ..\testdata\Test3\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test3\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test3.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test3.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test3.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 

#EarliestDateOfInterest not set in TzCompiler.ini. It should be set to default value(1980).
$(TARGET_DATA_DIRECTORY)\TZDB_Test4.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test4
	echo ========================================================
	copy ..\testdata\Test4\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test4\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test4.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test4.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test4.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 
	
# InputDirectory field not set in TzCompiler.ini. It should be set to default Value(.\data).
# A defect is raised on this.
$(TARGET_DATA_DIRECTORY)\TZDB_Test5.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test5
	echo ========================================================
	copy ..\testdata\Test5\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test5\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test5.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test5.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test5.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 
	
# InputDirectory specifies an absolute path. Console output should match the ini value.
$(TARGET_DATA_DIRECTORY)\TZDB_Test6.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test6
	echo ========================================================
	copy ..\testdata\Test6\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test6\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test6.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test6.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test6.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 

#InputDirectory specifies an invalid path. Console output matches the ini file value but database
# is not generated.
$(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test7.txt : $(TOOL)
	echo Started executing Test7
	echo ========================================================
	copy ..\testdata\Test7\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test7\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test7.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test7.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test7.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	echo ========================================================
	
#IncludedRegions is not set to any value. All regions are included in the database.
$(TARGET_DATA_DIRECTORY)\TZDB_Test8.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test8
	echo ========================================================
	copy ..\testdata\Test8\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test8\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test8.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test8.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test8.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 
	
#IncludedRegions contains only one region. AvailableRegions field should contain only that region.
$(TARGET_DATA_DIRECTORY)\TZDB_Test9.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test9
	echo ========================================================
	copy ..\testdata\Test9\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test9\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test9.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test9.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test9.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 

#IncludedRegion	contains more than one regions (separated by comma). AvailableRegions field should
# contain only those regions.
$(TARGET_DATA_DIRECTORY)\TZDB_Test10.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test10
	echo ========================================================
	copy ..\testdata\Test10\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test10\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test10.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test10.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test10.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 

#IncludedRegions contain an invalid value. Validation to fail.
$(TARGET_DATA_DIRECTORY)\TZDB_Test11.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test11
	echo ========================================================
	copy ..\testdata\Test11\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test11\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test11.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test11.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test11.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	echo ======================================================== 
	
# Everything proper.
$(TARGET_DATA_DIRECTORY)\TZDB_Test12.DBZ : $(TOOL) 
	echo ========================================================
	echo Started executing Test12
	echo ========================================================
	copy ..\testdata\Test12\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test12\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test12.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test12.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test12.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	del .\tzdb.dbz
	echo ======================================================== 
	
# Time zone id beyond the range. Shouldn't create the database
$(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test13.txt : $(TOOL) 
	echo ========================================================
	echo Started executing Test13
	echo ========================================================
	copy ..\testdata\Test13\TzCompiler.ini .\TzCompiler.ini
	copy ..\testdata\Test13\TzIdentities.ini .\TzIdentities.ini
	@$(TOOL)
	echo ========================================================
	copy .\TestTzCompiler.log $(TARGET_DATA_DIRECTORY)\TestTzCompiler_Test13.txt
	copy .\TzCompiler.ini $(TARGET_DATA_DIRECTORY)\TzCompiler_Test13.ini
	copy .\TzIdentities.ini $(TARGET_DATA_DIRECTORY)\TzIdentities_Test13.ini
	del .\TzCompiler.ini
	del .\TzIdentities.ini
	del .\TestTzCompiler.log
	echo ======================================================== 

do_nothing:
	rem do nothing

# the targets below are the public ones

MAKMAKE : do_nothing

BLD : $(TARGET_DATA_DIRECTORY) $(TARGET_FILES)

SAVESPACE : BLD

FREEZE : do_nothing
LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

CLEAN :  
	-@erase $(TARGET_FILES)

RELEASABLES : 
	@echo $(TARGET_FILES)

FINAL : do_nothing	

ROMFILE : do_nothing


!else
# for non winscw targets

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
