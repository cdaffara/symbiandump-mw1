NOTE for emulator!
Because all components may not function well in emulator with reseted commsdat, take a backup of 
epoc32\winscw\c\private\10202be9\persists\cccccc00.cre 
file before running tests. Restore file if problems in emulator after running tests.

formatterrfsplugintest - EUnit tests for FormatterRfsPlugin
secureformattertest - STIF tests for SecureFormatter
    testdatacreator - Used by secureformattertest and rfstestapp for generating test data
rfstestapp - Test application for functional testing
    Test data is copied to /epoc32/winscw/e/testdata. To be able to use test data from there,
    replace
    //_EPOC_LocDrv_1 E:
    with 
    _EPOC_DRIVE_E \epoc32\winscw\e
    in epoc32\data\epoc_<resx>x<resy>.ini file(s)
msgcentrerfsplugintest - STIF tests for msgcentrerfsplugin
