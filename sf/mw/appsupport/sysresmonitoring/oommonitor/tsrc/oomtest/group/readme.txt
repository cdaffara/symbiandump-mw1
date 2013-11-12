To test in emulator:
	1) bldmake bldfiles + abld test build 
	2) start emulator
	3) run "consoleui" from "eshell"

To test (manually) in HW:
    1) bldmake bldfiles + abld test build
    2) create and sign sis from oomtest_manualtestrun.pkg (note that oomtest.pkg is for automated testing)
    3) install sis into phone
    4) select OOM test config xml from c:\data\ (use t_oomtestapp -> Options -> Select...)
    5) reboot phone (config xml is taken in use)
    6) rename oomtest.ini in c:\testframework to testframework.ini (use FileBrowse)
    7) run "consoleui" from "eshell"
