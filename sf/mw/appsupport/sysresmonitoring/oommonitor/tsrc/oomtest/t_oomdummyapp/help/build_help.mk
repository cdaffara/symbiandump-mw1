# ============================================================================
#  Name	 : build_help.mk
#  Part of  : t_oomdummyapp
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : t_oomdummyapp
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : t_oomdummyapp_0xE6CFBA00.hlp
t_oomdummyapp_0xE6CFBA00.hlp : t_oomdummyapp.xml t_oomdummyapp.cshlp Custom.xml
	cshlpcmp t_oomdummyapp.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy t_oomdummyapp_0xE6CFBA00.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del t_oomdummyapp_0xE6CFBA00.hlp
	del t_oomdummyapp_0xE6CFBA00.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo t_oomdummyapp_0xE6CFBA00.hlp

FINAL : do_nothing
